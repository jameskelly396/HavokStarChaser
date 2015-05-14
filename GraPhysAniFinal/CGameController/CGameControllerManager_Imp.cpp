#include "CGameControllerManager_Imp.h"

#include "CXBoxController.h"


CGameControllerManager_Imp::CGameControllerManager_Imp()
{
	for ( int index = CGameControllerManager_Imp::MINCONTROLLERINDEX; index != CGameControllerManager_Imp::MAXNUMCONTROLLERS; index++ )
	{
		this->m_GameControllers[index] = 0;
	}
	return;
}

CGameControllerManager_Imp::~CGameControllerManager_Imp()
{
	// Get rid of all the controllers
	for ( int index = CGameControllerManager_Imp::MINCONTROLLERINDEX; index != CGameControllerManager_Imp::MAXNUMCONTROLLERS; index++ )
	{
		if ( this->m_GameControllers[index] != 0 )
		{
			delete this->m_GameControllers[index];
		}
	}
	return;
}

IGameController* CGameControllerManager_Imp::getController(int playerNumber)
{
	if ( ( playerNumber < CGameControllerManager_Imp::MINCONTROLLERINDEX ) || 
	     ( playerNumber >= CGameControllerManager_Imp::MAXNUMCONTROLLERS ) )
	{
		return 0;	// Invalid
	}
	if ( this->m_GameControllers[playerNumber] == 0 )
	{
		this->m_GameControllers[playerNumber] = new CXBoxController(playerNumber);
	}
	return this->m_GameControllers[playerNumber];
}

// This one, you don't pass a number; it just picks the next one
IGameController* CGameControllerManager_Imp::getNextController(void)
{
	// Got any controllers? 
	std::vector<unsigned int> vecControllerIDs = this->getNumberOfActiveControllers();
	// Any there? 
	if ( vecControllerIDs.empty() )
	{	// Nope, so create one and return it
		return this->getController( CGameControllerManager_Imp::MINCONTROLLERINDEX );
	}
	// or return the first one we found
	return this->getController( *( vecControllerIDs.begin() ) );
}

std::vector<unsigned int> CGameControllerManager_Imp::getNumberOfActiveControllers(void)
{
	std::vector<unsigned int> vecReturnIDs;
	for ( int index = CGameControllerManager_Imp::MINCONTROLLERINDEX; index != CGameControllerManager_Imp::MAXNUMCONTROLLERS; index++ )
	{
		if ( this->m_GameControllers[index] != 0 )
		{
			vecReturnIDs.push_back( index );
		}
	}
	return vecReturnIDs;
}
	
	// This will remove a controller (why would you do this??)
bool CGameControllerManager_Imp::deleteController(int playerNumber)
{
	if ( ( playerNumber < CGameControllerManager_Imp::MINCONTROLLERINDEX ) || 
	     ( playerNumber >= CGameControllerManager_Imp::MAXNUMCONTROLLERS ) )
	{
		return false;
	}
	delete this->m_GameControllers[playerNumber];
	return true;
}
