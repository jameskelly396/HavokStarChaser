#include "CHavokPhysics.h"

// Math and base include

//Havok headers
// Keycode
#include <Common/Base/keycode.cxx>
#include <Common/Base/Config/hkProductFeatures.cxx>
#include <Common/Base/System/Init/PlatformInit.cxx>

#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/System/Error/hkDefaultError.h>
#include <Common/Base/Memory/System/hkMemorySystem.h>
#include <Common/Base/Memory/System/Util/hkMemoryInitUtil.h>
#include <Common/Base/Memory/Allocator/Malloc/hkMallocAllocator.h>
#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>
#include <Physics2012/Collide/Dispatch/hkpAgentRegisterUtil.h>

#include <Physics2012/Collide/Shape/Convex/Box/hkpBoxShape.h>	
#include <Physics2012/Collide/Shape/Convex/Sphere/hkpSphereShape.h>	

#include <Physics/Constraint/Data/BallAndSocket/hkpBallAndSocketConstraintData.h>

#include <Physics2012/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>	

//Vehicle
#include <Physics2012/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesShape.h>
#include <Common/Internal/ConvexHull/hkGeometryUtility.h>
#include <Common/Base/Math/Matrix/hkMatrix3Util.h>


#include <sstream>
#include <vector>
std::vector<hkpRigidBody*> g_collidingEntities;

void MyEndOfStepListener::collisionAddedCallback( const hkpCollisionEvent& event )
{
	registerForEndOfStepContactPointCallbacks( event );
	hkpRigidBody* rigidBodyA = event.getBody(0);
	hkpRigidBody* rigidBodyB = event.getBody(1);

	const char* nameA = rigidBodyA->getName();
	const char* nameB = rigidBodyB->getName();

	bool bIsPlayer = false;
	if (strcmp(nameA, "Player") == 0 || strcmp(nameB, "Player") == 0)
		bIsPlayer = true;

	bool bIsSquare = false;
	if (strcmp(nameA, "Box") == 0 || strcmp(nameB, "Box") == 0)
		bIsSquare = true;

	if (bIsSquare && bIsPlayer){
		this->m_totalScore = 1;
		//std::cout << nameA << " ball hit floor" << std::endl;
	}
	//std::cout << "Objects collide" << std::endl; std::cout.flush();
}

void MyEndOfStepListener::collisionRemovedCallback( const hkpCollisionEvent& event )
{
	unregisterForEndOfStepContactPointCallbacks( event );
}

void MyEndOfStepListener::contactPointCallback( const hkpContactPointEvent& event )
{
	if ( event.m_contactPointProperties->m_flags & hkContactPointMaterial::CONTACT_IS_NEW )
	{
		//hkpConstraintInstance* constraint = event.m_bodies[0]->getConstraint(0);
		hkpRigidBody* rigidBody = event.m_bodies[0];
		hkpRigidBody* rigidBody2 = event.m_bodies[1];
		g_collidingEntities.push_back(rigidBody);
		g_collidingEntities.push_back(rigidBody2);
		int breakpoint = 0;
	}
}

void CPhantomListener::collidableAddedCallback(const hkpCollidableAddedEvent &event)
{
	hkpRigidBody* body = hkpGetRigidBody(event.m_collidable);
	if (!body) return;
	
	
	const char* name = body->getName();
	if (strcmp(name, "Player") == 0)
	{
		//std::cout << "Running Havok" << std::endl; std::cout.flush();
		isPlayerID = true;
	}
}

void CPhantomListener::collidableRemovedCallback(const hkpCollidableRemovedEvent &event)
{
	hkpRigidBody* body = hkpGetRigidBody(event.m_collidable);
	if (!body) return;

	const char* name = body->getName();
	if (strcmp(name, "Player") == 0)
	{
		//isPlayerID = false;
	}
}

CHavokPhysics::CHavokPhysics()
{
	this->m_bUsingVisualDebugger = false;
}

CHavokPhysics::~CHavokPhysics()
{
}

namespace CHavokPhysicsHelperFunctions
{
	std::string UNICODEtoASCII( std::wstring UNICODEstring )
	{
		std::stringstream ssReturnASCII;
		for ( std::wstring::iterator itChar = UNICODEstring.begin(); itChar != UNICODEstring.end(); itChar++ )
		{
			char theChar = static_cast<char>( *itChar );
			ssReturnASCII << theChar;
		}
		return ssReturnASCII.str();
	} 

	std::wstring ASCIItoUNICODE( std::string ASCIIstring )
	{
		std::wstringstream ssReturnUNICODE;
		for ( std::string::iterator itChar = ASCIIstring.begin(); itChar != ASCIIstring.end(); itChar++ )
		{
			wchar_t theChar = static_cast<wchar_t>( *itChar );
			ssReturnUNICODE << theChar;
		}
		return ssReturnUNICODE.str();
	}
};	// CHavokPhysicsHelperFunctions

static std::wstring static_errorString;

// Again, taken right from the Havok demo...
static void HK_CALL errorReport(const char* msg, void* userContext)
{
	using namespace std;
	std::string errorStringTemp;
	errorStringTemp.append( msg );
	static_errorString = CHavokPhysicsHelperFunctions::ASCIItoUNICODE( errorStringTemp );
	printf("%s\n", msg);
}

std::wstring CHavokPhysics::GetLastErrorString( void )
{
	return static_errorString;
}

void OnError(const char* msg, void* userArgGivenToInit) {
	std::cerr << "Report: " << msg << std::endl; 
}

void CHavokPhysics::Init( const std::vector< CNameValuePair2 > &vecParams )
{

	// Shamelessly stolen from the Havok Physics demo...
	PlatformInit();
	
	//hkMallocAllocator baseMalloc;
	// Need to have memory allocated for the solver. Allocate 1mb for it.
	//hkMemoryRouter* memoryRouter = hkMemoryInitUtil::initDefault( &baseMalloc, hkMemorySystem::FrameInfo(1024 * 1024) );
	this->m_pHavok_memoryRouter = hkMemoryInitUtil::initDefault( hkMallocAllocator::m_defaultMallocAllocator, hkMemorySystem::FrameInfo(1024 * 1024) );
	//hkBaseSystem::init( memoryRouter, errorReport );
	hkBaseSystem::init( this->m_pHavok_memoryRouter, errorReport );
	
	//hkpWorld* world = new hkpWorld( hkpWorldCinfo() );
	
	hkpWorldCinfo info;
	/*hkVector4 max(5000,5000,5000,0);
	hkVector4 min(-5000,-5000,-5000,0);
	info.m_broadPhaseWorldAabb.m_max = max;
	info.m_broadPhaseWorldAabb.m_min = min;*/
	info.m_gravity.set( 0,-9.8f,0);
	info.setBroadPhaseWorldSize( 2000.0f );
	info.setupSolverInfo( hkpWorldCinfo::SOLVER_TYPE_8ITERS_HARD );
	//info.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_CONTINUOUS;	// SLOWER
	info.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_DISCRETE;
	info.m_fireCollisionCallbacks = true;
	this->m_pHavok_world = new hkpWorld( info );
	int test = this->m_pHavok_world->getAllocatedSize();
	
	//hkpAgentRegisterUtil::registerAllAgents( world->getCollisionDispatcher() );
	hkpAgentRegisterUtil::registerAllAgents( this->m_pHavok_world->getCollisionDispatcher() );

	// Register all the physics viewers
	hkpPhysicsContext::registerAllPhysicsProcesses(); 

	// Set up a physics context containing the world for the use in the visual debugger
	this->m_pPhysicsContext = new hkpPhysicsContext;
	//context->addWorld(world); 
	this->m_pPhysicsContext->addWorld(this->m_pHavok_world); 


	// Set up the visual debugger?
	std::vector< CNameValuePair2 >::const_iterator itDebug = this->m_findNameValuePair( L"VisualDebugger", vecParams );
	if ( itDebug != vecParams.end() )
	{	// There is a param. 
		if ( itDebug->bValue )
		{

			//hkVisualDebugger* visualDebugger;
			// Setup the visual debugger
			hkArray<hkProcessContext*> contexts;
			contexts.pushBack(this->m_pPhysicsContext);

			//visualDebugger = new hkVisualDebugger(contexts);
			this->m_pVisualDebugger = new hkVisualDebugger(contexts);
			//visualDebugger->serve();
			this->m_pVisualDebugger->serve();	
			this->m_pVisualDebugger->addDefaultProcess("Phantom");

			this->m_bUsingVisualDebugger = true;
		}
	}

	m_respawnPoint = hkVector4(0.0f, 5.0f, 0.0f, 0.0f);
	m_totalScore = 0;

	return;
}

std::vector< CNameValuePair2 >::const_iterator 
		CHavokPhysics::m_findNameValuePair( std::wstring nameToFind, 
	                                        const std::vector< CNameValuePair2 > &vecNameValuePair )
{
	std::vector< CNameValuePair2 >::const_iterator itNVPair;
	for ( itNVPair = vecNameValuePair.begin(); itNVPair != vecNameValuePair.end(); itNVPair++ )
	{
		if ( itNVPair->name == nameToFind )
		{
			return itNVPair;
		}
	}
	return vecNameValuePair.end();
}

void CHavokPhysics::CreatePhantomCheckPoint(std::wstring ID, CVector3f min, CVector3f max)
{
	{
		hkAabb info; 
		info.m_min = hkVector4(min.x, min.y, min.z);
		info.m_max = hkVector4(max.x, max.y, max.z);
		hkpAabbPhantom* tempPhantom;
		tempPhantom = new hkpAabbPhantom(info);
		//this->m_mapPhantoms[ID] = tempPhantom;
		this->m_Phantom = tempPhantom;
	}
	/*this->m_mapPhantomListeners[ID] = new CPhantomListener();
	this->m_mapPhantomListeners[ID]->isPlayerID = false;
	this->m_mapPhantoms[ID]->addPhantomOverlapListener(this->m_mapPhantomListeners[ID]);

	this->m_pHavok_world->addPhantom(this->m_mapPhantoms[ID]);
	this->m_mapPhantoms[ID]->removeReference();*/

	this->m_PhantomListener = new CPhantomListener();
	this->m_PhantomListener->isPlayerID = false;
	this->m_Phantom->addPhantomOverlapListener(this->m_PhantomListener);

	this->m_pHavok_world->addPhantom(this->m_Phantom);
	//this->m_Phantom->removePhantomOverlapListener(this->m_PhantomListener);
	this->m_Phantom->removeReference();
	return;
}

void CHavokPhysics::SetCheckPoints(std::vector<CVector3f> vecCheckPointsInfo)
{
	this->m_vecCheckPoints = vecCheckPointsInfo;
	this->currentCheckPoint = 0;
	if (m_vecCheckPoints.size() > 0){
		makeNextCheckPoint();
	}
}

void CHavokPhysics::makeNextCheckPoint()
{
	CVector3f minSize(m_vecCheckPoints[this->currentCheckPoint]);
	CVector3f maxSize(m_vecCheckPoints[this->currentCheckPoint].x + 5.0f,
		m_vecCheckPoints[this->currentCheckPoint].y + 5.0f, m_vecCheckPoints[this->currentCheckPoint].z + 5.0f);
	std::wstringstream ss;
	ss << this->currentCheckPoint;
	CreatePhantomCheckPoint(ss.str(), minSize, maxSize);
}

void CHavokPhysics::CreateRigidBody(std::wstring ID, CVector3f box, float mass, float friction, CVector3f position, CVector4f Qrotation, bool moveable)
{
		/*hkpRigidBody* rigidBody;*/
	{
		hkVector4 halfExtents; halfExtents.set(box.x/2.0f, box.y/2.0f, box.z/2.0f);
		//hkVector4 halfExtents; halfExtents.set(box.x, box.y, box.z);
		hkpBoxShape* boxShape = new hkpBoxShape(halfExtents);

		hkpRigidBodyCinfo bodyCinfo;
		bodyCinfo.m_shape = boxShape;
		bodyCinfo.m_solverDeactivation = bodyCinfo.SOLVER_DEACTIVATION_MEDIUM;
		// Place box somewhere above floor...
		//bodyCinfo.m_position = hkVector4( position.x, position.y, position.z, 0.0f );
		bodyCinfo.m_position = hkVector4(position.x + box.x / 2.0f, position.y + box.y / 2.0f, position.z + box.z / 2.0f, 0.0f);
		bodyCinfo.m_rotation = hkQuaternion( Qrotation.x, Qrotation.y, Qrotation.z, 1.0f );
		bodyCinfo.m_rotation.normalize();
		bodyCinfo.m_restitution = 1.0f;
		bodyCinfo.m_friction = friction;
		if(moveable)
		{
			//  Calculate the mass properties for the shape
			const hkReal boxMass = mass;
			//hkpMassProperties massProperties;		// Deprecated for 2012 (was 2011)
			hkMassProperties massProperties;		// For 2012
			hkpInertiaTensorComputer::computeShapeVolumeMassProperties(boxShape, boxMass, massProperties);

			bodyCinfo.setMassProperties(massProperties);
		}
		else
		{
			bodyCinfo.m_motionType = hkpMotion::MOTION_FIXED;
		}
		// Create the rigid body
		//rigidBody = new hkpRigidBody(bodyCinfo);
		hkpRigidBody *tempRigidBody = new hkpRigidBody(bodyCinfo);
		if (moveable) 
			tempRigidBody->setName("Box");
		else 
			tempRigidBody->setName("Other");
		this->m_mapRigidBody[ID] = tempRigidBody;
		// No longer need the reference on the boxShape, as the rigidBody now owns it
		boxShape->removeReference();
	}

	CreateCollisionListener(ID, ID);

	// Add the rigidBody to the world
	//world->addEntity(rigidBody);
	//this->m_pHavok_world->addEntity(rigidBody);
	this->m_pHavok_world->addEntity(this->m_mapRigidBody[ID]);
	this->m_mapRigidBody[ID]->removeReference();
	return;
}

void CHavokPhysics::CreateRigidBody(std::wstring ID, float radius, float mass, 
	                                CVector3f position, CVector4f Qrotation, bool moveable, bool isStar)
{
		/*hkpRigidBody* rigidBody;*/
	{
		hkpSphereShape* sphereShape = new hkpSphereShape(radius);

		hkpRigidBodyCinfo bodyCinfo;
		bodyCinfo.m_shape = sphereShape;
		bodyCinfo.m_solverDeactivation = bodyCinfo.SOLVER_DEACTIVATION_MEDIUM;
		// Place box somewhere above floor...
		bodyCinfo.m_position = hkVector4( position.x, position.y, position.z, 0.0f );
		//bodyCinfo.m_rotation = hkQuaternion( Qrotation.x, Qrotation.y, Qrotation.z, Qrotation.a );
		//bodyCinfo.m_rotation.normalize();
		bodyCinfo.m_restitution = 1.0f;
		bodyCinfo.m_friction = 1.5f;
		
		if(moveable)
		{
			//  Calculate the mass properties for the shape
			const hkReal sphereMass = mass;
			//hkpMassProperties massProperties;		// Deprecated for 2012 (was in 2011)
			hkMassProperties massProperties;		// new for 2012
			hkpInertiaTensorComputer::computeShapeVolumeMassProperties(sphereShape, sphereMass, massProperties);

			bodyCinfo.setMassProperties(massProperties);
		}
		else
		{
			bodyCinfo.m_motionType = hkpMotion::MOTION_FIXED;
		}
		// Create the rigid body
		//rigidBody = new hkpRigidBody(bodyCinfo);
		hkpRigidBody *tempRigidBody = new hkpRigidBody(bodyCinfo);
		
		if (isStar){
			tempRigidBody->setGravityFactor(0.0f);
			tempRigidBody->setName("Star");
		}
		else
			tempRigidBody->setName("Other");
		this->m_mapRigidBody[ID] = tempRigidBody;
		// No longer need the reference on the boxShape, as the rigidBody now owns it
		sphereShape->removeReference();
	}

	hkpRigidBody* temp = this->m_mapRigidBody[ID];
	// Add the rigidBody to the world
	//world->addEntity(rigidBody);
	//this->m_pHavok_world->addEntity(rigidBody);
	this->m_pHavok_world->addEntity(this->m_mapRigidBody[ID]);
	this->m_mapRigidBody[ID]->removeReference();
	return;
}

unsigned int CHavokPhysics::GetFinalScore()
{
	int totalScore = 0;
	for (std::map<std::wstring, MyEndOfStepListener*>::iterator it = this->m_mapListeners.begin(); it != this->m_mapListeners.end(); it++)
	{
		totalScore += it->second->m_totalScore;
	}
	return totalScore;
}


void CHavokPhysics::BuildVehicle(std::wstring ID, CVector3f boxSize, float mass, CVector3f position, CVector4f Qrotation, bool moveable, bool isPlayer)
{
	//hkReal truckWidth = boxSize.x; //1.75f;
	//hkReal truckHeight = boxSize.y; //1.57f;  // removed clearance
	//hkReal truckLength = boxSize.z; //5.f;

	hkReal truckWidth = 1.75f;
	hkReal truckHeight = 1.57f;  // removed clearance
	hkReal truckLength = 5.f;

	// siimplest box to test
	hkReal xSize = truckWidth / 2.f;
	hkReal ySize = truckHeight / 2.f;
	hkReal zSize = truckLength / 2.f;

	int numVertices = 14;

	int stride = sizeof(hkReal)* 4;
	HK_ALIGN_REAL(hkReal vertices[]) = {
		xSize, -ySize, -zSize, 0.0f,		// bottom back right corner			v0
		-xSize, -ySize, -zSize, 0.0f,		// bottom back left corner			v1
		xSize, ySize - 0.55f, -zSize, 0.0f,		// top back right box corner	v2
		-xSize, ySize - 0.55f, -zSize, 0.0f,	// top back left box corner			v3
		xSize, -ySize, zSize, 0.0f,		// bottom front right corner		v4
		-xSize, -ySize, zSize, 0.0f,		// borrom front left corner			v5
		xSize, ySize - 0.6f, zSize, 0.0f,	// front right hood					v6
		-xSize, ySize - 0.6f, zSize, 0.0f,	// front left hood corner			v7
		xSize, ySize - 0.55f, zSize - 1.21f, 0.0f, // back right hood corner		v8
		-xSize, ySize - 0.55f, zSize - 1.21f, 0.0f,  // back left hood corner		v9
		xSize, ySize, zSize - 1.86f, 0.0f,		// front right roof corner		v10
		-xSize, ySize, zSize - 1.86f, 0.0f,		// front left roof corner		v11
		xSize, ySize, -zSize + 1.4f, 0.0f,		// back right roof corner		v12
		-xSize, ySize, -zSize + 1.4f, 0.0f		// back left roof corner		v13
	};


	//
	// SHAPE CONSTRUCTION.
	//

	hkStridedVertices		stridedVerts;
	stridedVerts.m_numVertices = numVertices;
	stridedVerts.m_striding = stride;
	stridedVerts.m_vertices = vertices;

	hkpConvexVerticesShape* chassisShape = new hkpConvexVerticesShape(stridedVerts);

	hkpRigidBodyCinfo bodyCinfo;
	bodyCinfo.m_mass = 750.0f;
	bodyCinfo.m_shape = chassisShape;
	bodyCinfo.m_friction = 0.8f;
	bodyCinfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

	bodyCinfo.m_position.set(position.x, position.y, position.z);
	//bodyCinfo.m_position.set(0.0f, 0.0f, 0.0);
	// Inertia tensor will be set by VehicleSetup.
	hkMatrix3Util::_setDiagonal(1.0f, 1.0f, 1.0f, bodyCinfo.m_inertiaTensor);
	bodyCinfo.m_centerOfMass.set(0.f, 0.f, 0.f);
		
	bodyCinfo.m_rotation.setAxisAngle(hkVector4(0.0, 1.0, 0.0), 3.14f);
	hkpRigidBody* mBody;
	mBody = new hkpRigidBody(bodyCinfo);
	chassisShape->removeReference();

	
	
	if (isPlayer)
		mBody->setName("Player");
	else mBody->setName("Other");

	hkpVehicleInstance* mVehicle;
	mVehicle = new hkpVehicleInstance(mBody);

	//
	// All memory allocations are made here.
	//

	mVehicle->m_data = new hkpVehicleData;
	mVehicle->m_driverInput = new hkpVehicleDefaultAnalogDriverInput;
	mVehicle->m_steering = new hkpVehicleDefaultSteering;
	mVehicle->m_engine = new hkpVehicleDefaultEngine;
	mVehicle->m_transmission = new hkpVehicleDefaultTransmission;
	mVehicle->m_brake = new hkpVehicleDefaultBrake;
	mVehicle->m_suspension = new hkpVehicleDefaultSuspension;
	mVehicle->m_aerodynamics = new hkpVehicleDefaultAerodynamics;
	mVehicle->m_velocityDamper = new hkpVehicleDefaultVelocityDamper;

	// For illustrative purposes we use a custom hkpVehicleRayCastWheelCollide
	// which implements varying 'ground' friction in a very simple way.
	mVehicle->m_wheelCollide = new hkpVehicleRayCastWheelCollide;// FrictionMapVehicleRaycastWheelCollide;

	SetupVehicleData(*mVehicle->m_data);

	// initialise the tyremarks controller with 128 tyremark points.
	mVehicle->m_tyreMarks = new hkpTyremarksInfo(*mVehicle->m_data, 128);

	SetupComponent(*mVehicle->m_data, *static_cast< hkpVehicleDefaultAnalogDriverInput* >(mVehicle->m_driverInput));
	SetupComponent(*mVehicle->m_data, *static_cast< hkpVehicleDefaultSteering*>(mVehicle->m_steering));
	SetupComponent(*mVehicle->m_data, *static_cast< hkpVehicleDefaultEngine*>(mVehicle->m_engine));
	SetupComponent(*mVehicle->m_data, *static_cast< hkpVehicleDefaultTransmission*>(mVehicle->m_transmission));
	SetupComponent(*mVehicle->m_data, *static_cast< hkpVehicleDefaultBrake*>(mVehicle->m_brake));
	SetupComponent(*mVehicle->m_data, *static_cast< hkpVehicleDefaultSuspension*>(mVehicle->m_suspension));
	SetupComponent(*mVehicle->m_data, *static_cast< hkpVehicleDefaultAerodynamics*>(mVehicle->m_aerodynamics));
	SetupComponent(*mVehicle->m_data, *static_cast< hkpVehicleDefaultVelocityDamper*>(mVehicle->m_velocityDamper));

	SetupWheelCollide(*mVehicle, *static_cast< hkpVehicleRayCastWheelCollide*>(mVehicle->m_wheelCollide));

	SetupTyremarks(*mVehicle->m_data, *static_cast< hkpTyremarksInfo*>(mVehicle->m_tyreMarks));

	//cQuaternion turn90; turn90.FromAxisRotation( 0.f, 1.f, 0.f, nMath::PI_DIV_2 );
	//hkQuaternion hkTurn90 = hkQuaternion( turn90.x, turn90.y, turn90.z, turn90.w );
	//mVehicle->getChassis()->setRotation( hkTurn90 );

	//
	// Check that all components are present.
	//
	HK_ASSERT(0x0, mVehicle->m_data);
	HK_ASSERT(0x7708674a, mVehicle->m_driverInput);
	HK_ASSERT(0x5a324a2d, mVehicle->m_steering);
	HK_ASSERT(0x7bcb2aff, mVehicle->m_engine);
	HK_ASSERT(0x29bddb50, mVehicle->m_transmission);
	HK_ASSERT(0x2b0323a2, mVehicle->m_brake);
	HK_ASSERT(0x7a7ade23, mVehicle->m_suspension);
	HK_ASSERT(0x6ec4d0ed, mVehicle->m_aerodynamics);
	HK_ASSERT(0x67161206, mVehicle->m_wheelCollide);
	HK_ASSERT(0x295015f1, mVehicle->m_tyreMarks);

	//
	// Set up any variables that store cached data.
	//


	// Give driver input default values so that the vehicle (if this input is a default for non
	// player cars) will drive, even if it is in circles!

	// Accelerate.
	mVehicle->m_deviceStatus = new hkpVehicleDriverInputAnalogStatus;
	hkpVehicleDriverInputAnalogStatus* deviceStatus = (hkpVehicleDriverInputAnalogStatus*)mVehicle->m_deviceStatus;
	deviceStatus->m_positionY = 0.0f;

	// Turn.
	deviceStatus->m_positionX = 0.0f;

	// Defaults
	deviceStatus->m_handbrakeButtonPressed = false;
	deviceStatus->m_reverseButtonPressed = false;

	hkVector4 rotationAxis(0.0f, 0.0f, 1.0f);
	hkVector4 upAxis(0.0f, 1.0f, 0.0f);
	m_reorientAction = new hkpReorientAction(mVehicle->getRigidBody(), rotationAxis, upAxis, 2.0f, 0.1f);
	//hkpReorientAction carflip(mVehicle->getRigidBody(), hkVector4(1.0f, 0.0f, 0.0f), hkVector4(0.0f, 1.0f, 0.0f), 1.0f, 0.1f);

	//
	// Don't forget to call init! (This function is necessary to set up derived data)
	//
	mVehicle->init();
	
	//mVehicle->addToWorld(world);
	this->m_mapRigidBody[ID] = mBody;
	this->m_mapVehicle[ID] = mVehicle;

	hkpRigidBody* temp = this->m_mapRigidBody[ID];
	// Add the rigidBody to the world
	//world->addEntity(rigidBody);
	//this->m_pHavok_world->addEntity(rigidBody);

	//this->m_pHavok_world->addEntity(this->m_mapRigidBody[ID]);
	//this->m_mapRigidBody[ID]->removeReference();

	mVehicle->addToWorld(this->m_pHavok_world);
	this->m_pHavok_world->addAction(mVehicle);

	mBody->removeReference();

	mVehicle->getChassis()->activate();
}



void CHavokPhysics::CreateConstraint(std::wstring ID, std::wstring ID2, std::wstring ConstraintID)
{
	hkpRigidBodyCinfo bodyCinfo;
	this->m_mapRigidBody[ID]->getCinfo(bodyCinfo);

	//temp = bodyCinfo.m_shape->;
	hkpBoxShape *boxShape;
	hkVector4 boxHalfExtents;
	hkVector4 fullSize;
	
	switch(bodyCinfo.m_shape->m_type)
	{
	case hkcdShapeType::BOX:
		{
			boxShape = ( hkpBoxShape *)(bodyCinfo.m_shape);
			boxHalfExtents = boxShape->getHalfExtents();
			fullSize.setMul4(hkVector4(2,2,2,1),boxHalfExtents);
		}
		break;
	}

	//hkVector4 position( bodyCinfo.m_position );

	//hkVector4 position(fullSize(0), -fullSize(1) -0.1f,-fullSize(2));


	hkVector4 pivot;
	//pivot = position;
	//pivot = hkVector4(0,0,0,0);
	pivot = hkVector4( bodyCinfo.m_position );

	//pivot.setAdd4(position,boxHalfExtents);
	//pivot(0) -= fullSize(0);

	hkpBallAndSocketConstraintData* ballSocket = new hkpBallAndSocketConstraintData();
	ballSocket->setInWorldSpace(this->m_mapRigidBody[ID]->getTransform(),this->m_mapRigidBody[ID2]->getTransform(),pivot);
	this->m_mapConstraints[ConstraintID] = new hkpConstraintInstance(this->m_mapRigidBody[ID], this->m_mapRigidBody[ID2], ballSocket);
	this->m_pHavok_world->addConstraint(this->m_mapConstraints[ConstraintID]);
	
	ballSocket->removeReference();
	this->m_mapConstraints[ConstraintID]->removeReference();

	int breakpoint = 0;
}

void CHavokPhysics::CreateCollisionListener(std::wstring ObjectID, std::wstring ListenerID)
{
	this->m_mapListeners[ListenerID] = new MyEndOfStepListener();
	this->m_mapListeners[ListenerID]->m_totalScore = 0;
	this->m_mapRigidBody[ObjectID]->addContactListener(this->m_mapListeners[ListenerID]);
}

void CHavokPhysics::CreateConstrainedCollisionFilter(std::wstring ObjectID, std::wstring ObjectID2, std::wstring FilterID)
{
	this->m_mapConstraintFilters[FilterID] = new hkpConstraintCollisionFilter();
	this->m_mapConstraintFilters[FilterID]->disableCollisionsBetween(this->m_mapRigidBody[ObjectID],this->m_mapRigidBody[ObjectID2]);
}

void CHavokPhysics::SetForce(std::wstring ID, float time, CVector3f force )
{
	hkVector4 vec4Force;
	vec4Force.set(force.x,force.y,force.z,0);
	this->m_mapRigidBody[ID]->applyForce(time,vec4Force);
}

void CHavokPhysics::DeleteEntity(std::wstring ID)
{
	this->m_pHavok_world->removeEntity(this->m_mapRigidBody[ID]);
	this->m_mapRigidBody.erase(ID);
}

std::vector<std::wstring> CHavokPhysics::GetEntitiesOutsideBounds(float maxbounds)
{
	std::vector<std::wstring> IDs;
	for(std::map<std::wstring,hkpRigidBody*>::iterator it = this->m_mapRigidBody.begin(); it != this->m_mapRigidBody.end(); it++)
	{
		hkVector4 position = it->second->getPosition();
		if(position.getComponent(0) > maxbounds || position.getComponent(0) < (-1*maxbounds) ||
			position.getComponent(1) > maxbounds || position.getComponent(1) < (-1*maxbounds) ||
			position.getComponent(2) > maxbounds || position.getComponent(2) < (-1*maxbounds))
		{
			IDs.push_back(it->first);
		}
	}
	return IDs;
}

std::vector<CVec3fWithName> CHavokPhysics::getPositions()
{

	// std::map<std::wstring,hkpRigidBody*> m_mapRigidBody;
	//hkpRigidBody* pPigObject = this->m_mapRigidBody[L"Pig 1"];

	std::vector<CVec3fWithName> positions;

	for(std::map<std::wstring,hkpRigidBody*>::iterator it = this->m_mapRigidBody.begin(); it != this->m_mapRigidBody.end(); it++)
	{
		hkVector4 temp = it->second->getPosition();
		positions.push_back(CVec3fWithName(CVector3f(temp.getComponent(0),temp.getComponent(1),temp.getComponent(2)),it->first));
	}

	return positions;
}


std::map<std::wstring, PhysicsMailMan> CHavokPhysics::getMatrix()
{
	std::map<std::wstring, PhysicsMailMan>  physicsMailMen;

	for(std::map<std::wstring,hkpRigidBody*>::iterator it = this->m_mapRigidBody.begin(); it != this->m_mapRigidBody.end(); it++)
	{
		PhysicsMailMan  tempPhysicsMailMan;
		//matrix
		hkMatrix4 m;
		hkTransform transform;
		it->second->approxCurrentTransform(transform);
		m.set(transform);
		glm::mat4 matOut;
		memcpy(&matOut, &m, sizeof(float)* 16);
		tempPhysicsMailMan.matrix = matOut;
		//position
		hkVector4 temp = it->second->getPosition();
		tempPhysicsMailMan.position = CVector3f(temp.getComponent(0), temp.getComponent(1), temp.getComponent(2));

		hkQuaternion tempQuat = it->second->getRotation();
		tempPhysicsMailMan.orientation = glm::vec4(tempQuat.getComponent<1>(), tempQuat.getComponent<2>(), tempQuat.getComponent<3>(), tempQuat.getComponent<0>() );

		physicsMailMen[it->first] = tempPhysicsMailMan;
		//give each gameobject a glm matrix to store the rotation and position
	}

	return physicsMailMen;
}

bool CHavokPhysics::GetWheelPositionRotation(std::wstring ID, int wheelNumber, glm::vec3 &output_pos, glm::fquat &output_rot)
{
	if (wheelNumber < 0 || wheelNumber > 3)
	{
		return false;
	}

	hkVector4 pos;
	hkQuaternion rot;
	hkpVehicleInstance* mVehicle;
	mVehicle = this->m_mapVehicle[ID];
	mVehicle->getChassis()->activate();
	mVehicle->calcCurrentPositionAndRotation(mVehicle->getRigidBody(), mVehicle->m_suspension, wheelNumber, pos, rot);

	output_pos.x = pos.getComponent(0);
	output_pos.y = pos.getComponent(1);
	output_pos.z = pos.getComponent(2);

	output_rot.x = rot.getComponent<0>();
	output_rot.y = rot.getComponent<1>();
	output_rot.z = rot.getComponent<2>();
	output_rot.w = rot.getComponent<3>();

	return true;
}

unsigned int CHavokPhysics::getCurrentCheckPoint()
{
	return this->currentCheckPoint;
}

/* Implement later
std::vector<CQuat> CHavokPhysics::getOrientations()
{
	std::vector<CQuat> orientations;

	for(std::map<std::wstring,hkpRigidBody*>::iterator it = this->m_mapRigidBody.begin(); it != this->m_mapRigidBody.end(); it++)
	{
		hkQuaternion tempQuat = it->second->getRotation();
		orientations.push_back(tempQuat);
	}

	return orientations;
}
*/

void CHavokPhysics::respawnObject(std::wstring ID)
{
	this->m_mapRigidBody[ID]->setRotation(hkQuaternion(0.0f, 1.0f, 0.0f, 0.0f));
	this->m_mapRigidBody[ID]->setPosition(m_respawnPoint);
	this->m_mapRigidBody[ID]->setLinearVelocity(hkVector4(0.0f, 0.0f, 0.0f, 0.0f));
}

bool CHavokPhysics::isPlayerInsideCheckPoint()
{
	return this->m_resetTimer;
}

void CHavokPhysics::resetTimerToFalse(){
	this->m_resetTimer = false;
}


//////////////////////////////////////////////////////////////////////////
/*
  _________ __                   __      __            .__       .___
 /   _____//  |_  ____ ______   /  \    /  \___________|  |    __| _/
 \_____  \\   __\/ __ \\____ \  \   \/\/   /  _ \_  __ \  |   / __ |
 /        \|  | \  ___/|  |_> >  \        (  <_> )  | \/  |__/ /_/ |
/_______  /|__|  \___  >   __/    \__/\  / \____/|__|  |____/\____ |
        \/           \/|__|            \/                         \/
*/////////////////////////////////////////////////////////////////////////

void CHavokPhysics::stepWorld( float timeStep )
{
	//m_pHavok_world->stepMultithreaded( m_pJobQueue, m_pThreadPool, timeStep);
	this->m_pHavok_world->stepDeltaTime(timeStep);
	//std::cout << "Running Havok" << std::endl; std::cout.flush();
	if (this->m_bUsingVisualDebugger)
	{
		this->m_pVisualDebugger->step();
	}

	
	//grabs outofbound objects and returns them to center
	std::vector<std::wstring> vec_outsideObjects;
	vec_outsideObjects = GetEntitiesOutsideBounds(600.0f); 
	for (int i = 0; i < vec_outsideObjects.size(); i++)
	{
		respawnObject(vec_outsideObjects[i]);
	}

	std::wstringstream ss;
	ss << this->currentCheckPoint;
	if (this->m_PhantomListener->isPlayerID)
	{
		this->m_resetTimer = true;
		//this->m_PhantomListener->isPlayerID = false;
		m_respawnPoint = this->m_Phantom->getAabb().m_max;
		if (this->currentCheckPoint < m_vecCheckPoints.size()-1){
			this->m_pHavok_world->removePhantom(this->m_Phantom);
			this->currentCheckPoint++;
			makeNextCheckPoint();
			std::cout << "Player inside checkpoint " << this->currentCheckPoint << std::endl; std::cout.flush();
		}
		else{
			std::cout << "Player has completed the circuit " << this->currentCheckPoint << std::endl; std::cout.flush();
		}
	}

	return;
}

std::vector<std::wstring> CHavokPhysics::checkCollisionListeners( void )
{
	std::vector<std::wstring> objectsColliding;
	if(g_collidingEntities.size() > 0)
	{
		for(std::map<std::wstring,hkpRigidBody*>::iterator it = this->m_mapRigidBody.begin(); it != this->m_mapRigidBody.end(); it++)
		{
			if(g_collidingEntities[0] == it->second || g_collidingEntities[1] == it->second)
			{
				objectsColliding.push_back(it->first);
			}
		}
		g_collidingEntities.clear();
	}
	return objectsColliding;
}

void CHavokPhysics::shutDown(void)
{
	// Release the visual debugger
	//visualDebugger->removeReference();
	//this->m_pVisualDebugger->removeReference();

	// No longer need the ref of rigidBody - as the world now owns it
	//rigidBody->removeReference();
	for(std::map<std::wstring,hkpRigidBody*>::iterator it = this->m_mapRigidBody.begin(); it != this->m_mapRigidBody.end(); it++)
	{
		//it->second->removeReference();
	}

	// Using the visual Debugger?
	if ( this->m_bUsingVisualDebugger )
	{
		this->m_pVisualDebugger->shutdown();
		this->m_pVisualDebugger->removeReference();
		this->m_pPhysicsContext->removeReference();
	}

	// Release the reference on the world
	//world->removeReference();
//	this->m_pHavok_world->removeReference();

	// Contexts are not reference counted at the base class level by the VDB as
	// they are just interfaces really. So only delete the context after you have
	// finished using the VDB.
	//g_context->removeReference();

//	hkBaseSystem::quit();
//	hkMemoryInitUtil::quit();
}

void CHavokPhysics::SetTurningState(float state, std::wstring ID)
{
	hkpVehicleInstance* mVehicle;
	mVehicle = this->m_mapVehicle[ID];
	mVehicle->getChassis()->activate();
	hkpVehicleDriverInputAnalogStatus*	deviceStatus = (hkpVehicleDriverInputAnalogStatus*)mVehicle->m_deviceStatus;

	float xPosition = state;
	if (state > 1.f)
	{
		xPosition = 1.f;
	}
	else if (state < -1.f)
	{
		xPosition = -1.f;
	}

	deviceStatus->m_positionX = xPosition;
}

void CHavokPhysics::SetAccelerationState(float state, std::wstring ID)
{
	hkpVehicleInstance* mVehicle;
	mVehicle = this->m_mapVehicle[ID];
	mVehicle->getChassis()->activate();
	hkpVehicleDriverInputAnalogStatus*	deviceStatus = (hkpVehicleDriverInputAnalogStatus*)mVehicle->m_deviceStatus;

	float yPosition = state;
	if (state > 1.f)
	{
		yPosition = 1.f;
	}
	else if (state < -1.f)
	{
		yPosition = -1.f;
	}

	deviceStatus->m_positionY = state;
}

void CHavokPhysics::AddFlipVehicle(bool state, std::wstring ID){
	//hkpAction* action = mVehicle->getRigidBody()->getAction(1);
	
	if (m_reorientAction->getWorld() == HK_NULL)
	{
		hkpVehicleInstance* mVehicle;
		mVehicle = this->m_mapVehicle[ID];
		mVehicle->getChassis()->activate();

		

		/*hkQuaternion tempQuat;
		tempQuat * hkVector4(0.0f, 1.0f, 0.0f);
		if (mVehicle->getRigidBody()->getRotation() != hkVector4(0.0f, 1.0f, 0.0f)*/

		this->m_pHavok_world->addAction(m_reorientAction);
	}
	else if (m_reorientAction->getWorld() != HK_NULL)
	{
		hkpVehicleInstance* mVehicle;
		mVehicle = this->m_mapVehicle[ID];
		mVehicle->getChassis()->activate();
		this->m_pHavok_world->removeAction(m_reorientAction);
	}
	return;
}


void CHavokPhysics::SetBrakingState(bool state, std::wstring ID)
{
	hkpVehicleInstance* mVehicle;
	mVehicle = this->m_mapVehicle[ID];
	mVehicle->getChassis()->activate();
	hkpVehicleDriverInputAnalogStatus*	deviceStatus = (hkpVehicleDriverInputAnalogStatus*)mVehicle->m_deviceStatus;
	deviceStatus->m_handbrakeButtonPressed = state;
}

void CHavokPhysics::SetupVehicleData(hkpVehicleData& data)
{
	//
	// The vehicleData contains information about the chassis.
	//

	// The coordinates of the chassis system, used for steering the vehicle.
	//										up					forward				right


	data.m_chassisOrientation.setCols(hkVector4(0, 1, 0), hkVector4(0, 0, -1), hkVector4(1, 0, 0));

	data.m_frictionEqualizer = 0.5f;


	// Inertia tensor for each axis is calculated by using : 
	// (1 / chassis_mass) * (torque(axis)Factor / chassisUnitInertia)
	data.m_torqueRollFactor = 0.625f;
	data.m_torquePitchFactor = 0.5f;
	data.m_torqueYawFactor = 0.35f;

	data.m_chassisUnitInertiaYaw = 1.0f;
	data.m_chassisUnitInertiaRoll = 1.0f;
	data.m_chassisUnitInertiaPitch = 1.0f;

	// Adds or removes torque around the yaw axis 
	// based on the current steering angle.  This will 
	// affect steering.
	data.m_extraTorqueFactor = -0.5f;
	data.m_maxVelocityForPositionalFriction = 0.0f;

	//
	// Wheel specifications
	//
	data.m_numWheels = 4;

	data.m_wheelParams.setSize(data.m_numWheels);

	data.m_wheelParams[0].m_axle = 1;
	data.m_wheelParams[1].m_axle = 0;
	data.m_wheelParams[2].m_axle = 1;
	data.m_wheelParams[3].m_axle = 0;

	data.m_wheelParams[0].m_friction = 1.5f;
	data.m_wheelParams[1].m_friction = 1.5f;
	data.m_wheelParams[2].m_friction = 1.5f;
	data.m_wheelParams[3].m_friction = 1.5f;

	data.m_wheelParams[0].m_slipAngle = 0.0f;
	data.m_wheelParams[1].m_slipAngle = 0.0f;
	data.m_wheelParams[2].m_slipAngle = 0.0f;
	data.m_wheelParams[3].m_slipAngle = 0.0f;

	for (int i = 0; i < data.m_numWheels; i++)
	{
		// This value is also used to calculate the m_primaryTransmissionRatio.
		data.m_wheelParams[i].m_radius = 0.4f;
		data.m_wheelParams[i].m_width = 0.2f;
		data.m_wheelParams[i].m_mass = 10.0f;

		data.m_wheelParams[i].m_viscosityFriction = 0.25f;
		data.m_wheelParams[i].m_maxFriction = 2.0f * data.m_wheelParams[i].m_friction;
		data.m_wheelParams[i].m_forceFeedbackMultiplier = 0.1f;
		data.m_wheelParams[i].m_maxContactBodyAcceleration = hkReal(data.m_gravity.length3()) * 2;
	}
}


void CHavokPhysics::SetupComponent(const hkpVehicleData& data, hkpVehicleDefaultAnalogDriverInput& driverInput)
{
	// We also use an analog "driver input" class to help converting user input to vehicle behavior.

	driverInput.m_slopeChangePointX = 0.8f;
	driverInput.m_initialSlope = 0.7f;
	driverInput.m_deadZone = 0.0f;
	driverInput.m_autoReverse = true;
}

void CHavokPhysics::SetupComponent(const hkpVehicleData& data, hkpVehicleDefaultSteering& steering)
{
	steering.m_doesWheelSteer.setSize(data.m_numWheels);

	// degrees
	steering.m_maxSteeringAngle = 75 * (HK_REAL_PI / 180);

	// [mph/h] The steering angle decreases linearly 
	// based on your overall max speed of the vehicle. 
	steering.m_maxSpeedFullSteeringAngle = 70.0f * (1.605f / 3.6f);
	steering.m_doesWheelSteer[0] = true;
	steering.m_doesWheelSteer[1] = false;
	steering.m_doesWheelSteer[2] = true;
	steering.m_doesWheelSteer[3] = false;
}

void CHavokPhysics::SetupComponent(const hkpVehicleData& data, hkpVehicleDefaultEngine& engine)
{
	engine.m_maxTorque = 500.0f;

	engine.m_minRPM = 1000.0f;
	engine.m_optRPM = 5500.0f;

	// This value is also used to calculate the m_primaryTransmissionRatio.
	engine.m_maxRPM = 7500.0f;

	engine.m_torqueFactorAtMinRPM = 0.8f;
	engine.m_torqueFactorAtMaxRPM = 0.8f;

	engine.m_resistanceFactorAtMinRPM = 0.05f;
	engine.m_resistanceFactorAtOptRPM = 0.1f;
	engine.m_resistanceFactorAtMaxRPM = 0.3f;
}

void CHavokPhysics::SetupComponent(const hkpVehicleData& data, hkpVehicleDefaultTransmission& transmission)
{
	int numGears = 4;

	transmission.m_gearsRatio.setSize(numGears);
	transmission.m_wheelsTorqueRatio.setSize(data.m_numWheels);

	transmission.m_downshiftRPM = 3500.0f;
	transmission.m_upshiftRPM = 6500.0f;

	transmission.m_clutchDelayTime = 0.0f;
	transmission.m_reverseGearRatio = 1.0f;
	transmission.m_gearsRatio[0] = 2.0f;
	transmission.m_gearsRatio[1] = 1.5f;
	transmission.m_gearsRatio[2] = 1.0f;
	transmission.m_gearsRatio[3] = 0.75f;
	transmission.m_wheelsTorqueRatio[0] = 0.15f;
	transmission.m_wheelsTorqueRatio[1] = 0.35f;
	transmission.m_wheelsTorqueRatio[2] = 0.15f;
	transmission.m_wheelsTorqueRatio[3] = 0.35f;

	const hkReal vehicleTopSpeed = 130.0f;
	const hkReal wheelRadius = 0.4f;
	const hkReal maxEngineRpm = 7500.0f;
	transmission.m_primaryTransmissionRatio = hkpVehicleDefaultTransmission::calculatePrimaryTransmissionRatio(vehicleTopSpeed,
		wheelRadius,
		maxEngineRpm,
		transmission.m_gearsRatio[numGears - 1]);
}

void CHavokPhysics::SetupComponent(const hkpVehicleData& data, hkpVehicleDefaultBrake& brake)
{
	brake.m_wheelBrakingProperties.setSize(data.m_numWheels);

	const float bt = 1500.0f;
	brake.m_wheelBrakingProperties[0].m_maxBreakingTorque = bt;
	brake.m_wheelBrakingProperties[1].m_maxBreakingTorque = bt;
	brake.m_wheelBrakingProperties[2].m_maxBreakingTorque = bt;
	brake.m_wheelBrakingProperties[3].m_maxBreakingTorque = bt;

	// Handbrake is attached to rear wheels only.
	brake.m_wheelBrakingProperties[0].m_isConnectedToHandbrake = false;
	brake.m_wheelBrakingProperties[1].m_isConnectedToHandbrake = true;
	brake.m_wheelBrakingProperties[2].m_isConnectedToHandbrake = false;
	brake.m_wheelBrakingProperties[3].m_isConnectedToHandbrake = true;
	brake.m_wheelBrakingProperties[0].m_minPedalInputToBlock = 0.9f;
	brake.m_wheelBrakingProperties[1].m_minPedalInputToBlock = 0.9f;
	brake.m_wheelBrakingProperties[2].m_minPedalInputToBlock = 0.9f;
	brake.m_wheelBrakingProperties[3].m_minPedalInputToBlock = 0.9f;
	brake.m_wheelsMinTimeToBlock = 1000.0f;
}

void CHavokPhysics::SetupComponent(const hkpVehicleData& data, hkpVehicleDefaultSuspension& suspension)
{
	suspension.m_wheelParams.setSize(data.m_numWheels);
	suspension.m_wheelSpringParams.setSize(data.m_numWheels);

	suspension.m_wheelParams[0].m_length = 0.5f;
	suspension.m_wheelParams[1].m_length = 0.5f;
	suspension.m_wheelParams[2].m_length = 0.5f;
	suspension.m_wheelParams[3].m_length = 0.5f;

	const float str = 50.0f;
	suspension.m_wheelSpringParams[0].m_strength = str;
	suspension.m_wheelSpringParams[1].m_strength = str;
	suspension.m_wheelSpringParams[2].m_strength = str;
	suspension.m_wheelSpringParams[3].m_strength = str;

	const float wd = 3.0f;
	suspension.m_wheelSpringParams[0].m_dampingCompression = wd;
	suspension.m_wheelSpringParams[1].m_dampingCompression = wd;
	suspension.m_wheelSpringParams[2].m_dampingCompression = wd;
	suspension.m_wheelSpringParams[3].m_dampingCompression = wd;

	suspension.m_wheelSpringParams[0].m_dampingRelaxation = wd;
	suspension.m_wheelSpringParams[1].m_dampingRelaxation = wd;
	suspension.m_wheelSpringParams[2].m_dampingRelaxation = wd;
	suspension.m_wheelSpringParams[3].m_dampingRelaxation = wd;

	//
	// NB: The hardpoints MUST be positioned INSIDE the chassis.
	//
	{
		const hkReal hardPointFrontX = 0.73f;// 1.3f;
		const hkReal hardPointBackX = -0.73f;
		const hkReal hardPointY = -0.05f;
		const hkReal hardBackPointZ = 1.15f;
		const hkReal hardFrontPointZ = 1.75f;

		suspension.m_wheelParams[0].m_hardpointChassisSpace.set(hardPointFrontX, hardPointY, -hardFrontPointZ);
		suspension.m_wheelParams[1].m_hardpointChassisSpace.set(hardPointFrontX, hardPointY, hardBackPointZ);
		suspension.m_wheelParams[2].m_hardpointChassisSpace.set(hardPointBackX, hardPointY, -hardFrontPointZ);
		suspension.m_wheelParams[3].m_hardpointChassisSpace.set(hardPointBackX, hardPointY, hardBackPointZ);

	}

	const hkVector4 downDirection(0.0f, -1.0f, 0.0f);
	suspension.m_wheelParams[0].m_directionChassisSpace = downDirection;
	suspension.m_wheelParams[1].m_directionChassisSpace = downDirection;
	suspension.m_wheelParams[2].m_directionChassisSpace = downDirection;
	suspension.m_wheelParams[3].m_directionChassisSpace = downDirection;
}

void CHavokPhysics::SetupComponent(const hkpVehicleData& data, hkpVehicleDefaultAerodynamics& aerodynamics)
{
	aerodynamics.m_airDensity = 1.3f;
	// In m^2.
	aerodynamics.m_frontalArea = 1.0f;

	aerodynamics.m_dragCoefficient = 0.7f;
	aerodynamics.m_liftCoefficient = -0.3f;

	// Extra gavity applies in world space (independent of m_chassisCoordinateSystem).
	aerodynamics.m_extraGravityws.set(0.0f, -5.0f, 0.0f);
}

void CHavokPhysics::SetupComponent(const hkpVehicleData& data, hkpVehicleDefaultVelocityDamper& velocityDamper)
{
	// Caution: setting negative damping values will add energy to system. 
	// Setting the value to 0 will not affect the angular velocity. 

	// Damping the change of the chassis angular velocity when below m_collisionThreshold. 
	// This will affect turning radius and steering.
	velocityDamper.m_normalSpinDamping = 0.0f;

	// Positive numbers dampen the rotation of the chassis and 
	// reduce the reaction of the chassis in a collision. 
	velocityDamper.m_collisionSpinDamping = 4.0f;

	// The threshold in m/s at which the algorithm switches from 
	// using the normalSpinDamping to the collisionSpinDamping. 	
	velocityDamper.m_collisionThreshold = 1.0f;
}

void CHavokPhysics::SetupWheelCollide(const hkpVehicleInstance& vehicle, hkpVehicleRayCastWheelCollide& wheelCollide)
{
	// Set the wheels to have the same collision filter info as the chassis.
	wheelCollide.m_wheelCollisionFilterInfo = vehicle.getChassis()->getCollisionFilterInfo();
}

void CHavokPhysics::SetupTyremarks(const hkpVehicleData& data, hkpTyremarksInfo& tyreMarks)
{
	tyreMarks.m_minTyremarkEnergy = 100.0f;
	tyreMarks.m_maxTyremarkEnergy = 1000.0f;
}