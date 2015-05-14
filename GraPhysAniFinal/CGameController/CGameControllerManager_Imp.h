#ifndef _CGameControllerManager_Imp_HG_
#define _CGameControllerManager_Imp_HG_

#include <vector>
#include "IGameController.h"

class CGameControllerManager_Imp
{
public:
	CGameControllerManager_Imp();
	~CGameControllerManager_Imp();

	IGameController* getController(int playerNumber);
	// This will remove a controller (why would you do this??)
	// This one, you don't pass a number; it just picks the next one
	IGameController* getNextController(void);
	std::vector<unsigned int> getNumberOfActiveControllers(void);
	
	// This will remove a controller (why would you do this??)
	bool deleteController(int playerNumber);
private:
	// Can be a value from 0 to 3
	// from: http://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.reference.xinputgetstate(v=vs.85).aspx
	static const unsigned int MINCONTROLLERINDEX = 0;
	static const unsigned int MAXNUMCONTROLLERS = 3;
	static unsigned int m_nextControllerID;
	IGameController* m_GameControllers[MAXNUMCONTROLLERS];
};

#endif 
