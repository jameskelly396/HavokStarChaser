#ifndef _IBrain_HG_
#define _IBrain_HG_

#include "../IMediator.h"
//#include "../CGameObjects/CGameObject.h"

class CGameObject;	// forward declaration (so we don't have to have an IGameObject)

class IBrain
{
public:
	virtual ~IBrain() {};

	virtual void setMediator( IMediator* pTheMediator ) = 0;
	virtual void setGameObject( CGameObject* pGameObject ) = 0;

	// These messages are passed in from the object
	virtual void receiveMessage( CMessage theMessage, unsigned int senderID ) = 0;


	virtual void Update( float deltaTime ) = 0;
};

#endif 
