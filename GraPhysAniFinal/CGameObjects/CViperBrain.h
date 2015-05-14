#ifndef _CViperBrain_HG_
#define _CViperBrain_HG_

#include "IBrain.h"

class CViperBrain : public IBrain
{
public:
	CViperBrain();
	virtual ~CViperBrain();
	virtual void setMediator( IMediator* pTheMediator );
	virtual void setGameObject( CGameObject* pGameObject );
	
	// These messages are passed in from the object
	virtual void receiveMessage( CMessage theMessage, unsigned int senderID );


	virtual void Update( float deltaTime );
private:
	IMediator*		m_pMediator;
	CGameObject*	m_pGameObject;

	// How about:
	// 1. Pick a waypoint in the world
	// 2. Accelerate towards waypoint, to may speed
	// 3. Turn towards (dot product)

	enum enumObjectState
	{
		USER_CONTROLLED = 60000,
		ON_PATROL
	};

	enumObjectState m_BrainState;
	CVector3f m_nextWaypoint;
	glm::fquat m_idealOrientation;
	float m_maxSpeed;
	float m_maxTurnSpeed;
	float m_maxPatrolDistance;
	float m_minDistanceToWaypoint;
	float m_maxThrust;
	bool m_bUseQuaternionLookatForOrientation;

	float CViperBrain::m_getRand(float lowRange, float highRange);
};

#endif

