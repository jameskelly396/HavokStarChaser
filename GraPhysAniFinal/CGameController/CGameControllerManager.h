#ifndef _CGameControllerManager_HG_
#define _CGameControllerManager_HG_

#include "IGameController.h"
#include <vector>

// Forward declaration for p-imp pattern
class CGameControllerManager_Imp;	

// This is a singleton
class CGameControllerManager
{
public:
	static CGameControllerManager* getGameControllerManager(void);
	~CGameControllerManager();
	
	// Can pass zero (0) to get whatever next controller there is
	// Returns null (0) if can't get one
	IGameController* getController(int playerNumber);
	// This one, you don't pass a number; it just picks the next one
	IGameController* getNextController(void);
	std::vector<unsigned int> getNumberOfActiveControllers(void);
	// This will remove a controller (why would you do this??)
	bool deleteController(int playerNumber);

private: 
	// Can't be called directly (it's a singleton)
	CGameControllerManager();
	static CGameControllerManager* m_sp_theOneAndOnlyGameControllerManager;
	CGameControllerManager_Imp* m_p_GCMImp;
};

#endif

	