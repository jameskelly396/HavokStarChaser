#include "CGameControllerManager.h"
#include "CGameControllerManager_Imp.h"

// Sigleton method
/* static */
CGameControllerManager* CGameControllerManager::getGameControllerManager(void)
{
	if ( CGameControllerManager::m_sp_theOneAndOnlyGameControllerManager == 0 )
	{
		CGameControllerManager::m_sp_theOneAndOnlyGameControllerManager = new CGameControllerManager();
	}
	return CGameControllerManager::m_sp_theOneAndOnlyGameControllerManager;
}

/* static */
CGameControllerManager* CGameControllerManager::m_sp_theOneAndOnlyGameControllerManager = 0;


CGameControllerManager::CGameControllerManager()
{
	this->m_p_GCMImp = new CGameControllerManager_Imp();
	return;
}

CGameControllerManager::~CGameControllerManager()
{
	delete this->m_p_GCMImp;
}
	
// Can be a value from 0 to 3
// from: http://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.reference.xinputgetstate(v=vs.85).aspx
IGameController* CGameControllerManager::getController(int playerNumber)
{
	return this->m_p_GCMImp->getController(playerNumber);
}

IGameController* CGameControllerManager::getNextController(void)
{
	return this->m_p_GCMImp->getNextController();
}

std::vector<unsigned int> CGameControllerManager::getNumberOfActiveControllers(void)
{
	return this->m_p_GCMImp->getNumberOfActiveControllers();
}

bool CGameControllerManager::deleteController(int playerNumber)
{
	return this->m_p_GCMImp->deleteController( playerNumber );
}
