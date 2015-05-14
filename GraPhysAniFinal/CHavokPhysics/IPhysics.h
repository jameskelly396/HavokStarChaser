#ifndef _IPhysics_HG_
#define _IPhysics_HG_

// This will be the interface to the Havok Physics Motron thingy, 
//	and also any other physics thingy we may want to create. 
// This will allow us to separate all the "havok" stuff from 
//	our actual directX project. 
// (The $10 word for that is 'decoupled')

#include <string>
#include <vector>
#include <map>


#include "../PhysicsMailMan.h"
#include "../CCommonTypes/CVector4f.h"

class CVec3fWithName
{
public:
	CVector3f xyz;
	std::wstring ID;

	CVec3fWithName( CVector3f a, std::wstring b ) : xyz(a), ID(b){};
};

class CNameValuePair2
{
public:
	CNameValuePair2();
	CNameValuePair2( std::wstring initName, float initfValue );
	CNameValuePair2( std::wstring initName, bool initbValue );
	CNameValuePair2( std::wstring initName, std::wstring initsValue );
	std::wstring name;
	float fValue;
	bool bValue;
	std::wstring sValue;
	inline void Clear(void);
};


class IPhysics
{
public:
	virtual ~IPhysics() {};	// Ooops. Michael forgot to add this.
	virtual void Init( const std::vector< CNameValuePair2 > &vecParams ) = 0;
	virtual void CreateRigidBody(std::wstring ID, CVector3f box, float mass, float friction, CVector3f position, CVector4f Qrotation, bool moveable) = 0;
	virtual void CreateRigidBody(std::wstring ID, float radius, float mass, CVector3f position, CVector4f Qrotation, bool moveable, bool isStar) = 0;
	virtual void BuildVehicle(std::wstring ID, CVector3f boxSize, float mass, CVector3f position, CVector4f Qrotation, bool moveable, bool isPlayer) = 0;
	virtual void CreatePhantomCheckPoint(std::wstring ID, CVector3f min, CVector3f max) = 0;

	virtual void CreateConstraint(std::wstring ObjectID, std::wstring ObjectID2, std::wstring ConstraintID) = 0;
	virtual void CreateCollisionListener(std::wstring ObjectID, std::wstring ListenerID) = 0;
	virtual void CreateConstrainedCollisionFilter(std::wstring ObjectID, std::wstring ObjectID2, std::wstring FilterID) = 0;
	virtual void DeleteEntity(std::wstring ID) = 0;
	virtual std::vector<std::wstring> GetEntitiesOutsideBounds(float maxbounds) = 0;
	virtual void SetForce(std::wstring ID, float time, CVector3f force ) = 0;
	virtual std::wstring GetLastErrorString( void ) = 0;
	virtual void SetTurningState(float state, std::wstring ID) = 0;
	virtual void SetBrakingState(bool state, std::wstring ID) = 0;
	virtual void SetAccelerationState(float state, std::wstring ID) = 0;
	virtual void SetCheckPoints(std::vector<CVector3f> vecCheckPointsInfo) = 0;
	virtual void AddFlipVehicle(bool state, std::wstring ID) = 0;
	virtual void respawnObject(std::wstring ID) = 0;
	virtual bool isPlayerInsideCheckPoint() = 0;
	virtual void resetTimerToFalse() = 0;
	virtual unsigned int GetFinalScore() = 0;
	//
	virtual std::vector<CVec3fWithName> getPositions( void ) = 0;
	virtual std::map<std::wstring, PhysicsMailMan> getMatrix(void) = 0;
	virtual bool GetWheelPositionRotation(std::wstring ID, int wheelNumber, glm::vec3 &output_pos, glm::fquat &output_rot) = 0;
	virtual unsigned int getCurrentCheckPoint() = 0;
	virtual void stepWorld( float timeStep ) = 0;
	virtual std::vector<std::wstring> checkCollisionListeners( void ) = 0;
	virtual void shutDown(void) = 0;
};

#endif