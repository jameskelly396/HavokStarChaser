#ifndef _CPhysicsManagerFactory_HG_
#define _CPhysicsManagerFactory_HG_

// Abstract factory. 
// ONLY include interfaces, NOT implementation classes
#include "IPhysics.h"
#include <string>

class CPhysicsManagerFactory
{
public:
	static IPhysics* CreatePhysicsMananger( std::wstring managerType );
};

#endif 