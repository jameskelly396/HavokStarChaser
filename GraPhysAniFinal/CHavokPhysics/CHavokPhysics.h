#ifndef _CHavokPhysics_HG_
#define _CHavokPhysics_HG_

// This is the actual class that will interface to the 
//	Havok middleware...

#include "IPhysics.h"


// All this stuff was from the Havok demo

// NOTE: To make Visual Studio happy, you have to tell it where
//	all the carp is. 
// You could: 
// a) update each header like this:
//	#include "C:\hk2011_3_0_r1\Source\Common\Base\hkBase.h"
// b) change the "Additional Include Directories" to have "C:\hk2011_3_0_r1\Source"
//    which is the root of where all these includes are...
// Math and base includes
#include <Common/Base/hkBase.h>
#include <Common/Visualize/hkVisualDebugger.h>

#include <Physics/Constraint/Data/hkpConstraintData.h>

#include <Physics2012/Dynamics/World/hkpWorld.h>	
#include <Physics2012/Dynamics/Entity/hkpRigidBody.h>
#include <Physics2012/Dynamics/Collide/ContactListener/Util/hkpEndOfStepCallbackUtil.h>
#include <Physics2012/Dynamics/Collide/Filter/Constraint/hkpConstraintCollisionFilter.h>

// ADDED for Visual Debugger support
#include <Physics2012/Utilities/VisualDebugger/hkpPhysicsContext.h>



//Vehicle
#include <Physics2012/Vehicle/hkpVehicleInstance.h>
#include <Physics2012/Vehicle/AeroDynamics/Default/hkpVehicleDefaultAerodynamics.h>
#include <Physics2012/Vehicle/DriverInput/hkpVehicleDriverInput.h>
#include <Physics2012/Vehicle/DriverInput/Default/hkpVehicleDefaultAnalogDriverInput.h>
#include <Physics2012/Vehicle/Brake/Default/hkpVehicleDefaultBrake.h>
#include <Physics2012/Vehicle/Engine/Default/hkpVehicleDefaultEngine.h>
#include <Physics2012/Vehicle/VelocityDamper/Default/hkpVehicleDefaultVelocityDamper.h>
#include <Physics2012/Vehicle/Steering/Default/hkpVehicleDefaultSteering.h>
#include <Physics2012/Vehicle/Suspension/Default/hkpVehicleDefaultSuspension.h>
#include <Physics2012/Vehicle/Transmission/Default/hkpVehicleDefaultTransmission.h>
#include <Physics2012/Vehicle/WheelCollide/RayCast/hkpVehicleRayCastWheelCollide.h>
#include <Physics2012/Vehicle/TyreMarks/hkpTyremarksInfo.h>

#include <Physics2012\Utilities\Actions\Reorient\hkpReorientAction.h>

//Camera
#include <Physics2012/Vehicle/Camera/hkp1dAngularFollowCam.h>
#include <Physics2012/Vehicle/Camera/hkp1dAngularFollowCamCinfo.h>

//Contact Listeners
#include <Physics2012/Dynamics/Collide/ContactListener/hkpContactListener.h>
#include <Physics2012/Dynamics/Collide/ContactListener/hkpContactPointEvent.h>
#include <Physics2012/Dynamics/Collide/ContactListener/hkpCollisionEvent.h>
#include <Physics2012/Dynamics/Phantom/hkpAabbPhantom.h>
#include <Physics2012/Dynamics/Phantom/hkpPhantomOverlapListener.h>

//Particles
//#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysics2012Demo.h>
//#include <Demos/DemoCommon/Utilities/Particles/Utilities/ContinuousEmissionUtil.h>
//#include <Demos/DemoCommon/Utilities/Particles/Utilities/ChimneyEmissionUtil.h>
//#include <Demos/DemoCommon/Utilities/Particles/Utilities/ParticleUtilsPhysics.h>

//class HkgSmokeParticleSystem;
//class HkgDebrisParticleSystem;
//class HkgSparksParticleSystem;
//class HkgDustParticleSystem;
//class HkgExplosionParticleSystem;
//class ScrapeParticleListener;

#include <map>
#include <iostream>		// Added

class MyEndOfStepListener : public hkReferencedObject, public hkpContactListener
{
public:
	HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO );

	void collisionAddedCallback( const hkpCollisionEvent& event );

	void collisionRemovedCallback( const hkpCollisionEvent& event );

	void contactPointCallback( const hkpContactPointEvent& event );

	unsigned int		m_totalScore;
};

class CPhantomListener : public hkReferencedObject, public hkpPhantomOverlapListener
{
public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);
	bool isPlayerID;
	unsigned int playerID;
	// collision begins
	virtual void  collidableAddedCallback(const hkpCollidableAddedEvent &event);
	// collision ends
	virtual void  collidableRemovedCallback(const hkpCollidableRemovedEvent &event);
};

class CHavokPhysics : public IPhysics
{
public:
	CHavokPhysics();
	virtual ~CHavokPhysics();

	void Init( const std::vector< CNameValuePair2 > &vecParams );
	std::wstring GetLastErrorString( void );
	void DeleteEntity(std::wstring ID);
	std::vector<std::wstring> GetEntitiesOutsideBounds(float maxbounds);
	void CreateRigidBody(std::wstring ID, CVector3f box, float mass, float friction, CVector3f position, CVector4f Qrotation, bool moveable);
	void CreateRigidBody(std::wstring ID, float radius, float mass, CVector3f position, CVector4f Qrotation, bool moveable, bool isStar);
	void CreateConstraint(std::wstring ID, std::wstring ID2, std::wstring ConstraintID);
	void CreateCollisionListener(std::wstring ObjectID, std::wstring ListenerID);
	void CreateConstrainedCollisionFilter(std::wstring ObjectID, std::wstring ObjectID2, std::wstring FilterID);
	void SetForce(std::wstring ID, float time, CVector3f force );
	//
	std::vector<CVec3fWithName> getPositions( void );
	std::map<std::wstring, PhysicsMailMan> getMatrix(void);
	bool GetWheelPositionRotation(std::wstring ID, int wheelNumber, glm::vec3 &output_pos, glm::fquat &output_rot);
	void respawnObject(std::wstring ID);
	unsigned int getCurrentCheckPoint();
	bool isPlayerInsideCheckPoint();
	void resetTimerToFalse();
	void stepWorld( float timeStep );
	std::vector<std::wstring> checkCollisionListeners( void );
	void shutDown(void);

	void InitMemory();
	void InitPhysicalWorld();
	void InitVDB();

	void BuildVehicle(std::wstring ID, CVector3f boxSize, float mass, CVector3f position, CVector4f Qrotation, bool moveable, bool isPlayer);
	void SetTurningState(float state, std::wstring ID);
	void SetBrakingState(bool state, std::wstring ID);
	void SetAccelerationState(float state, std::wstring ID);
	void AddFlipVehicle(bool state, std::wstring ID);

	void CreatePhantomCheckPoint(std::wstring ID, CVector3f min, CVector3f max);
	void SetCheckPoints(std::vector<CVector3f> vecCheckPointsInfo);
	void makeNextCheckPoint();
	unsigned int GetFinalScore();

protected:
	//Vehicle stuff
	virtual void SetupVehicleData(hkpVehicleData& data);
	virtual void SetupComponent(const hkpVehicleData& data, hkpVehicleDefaultAnalogDriverInput& driverInput);
	virtual void SetupComponent(const hkpVehicleData& data, hkpVehicleDefaultSteering& steering);
	virtual void SetupComponent(const hkpVehicleData& data, hkpVehicleDefaultEngine& engine);
	virtual void SetupComponent(const hkpVehicleData& data, hkpVehicleDefaultTransmission& transmission);
	virtual void SetupComponent(const hkpVehicleData& data, hkpVehicleDefaultBrake& brake);
	virtual void SetupComponent(const hkpVehicleData& data, hkpVehicleDefaultSuspension& suspension);
	virtual void SetupComponent(const hkpVehicleData& data, hkpVehicleDefaultAerodynamics& aerodynamics);
	virtual void SetupComponent(const hkpVehicleData& data, hkpVehicleDefaultVelocityDamper& velocityDamper);
	virtual void SetupWheelCollide(const hkpVehicleInstance& vehicle, hkpVehicleRayCastWheelCollide& wheelCollide);
	virtual void SetupTyremarks(const hkpVehicleData& data, hkpTyremarksInfo& tyremarkscontroller);

private:
	hkJobQueue*			m_pJobQueue;
	hkJobThreadPool*	m_pThreadPool;
	hkMemoryRouter*		m_pHavok_memoryRouter;
	hkpWorld*			m_pHavok_world;
	hkpWorldCinfo		m_pWorldInfo; // Info about global simulation parameters.
	hkVisualDebugger*	m_pVisualDebugger;
	hkpPhysicsContext*	m_pPhysicsContext;		// Used by the visual debugger
	bool				m_bUsingVisualDebugger;
	unsigned int		currentCheckPoint;
	hkpReorientAction*  m_reorientAction;
	hkVector4			m_respawnPoint;
	bool				m_resetTimer;
	unsigned int		m_totalScore;
	
	//
	std::map<std::wstring, hkpRigidBody*> m_mapRigidBody;
	std::map<std::wstring, hkpConstraintInstance*> m_mapConstraints;
	std::map<std::wstring, MyEndOfStepListener*> m_mapListeners;
	std::map<std::wstring, hkpConstraintCollisionFilter*> m_mapConstraintFilters;
	std::map<std::wstring, hkpVehicleInstance*> m_mapVehicle;
	std::map<std::wstring, hkpAabbPhantom*>	m_mapPhantoms;
	std::map<std::wstring, CPhantomListener*> m_mapPhantomListeners;

	std::vector< CNameValuePair2 >::const_iterator m_findNameValuePair(std::wstring nameToFind, const std::vector< CNameValuePair2 > &vecNameValuePair);
	std::vector<CVector3f> m_vecCheckPoints;
	hkpAabbPhantom*		m_Phantom;
	CPhantomListener*	m_PhantomListener;
};

#endif