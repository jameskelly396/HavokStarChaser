#ifndef _CNinjaTeapot_HG_
#define _CNinjaTeapot_HG_

#include "CGameObject.h"

class CNinjaTeapot : public CGameObject
{
public: 
	CNinjaTeapot();
	virtual ~CNinjaTeapot();

	// Taken from the CGameObject
	virtual void Update( float deltaTime );
	virtual void receiveMessage( CMessage theMessage, unsigned int senderID );


};

#endif