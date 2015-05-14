#ifndef _IGameObjectPhysics_HG_
#define _IGameObjectPhysics_HG_

#include "../CPhysics/CPhysicalProp.h"

// This is the physics updaters view of the game object

class IGameObjectPhysics
{
public:
	virtual ~IGameObjectPhysics() {};

	// Quicker than asking for the entire CPhysicalProp structure
	virtual bool getIsIncludedInPhysicsUpdate(void) = 0;						// From IGameObjectPhysics
	// Reutrns the "last frame" properties (the stable ones)
	virtual void getPhysicalProperties(CPhysicalProp &physProp) = 0;			// From IGameObjectPhysics
	// Forces copy of 
	virtual void setPhysicalProperties(const CPhysicalProp &physProp) = 0;		// From IGameObjectPhysics
	// Called at the end of the physics update. 
	virtual void commitPhysicalProperties(void) = 0;							// From IGameObjectPhysics

	virtual void Update( float deltaTime ) = 0;									// From IGameObjectPhysics

};

#endif

