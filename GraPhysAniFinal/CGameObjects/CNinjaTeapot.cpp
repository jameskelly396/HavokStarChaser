#include "CNinjaTeapot.h"

CNinjaTeapot::CNinjaTeapot()
{
	return;
}

CNinjaTeapot::~CNinjaTeapot()
{
	return;
}

void CNinjaTeapot::Update( float deltaTime )
{

	// Do specific stuff, and optionally call the base class, perhaps

	CGameObject::Update( deltaTime );

	return;
}

void CNinjaTeapot::receiveMessage( CMessage theMessage, unsigned int senderID )
{
	// Do stuff that's specific to this type of object...

	// Calls the base class (or in Java, the super class)
	CGameObject::receiveMessage( theMessage, senderID );

	return;
}
