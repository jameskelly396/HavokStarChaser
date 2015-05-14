#include "CPhysicsCalculatron.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

CPhysicsCalculatron::CPhysicsCalculatron()
{
	return;
}

CPhysicsCalculatron::~CPhysicsCalculatron()
{
	return;
}

bool CPhysicsCalculatron::simulationStep( std::vector< IGameObjectPhysics* > &vecGameObjects, float deltaTime )
{
	// Go through all the objects, updating them with gravity, etc.

	CPhysicalProp curPhysProps;
	for ( std::vector< IGameObjectPhysics* >::iterator itGO = vecGameObjects.begin(); itGO != vecGameObjects.end(); itGO++ )
	{
		// Get the physical properties of this object
		(*itGO)->getPhysicalProperties(curPhysProps);
		
		// Update this one?
		if ( curPhysProps.bIncludeInPhysicsUpdate )
		{	// yup, so do the magic
			// Is this infinitely massive (i.e. don't integrate stuff)
			(*itGO)->getPhysicalProperties( curPhysProps );
			if ( ! curPhysProps.bInfiniteMass )
			{	// Update it
				
				// There are a number of phases to this, and you'll get different results depending on the order
				// (bad news: there is no "right" order. Sorry.)

				// 1. Update orientation based on rotational speed
				// 2. Update acceleration based on directedAcceleration (aka "impulse thrust" along orientation)
				// 3. Update velocity based on directedVelocity (aka "thrust" along orientation)
				// 4. Update velocity based on acceleration
				// 5. Update position based on velocity

				// Update rotational stuff first because, yolo!
				// _      ___       _          _    _                   _                          _ 
				/// |    | _ \ ___ | |_  __ _ | |_ (_) ___  _ _   __ _ | |  ___ _ __  ___  ___  __| |
				//| | _  |   // _ \|  _|/ _` ||  _|| |/ _ \| ' \ / _` || | (_-<| '_ \/ -_)/ -_)/ _` |
				//|_|(_) |_|_\\___/ \__|\__,_| \__||_|\___/|_||_|\__,_||_| /__/| .__/\___|\___|\__,_|
				//                                                             |_|                   
				 glm::fquat rotAdjuFullStep = curPhysProps.orientation * curPhysProps.rotationalSpeed;
				// SLERP for this delta time
				curPhysProps.orientation = glm::slerp( curPhysProps.orientation, rotAdjuFullStep, deltaTime );

				// ___      ___   _                _            _                      _                  _    _            
				//|_  )    |   \ (_) _ _  ___  __ | |_  ___  __| |  __ _  __  __  ___ | | ___  _ _  __ _ | |_ (_) ___  _ _  
				// / /  _  | |) || || '_|/ -_)/ _||  _|/ -_)/ _` | / _` |/ _|/ _|/ -_)| |/ -_)| '_|/ _` ||  _|| |/ _ \| ' \ 
				///___|(_) |___/ |_||_|  \___|\__| \__|\___|\__,_| \__,_|\__|\__|\___||_|\___||_|  \__,_| \__||_|\___/|_||_|
				CVector3f dirAccelThisFrame = CVector3f::ScalarMultiply( curPhysProps.directedAcceleration, deltaTime );
				curPhysProps.directedVelocity += dirAccelThisFrame;
				//// Scale acceleration
				//glm::vec4 accelThisFrame( curPhysProps.directedAcceleration.x * deltaTime, 
				//	                      curPhysProps.directedAcceleration.y * deltaTime, 
				//						  curPhysProps.directedAcceleration.z * deltaTime, 1.0f );
				//// Calculate adjustment based on orientation (just like in the shader)
				//glm::vec4 accelAdjust = matOrientation * accelThisFrame; 
				//// Update the directed Velocity based on this acceleration step (this frame)
				//// (Note: the directed velocity updates the linear velocity in the next step, so 
				////	we don't have to update the linear acceleration also)
				//curPhysProps.acceleration += CVector3f( accelAdjust.x, accelAdjust.y, accelAdjust.z );

				// ____     ___   _                _            _             _           _  _         
				//|__ /    |   \ (_) _ _  ___  __ | |_  ___  __| | __ __ ___ | | ___  __ (_)| |_  _  _ 
				// |_ \ _  | |) || || '_|/ -_)/ _||  _|/ -_)/ _` | \ V // -_)| |/ _ \/ _|| ||  _|| || |
				//|___/(_) |___/ |_||_|  \___|\__| \__|\___|\__,_|  \_/ \___||_|\___/\__||_| \__| \_, |
				//                                                                                |__/ 
				// Scale acceleration
				glm::vec4 velThisFrame( curPhysProps.directedVelocity.x * deltaTime, 
					                    curPhysProps.directedVelocity.y * deltaTime, 
										curPhysProps.directedVelocity.z * deltaTime, 1.0f );
				// Calculate adjustment based on orientation (just like in the shader)
				glm::mat4 matOrientation = glm::toMat4( curPhysProps.orientation );
				glm::vec4 velAdjust = matOrientation * velThisFrame; 
				// Update the linear velocity based on this acceleration step (this frame)
				curPhysProps.velocity += CVector3f( velAdjust.x, velAdjust.y, velAdjust.z );

				// _ _       __ _  _                      __   __   __     _           _  _            __                                       _                  _    _            
				//| | |     / /| |(_) _ _   ___  __ _  _ _\ \  \ \ / /___ | | ___  __ (_)| |_  _  _   / _| _ _  ___  _ __    __ _  __  __  ___ | | ___  _ _  __ _ | |_ (_) ___  _ _  
				//|_  _|_  | | | || || ' \ / -_)/ _` || '_|| |  \ V // -_)| |/ _ \/ _|| ||  _|| || | |  _|| '_|/ _ \| '  \  / _` |/ _|/ _|/ -_)| |/ -_)| '_|/ _` ||  _|| |/ _ \| ' \ 
				//  |_|(_) | | |_||_||_||_|\___|\__,_||_|  | |   \_/ \___||_|\___/\__||_| \__| \_, | |_|  |_|  \___/|_|_|_| \__,_|\__|\__|\___||_|\___||_|  \__,_| \__||_|\___/|_||_|
				//          \_\                           /_/                                  |__/                                                                                  
				CVector3f accAdjustThisStep = CVector3f::ScalarMultiply( curPhysProps.acceleration, deltaTime );
				curPhysProps.velocity += accAdjustThisStep;

				// ___       __ _  _                      __    ___           _  _    _               __                              _           _  _         
				//| __|     / /| |(_) _ _   ___  __ _  _ _\ \  | _ \ ___  ___(_)| |_ (_) ___  _ _    / _| _ _  ___  _ __   __ __ ___ | | ___  __ (_)| |_  _  _ 
				//|__ \ _  | | | || || ' \ / -_)/ _` || '_|| | |  _// _ \(_-<| ||  _|| |/ _ \| ' \  |  _|| '_|/ _ \| '  \  \ V // -_)| |/ _ \/ _|| ||  _|| || |
				//|___/(_) | | |_||_||_||_|\___|\__,_||_|  | | |_|  \___//__/|_| \__||_|\___/|_||_| |_|  |_|  \___/|_|_|_|  \_/ \___||_|\___/\__||_| \__| \_, |
				//          \_\                           /_/                                                                                             |__/ 
				CVector3f velAdjustThisStep = CVector3f::ScalarMultiply( curPhysProps.velocity, deltaTime );
				curPhysProps.position += velAdjustThisStep;


				// And update the object
				(*itGO)->setPhysicalProperties( curPhysProps );
			}
		}// if ( (*itGO)->bIncludeInPhysicsUpdate )
	}// for ( std::vector< CGameObject* >::iterator itGO...


	// Now go through again, but setting commit...
	for ( std::vector< IGameObjectPhysics* >::iterator itGO = vecGameObjects.begin(); itGO != vecGameObjects.end(); itGO++ )
	{
		(*itGO)->commitPhysicalProperties();
	}

	return true;
}
