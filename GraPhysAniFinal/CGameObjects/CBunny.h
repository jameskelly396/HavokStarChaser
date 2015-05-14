#ifndef _CBunny_HG_
#define _CBunny_HG_

#include "CGameObject.h"

class CBunny : public CGameObject
{
public: 
	CBunny();
	virtual ~CBunny();

	// Taken from the CGameObject
	virtual void Update( float deltaTime );
	virtual void receiveMessage( CMessage theMessage, unsigned int senderID );


private:
	// This is taken from the message bevaviour we need...
	float m_followClosestDistance;
	float m_followSpeed;
	CVector3f m_closestObject;
	unsigned int m_ID_of_ObjectToFollow;

	enum enumState
	{
		IS_DOING_NOTHING,		// = 0
		IS_FOLLOWING_CLOSEST_OBJECT,
		IS_FOLLOWING_SPECIFIC_OBJECT,
		UNKNOWN
	};
	enumState m_CurrentState;

	// Moved the "follow" code here
	// Moves this object towards the target by "increment" amount
	void m_followAtSpeed( CVector3f target, float increment );
};

#endif
