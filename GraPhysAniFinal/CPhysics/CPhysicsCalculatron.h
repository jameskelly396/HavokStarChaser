#ifndef _CPhysicsCalculatron_HG_
#define _CPhysicsCalculatron_HG_

// This does the physics loop, collision, etc.

#include "../CGameObjects/IGameObjectPhysics.h"
#include <vector>

class CPhysicsCalculatron
{
public:
	CPhysicsCalculatron();
	~CPhysicsCalculatron();

	bool simulationStep( std::vector< IGameObjectPhysics* > &vecGameObjects, float deltaTime );
};

#endif
