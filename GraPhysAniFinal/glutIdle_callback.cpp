#include "global.h"

#include "CPhysics/Physics.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
//#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <iostream>
#include <sstream>

void AddClosestPointDebugSpheres(void);

static const float g_MAXIMUM_TIME_STEP = 0.1f;		// 10th of a second or 100ms

float getDistanceBetweenPoints_YOLO3(CVector3f point1, CVector3f point2)
{
	float deltaXsquared = (point2.x - point1.x) * (point2.x - point1.x);  // pow()
	float deltaYsquared = (point2.y - point1.y) * (point2.y - point1.y);
	float deltaZsquared = (point2.z - point1.z) * (point2.z - point1.z);

	// There's a number of square root functions. 
	// You many have to include "math.h" or something else. 
	float distance = sqrt(deltaXsquared + deltaYsquared + deltaZsquared);

	return distance;
}

static const float g_MAXIMUM_TIME_TO_REACH_CHECKPOINT = 10.0f;  //20 seconds
static float countdownTime = 0.0f; //counts down to 0 if player doesn't make it to check point in time

void IdleFunction(void)
{
	// Update the "simulation" at this point.
	// 1. Get elapsed time
	g_simTimer.Stop();
	float deltaTime = ::g_simTimer.GetElapsedSeconds();
	// Is the number TOO big (we're in a break point or something?)
	if ( deltaTime > g_MAXIMUM_TIME_STEP )
	{	// Yup, so clamp it to a new value
		deltaTime = g_MAXIMUM_TIME_STEP;
	}
	g_simTimer.Start();

	int currentCountdownParticleID = -1;

	countdownTime += deltaTime;
	if (g_MAXIMUM_TIME_TO_REACH_CHECKPOINT - countdownTime < 9.0f)
	{
		currentCountdownParticleID = (int)g_MAXIMUM_TIME_TO_REACH_CHECKPOINT - (int)countdownTime;
	}

	if (g_MAXIMUM_TIME_TO_REACH_CHECKPOINT + 0.75f < countdownTime)
	{
		countdownTime = 0;
		std::wstringstream ss;
		ss << g_Player_ID;
		g_pPhysicsManager->respawnObject(ss.str());
	}

	if (g_pPhysicsManager->isPlayerInsideCheckPoint())
	{
		countdownTime = 0;
		g_pPhysicsManager->resetTimerToFalse();
	}

	if (::g_gameState.currentGameMode == CGameState::GAME_BOWLING) //used to display the number of squares the player's vehicle had hit
	{
		int finalScore = g_pPhysicsManager->GetFinalScore();
		if (finalScore != 0)
		{
			currentCountdownParticleID = finalScore;
		}
	}
	//CVector3f playerLocation;
	::g_p_GameController_0->Update( deltaTime );

	// Set the light at the same location as the player object...
	unsigned int objectID;
	if (::g_gameState.currentGameMode == CGameState::EDITING_OBJECTS)
		objectID = vecModelsInfo[g_gameState.selectedObjectID].ID;
	else
		objectID = ::g_Player_ID;


	static std::vector< IGameObjectPhysics* > vec_pGameObjects;
	g_pFactoryMediator->getPhysicsObjects( vec_pGameObjects );

	//::g_p_PhysicsThingy->simulationStep( vec_pGameObjects, deltaTime );

	/*for ( int index = 0; index != static_cast<int>( vec_pGameObjects.size() ); index++ )
	{
		vec_pGameObjects[index]->Update(deltaTime);
	}*/

	::g_pPhysicsManager->stepWorld(deltaTime);
	PhysicsMailMan playerOrientation;

	std::map<std::wstring, PhysicsMailMan> vecObjectOrientations = g_pPhysicsManager->getMatrix();
	for (std::map<std::wstring, PhysicsMailMan>::iterator itObject = vecObjectOrientations.begin();
		itObject != vecObjectOrientations.end(); itObject++)
	{
		//CObject* pObject = findObjectByName( itObject->ID );
		std::wstringstream ss;
		ss << itObject->first;
		int IDToFind;
		ss >> IDToFind;
		if (IDToFind == g_Player_ID)
		{
			playerOrientation = itObject->second;
		}

		CPhysicalProp pObject;
		// Find a match? 
		if (g_pFactoryMediator->getPhysicalPropertiesByID(IDToFind, pObject))
		{	// Yup, so update the position of the object
			pObject.setMailMan(itObject->second);

			g_pFactoryMediator->setPhysicalPropertiesByID(IDToFind, pObject);
		}
	}

	std::vector<CMeshDesc> vec_playerWheels;
	::g_pFactoryMediator->getRenderingPropertiesByID(::g_Player_ID, vec_playerWheels);
	if (vec_playerWheels.size() > 1){
		for (int i = 1; i < vec_playerWheels.size(); i++)
		{
			std::wstringstream ss;
			ss << ::g_Player_ID;
			glm::vec3 out_pos;
			glm::fquat out_rot;
			g_pPhysicsManager->GetWheelPositionRotation(ss.str(), i - 1, out_pos, out_rot);

			vec_playerWheels[i].relPosition.x = out_pos.x;
			vec_playerWheels[i].relPosition.y = out_pos.y;
			vec_playerWheels[i].relPosition.z = out_pos.z;

			vec_playerWheels[i].orientation = out_rot;
		}
		g_pFactoryMediator->setRenderingPropertiesByID(::g_Player_ID, vec_playerWheels);
	}

	static CPhysicalProp playerProps;
	::g_pFactoryMediator->getPhysicalPropertiesByID(objectID, playerProps);

	CPlyInfo shipModelInfo;
	g_pModelLoader->GetRenderingInfoByModelFileName("truck.ply", shipModelInfo);

	glm::fquat rotation;
	rotation = playerProps.orientation;
	
	glm::mat4 matOrientation = playerProps.matrix;

	glm::vec3 diffVect;
	diffVect.x = shipModelInfo.minExtentZ.x - shipModelInfo.minExtentZ.x;      //get direction to travel from the current point to the desired location
	diffVect.y = shipModelInfo.maxExtentZ.y - shipModelInfo.minExtentZ.y;
	diffVect.z = shipModelInfo.maxExtentZ.z - shipModelInfo.minExtentZ.z;
	diffVect = glm::normalize(diffVect);

	diffVect = glm::rotate(rotation, diffVect);

	

	float eyeDistFromShip = 10.5f;

	//glm::vec4 velThisFrame(diffVect.x, diffVect.y, diffVect.z, 1.0f);
	//glm::vec4 velAdjust = matOrientation * velThisFrame;
	// Update the linear velocity based on this acceleration step (this frame)
	if (::g_pCamera->getCameraState() == CCamera::FOLLOW_TARGET_AT_DISTANCE_CHASE_PLANE)
	{
		::g_pCamera->eye = CVector3f(playerProps.position.x + (diffVect.x * eyeDistFromShip), playerProps.position.y + (diffVect.y * eyeDistFromShip), playerProps.position.z + (diffVect.z * eyeDistFromShip));
			//CVector3f(playerProps.position.x + (velAdjust.x * eyeDistFromShip), playerProps.position.y + (velAdjust.y  * eyeDistFromShip), playerProps.position.z + (velAdjust.z  * eyeDistFromShip));
		::g_pCamera->eye.y += 3.5f;
	}


	// Added October 3, 2014
	::g_pCamera->Update( deltaTime );
		// Adjust camera based on the Right stick
	float rightJoyX = ::g_p_GameController_0->getRightJoystickXAxis();
	float rightJoyY = ::g_p_GameController_0->getRightJoystickYAxis();
	static const float lookSpeedScale = 0.01f;
	::g_pCamera->AdjustYaw( rightJoyX * lookSpeedScale );
	::g_pCamera->AdjustPitch( -rightJoyY * lookSpeedScale );
	// Move camera forward with "A"
	if ( ::g_p_GameController_0->bIsAButtonDown() )
	{
		::g_pCamera->MoveForwardBackward( deltaTime );
	}


	CPhysicalProp starProps;
	::g_pFactoryMediator->getPhysicalPropertiesByID(::g_StarHavokID, starProps);
	starProps.position = vecCheckPointsInfo[g_pPhysicsManager->getCurrentCheckPoint()];
	starProps.position.y += 5.0f;
	::g_pFactoryMediator->setPhysicalPropertiesByID(::g_StarHavokID, starProps);
	g_p_checkpointEmitter->SetLocation(starProps.position);
	g_p_checkpointEmitter->Update(deltaTime);

	CVector3f timerSpawnlocation = CVector3f(playerProps.position.x - (diffVect.x * eyeDistFromShip), playerProps.position.y - (diffVect.y * eyeDistFromShip), playerProps.position.z - (diffVect.z * eyeDistFromShip));
	timerSpawnlocation.y += 2.0f;

	g_p_timerEmitter->SetLocation(timerSpawnlocation);
	g_p_timerEmitter->Update(deltaTime);

	// Add a sphere wherever there is a particle...
	for ( int index = 0; index != ::g_vecPhysDebugObjects.size(); index++ )
	{
		delete g_vecPhysDebugObjects[index];
	}
	::g_vecPhysDebugObjects.clear();

	std::vector< CParticleEmitter::CParticle > vecParticles;
	if (drawParticles){
		// Ask for all the "alive" particles
		
		g_p_checkpointEmitter->GetParticles(vecParticles);
			for (int index = 0; index != vecParticles.size(); index++)
			{
				std::vector< CMeshDesc > vecMeshDescs;
				g_pFactoryMediator->getRenderingPropertiesByID(g_vecParticleID[g_vecParticleID.size() - 1], vecMeshDescs);

				CGameObject* pGameObject = new CGameObject();
				pGameObject->m_physicalProperties.position = vecParticles[index].position;
				CMeshDesc meshDesc;
				meshDesc = vecMeshDescs[0];
				meshDesc.bIsHavok = false;
				meshDesc.orientation.x = static_cast<float>(PI) / 2.0f;

				//meshDesc.modelName = "1x1_6_Star_2_Sided.ply";
				meshDesc.scale = 1.0f;
				meshDesc.tranparency = 0.5f;
				meshDesc.bIsTransparent = true;
				meshDesc.debugColour = CVector4f(1.0f, 1.0f, 1.0f, 1.0f);
				// Add it 
				pGameObject->addMeshDescription(meshDesc);
				::g_vecPhysDebugObjects.push_back(pGameObject);
			}

		
	}
	
	g_p_timerEmitter->GetParticles(vecParticles);
	if (currentCountdownParticleID != -1){
		for (int index = 0; index != vecParticles.size(); index++)
		{
			std::vector< CMeshDesc > vecMeshDescs;
			g_pFactoryMediator->getRenderingPropertiesByID(g_vecParticleID[currentCountdownParticleID], vecMeshDescs);                           /****Spins with it***/
			//vecMeshDescs[0].orientation = glm::fquat(glm::vec3(playerOrientation.orientation));                           /****Spins with it***/
			//vecMeshDescs[0].orientation.y = playerOrientation.orientation.x;
			//g_pFactoryMediator->setRenderingPropertiesByID(g_ParticleID, vecMeshDescs);

			CGameObject* pGameObject = new CGameObject();                           /****Spins with it***/
			pGameObject->m_physicalProperties.position = vecParticles[index].position;                            /****Spins with it***/
			//pGameObject->m_physicalProperties.matrix = playerOrientation.matrix;                           /****Spins with it***/
			//pGameObject->m_physicalProperties.orientation = glm::fquat(glm::vec3(playerOrientation.orientation));                           /****Spins with it***/
			pGameObject->m_physicalProperties.orientation.y = playerOrientation.orientation.x;   /****Spins with it***/

			//static const float oneDegreeInRadians = static_cast<float>(PI) / 180.0f;
			//pGameObject->m_physicalProperties.adjRotationalSpeedEuler(CVector3f(0.0f, oneDegreeInRadians*300, 0.0f));
			//glm::fquat rotAdjuFullStep = pGameObject->m_physicalProperties.orientation * pGameObject->m_physicalProperties.rotationalSpeed;
			// SLERP for this delta time
			//pGameObject->m_physicalProperties.orientation = glm::slerp(pGameObject->m_physicalProperties.orientation, rotAdjuFullStep, deltaTime);

			//pGameObject->m_physicalProperties.orientation.z = static_cast<float>(PI) / 2.0f;
			//pGameObject->m_physicalProperties.setOrientationEulerAngles(CVector3f(playerOrientation.x, playerOrientation.y, playerOrientation.z));
			CMeshDesc meshDesc;                           /****Spins with it***/
			meshDesc = vecMeshDescs[0];                           /****Spins with it***/
			meshDesc.bIsHavok = false;

			//meshDesc.orientation = glm::fquat(glm::vec3(playerOrientation));
			//meshDesc.orientation.z = static_cast<float>(PI) / 2.0f;

			meshDesc.scale = 3.0f;                           /****Spins with it***/
			meshDesc.tranparency = 0.5f;                           /****Spins with it***/
			meshDesc.bIsTransparent = true;                           /****Spins with it***/
			meshDesc.debugColour = CVector4f(1.0f, 1.0f, 1.0f, 1.0f);                           /****Spins with it***/
			// Add it 
			pGameObject->addMeshDescription(meshDesc);
			::g_vecPhysDebugObjects.push_back(pGameObject);
		}
	}

	/*for (int index = 0; index != ::g_vecPhysDebugObjects.size(); index++)
	{
		glm::fquat rotAdjuFullStep = g_vecPhysDebugObjects[index]->m_physicalProperties.orientation * g_vecPhysDebugObjects[index]->m_physicalProperties.rotationalSpeed;
		g_vecPhysDebugObjects[index]->m_physicalProperties.orientation = glm::slerp(g_vecPhysDebugObjects[index]->m_physicalProperties.orientation, rotAdjuFullStep, deltaTime);
		g_vecPhysDebugObjects[index]->Update(deltaTime);
	}*/


	if (isFocusedOnLight){
		CPhysicalProp lightBoxProp;
		::g_pFactoryMediator->getPhysicalPropertiesByID(::g_lightModelID, lightBoxProp);
		lightBoxProp.position = ::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->position;
		::g_pFactoryMediator->setPhysicalPropertiesByID(::g_lightModelID, lightBoxProp);
	}

	// Place light #0 near the viper
	::g_p_LightManager->GetLightPointer( 0 )->position 
			= CVector3f( playerProps.position.x, playerProps.position.y + 2.0f, playerProps.position.z );	
	::g_p_LightManager->GetLightPointer(1)->position
		= CVector3f(starProps.position.x, starProps.position.y + 2.0f, starProps.position.z);
	float lightColorChangeRate = 0.01f;
	if (g_p_LightManager->GetLightPointer(1)->ambientColour.x >= 1.0){
		g_p_LightManager->GetLightPointer(1)->ambientColour.z = 0.0f;
		g_p_LightManager->GetLightPointer(1)->ambientColour.y += lightColorChangeRate;
	}
	if (g_p_LightManager->GetLightPointer(1)->ambientColour.y >= 1.0)
	{
		g_p_LightManager->GetLightPointer(1)->ambientColour.x = 0.0f;
		g_p_LightManager->GetLightPointer(1)->ambientColour.z += lightColorChangeRate;
		//g_p_LightManager->GetLightPointer(1)->ambientColour = CVector3f(0.0f, 0.0f, 1.0f);
	}
	if (g_p_LightManager->GetLightPointer(1)->ambientColour.z >= 1.0)
	{
		g_p_LightManager->GetLightPointer(1)->ambientColour.y = 0.0f;
		g_p_LightManager->GetLightPointer(1)->ambientColour.x += lightColorChangeRate;
		//g_p_LightManager->GetLightPointer(1)->ambientColour = CVector3f(1.0f, 0.0f, 0.0f);
	}

	// ******************** 
	// Keep skybox around camera
	static CPhysicalProp skyboxProps;
	::g_pFactoryMediator->getPhysicalPropertiesByID( ::g_skyBoxID, skyboxProps );
	skyboxProps.position = ::g_pCamera->eye;
	::g_pFactoryMediator->setPhysicalPropertiesByID( ::g_skyBoxID, skyboxProps );

	float thrust = (25.0f * ::g_p_GameController_0->getRightTrigger() ) - ( 2.5f * ::g_p_GameController_0->getLeftTrigger() );
	CVector4f directedVel = CVector4f( 0.0f, 0.0f, ( thrust * deltaTime ), 0.0f );
	::g_pFactoryMediator->SendMessageToObject( ::g_Player_ID, 0, CMessage( CNameValuePair( "SetDirectedVelocity", directedVel ) ) );


	CVector4f orientationAdj;
	if ( ::g_p_GameController_0->bIsGamePadLeft() )			{ orientationAdj.y = -deltaTime; }
	else if ( ::g_p_GameController_0->bIsGamePadRight() )	{ orientationAdj.y = +deltaTime; }
	if ( ::g_p_GameController_0->bIsGamePadUp() )			{ orientationAdj.x = -deltaTime; }
	else if ( ::g_p_GameController_0->bIsGamePadDown() )	{ orientationAdj.x = +deltaTime; }

	if ( ::g_p_GameController_0->bIsLeftButtonDown() ) { orientationAdj.z = -deltaTime; }
	if ( ::g_p_GameController_0->bIsRightButtonDown() ) { orientationAdj.z = +deltaTime; }
	::g_pFactoryMediator->SendMessageToObject( ::g_Player_ID, 0, CMessage( CNameValuePair( "adjOrientationEulerAngles", orientationAdj ) ) );

	if ( ::g_gameState.bPrintViperPhysicsStats )
	{
		CPhysicalProp viperPhysProps;
		::g_pFactoryMediator->getPhysicalPropertiesByID( ::g_Player_ID, viperPhysProps );
		std::cout << "Viper thrust = " << viperPhysProps.directedVelocity.z 
			<< " accel = " << viperPhysProps.directedAcceleration.z 
			<< " vel( " << viperPhysProps.velocity.x << ", " 
						<< viperPhysProps.velocity.y << ", " 
						<< viperPhysProps.velocity.z << ")"
			<< " pos( " << viperPhysProps.position.x << ", "
						<< viperPhysProps.position.y << ", "
						<< viperPhysProps.position.z << ") " << std::endl;
	}

	glutPostRedisplay();
	return;
}