#include "CLightManager.h"
#include <algorithm>
#include <iterator>
#include <sstream>

CLightManager::CLightManager()
{
	this->m_maxNumberOfLights = CLightManager::DEFAULT_MAX_NUMBER_OF_LIGHTS;
	this->m_vecLights.reserve( this->m_maxNumberOfLights );
	for ( unsigned int index = 0; index != this->m_maxNumberOfLights; index++ )
	{
		CLight* pTempLight = new CLight();
		pTempLight->setID( index );
		pTempLight->bIsEnabled = false;
		this->m_vecLights.push_back(pTempLight);
	}
	return;
}

CLightManager::CLightManager(unsigned int maxNumberOfLights)
{
	this->m_maxNumberOfLights = maxNumberOfLights;
	this->m_vecLights.reserve( this->m_maxNumberOfLights );
	for ( unsigned int index = 0; index != this->m_maxNumberOfLights; index++ )
	{
		CLight* pTempLight = new CLight();
		pTempLight->setID( index );
		pTempLight->bIsEnabled = false;
		this->m_vecLights.push_back(pTempLight);
	}
	return;
}

CLightManager::~CLightManager()
{
	return;
}

bool CLightManager::getHasShadows(void)
{
	// If any of the lights have shadows, return true
	for ( std::vector< CLight* >::iterator it_pLight = this->m_vecLights.begin();
		  it_pLight != this->m_vecLights.end(); it_pLight++ )
	{
		if ( (*it_pLight)->get_bHasShadow() )
		{
			return true;
		}
	}
	// None of the lights have shadows
	return false;
}


// This is an EXPENSIVE operation
bool CLightManager::initShadowMaps( unsigned int numberOfLightsWithShadows, unsigned int textureSize = CLightManager::DEFAULT_SHADOW_DEPTH_TEXTURE_SIZE )
{
	// Make 


	return true;
}

bool CLightManager::GetLightInformation(unsigned int lightID, CLight &theLight)
{
	if ( lightID >= this->m_vecLights.size() )	{ return false;	}

	if ( this->m_vecLights[lightID] == 0 )	{ return false;	}

	theLight = *( this->m_vecLights[lightID] );

	return true;
}

CLight* CLightManager::GetLightPointer(unsigned int lightID)
{
	if ( lightID >= this->m_vecLights.size() )	{ return false; }
	
	return this->m_vecLights[lightID];

	//std::map<unsigned int, CLight*>::iterator itLight 
	//	= this->m_map_pLights.find( lightID );
	//if ( itLight == this->m_map_pLights.end() )
	//{	// Didn't find it 
	//	return 0;
	//}
	//CLight*	pDebug = itLight->second;
	//return pDebug;
}

bool CLightManager::bIsLightPresent( unsigned int lightID )
{
	if ( lightID >= this->m_maxNumberOfLights )
	{	// ID is too high
		return false;
	}
	// We got a light at that location?
	if ( this->m_vecLights[lightID] == 0 )
	{// Nope
		return true;
	}
	// Yup, there's a light there
	return false;
}

// The lightID indicates which light this is
bool CLightManager::SetLight(CLight &theLight, bool bOverwriteIfPresent /*= true*/)
{
	// See if the light is there
	if ( theLight.getID() >= this->m_vecLights.size() )		{ return false; }

	if ( (this->m_vecLights[theLight.getID()] == 0) || bOverwriteIfPresent )
	{	// Not there, so add it
		(*(this->m_vecLights[theLight.getID()])) = theLight;
	}
	return true;

	//std::map<unsigned int, CLight*>::iterator itLight 
	//	= this->m_map_pLights.find( theLight.getID() );
	//if ( ( itLight == this->m_map_pLights.end() ) || (bOverwriteIfPresent) )
	//{	// Didn't find it, so add it (or we DID find, but are overwriting it
	//	(*(this->m_map_pLights[theLight.getID()])) = theLight;
	//	return true;
	//}
	//// It's there, but were not supposed to overwrite it
	//return false;
}

bool predicateLightIDGreater( CLight* pL1, CLight* pL2 )
{
	if ( pL1->getID() < pL2->getID() )
	{
		return true;
	}
	return false;
}

void CLightManager::GetAllLights(std::vector< CLight* > &vec_pLights )
{
	// Clear the vector??? But it's a vector of pointers...
	//for ( std::map<unsigned int, CLight*>::iterator itLight = this->m_map_pLights.begin();
	//	  itLight != this->m_map_pLights.end(); itLight++ )
	for ( std::vector<CLight*>::iterator itLight = this->m_vecLights.begin();
		  itLight != this->m_vecLights.end(); itLight++ )
	{
		vec_pLights.push_back( *itLight );
	}
	// Sort them by light ID
	std::sort( vec_pLights.begin(), vec_pLights.end(), predicateLightIDGreater );
	return;
}

void CLightManager::GetFirstNLights(std::vector< CLight* > &vec_pLights)
{
	// TODO:
	return;
}

// Returns ID of light. Note that the ID is not the same as the OpenGL ID in the shader
// (it can be, though)
//unsigned int CLightManager::CreateLight( CLight::enumLightType lightType )

unsigned int CLightManager::getIDofLastLight(void)
{
	unsigned int lastLightID = 0;
	//for ( std::map<unsigned int, CLight*>::iterator itLight = this->m_map_pLights.begin();
	//	  itLight != this->m_map_pLights.end(); itLight++ )
	for ( std::vector<CLight*>::iterator itLight = this->m_vecLights.begin();
		  itLight != this->m_vecLights.end(); itLight++ )
	{
		if ( *itLight != 0 )
		{
			if ( (*itLight)->getID() > lastLightID )	{ lastLightID = (*itLight)->getID(); }
		}
	}
	return lastLightID;
}

// Last light + 1
unsigned int CLightManager::getIDofNextLight(void) 
{
	if ( this->m_vecLights.empty() )	{ return 0; }
	//if ( this->m_map_pLights.empty() )
	if ( this->m_vecLights[0] = 0 )
	{
		return 0;	// There are no lights, to 0 is the first one
	}
	unsigned int iNextID = this->getIDofLastLight() + 1;
	return iNextID;
}

void CLightManager::CreateBasicOpenGLLights(bool bEraseExistingLights)
{
	// Erase existing? 
	if ( bEraseExistingLights )	
	{ 
		//for ( std::map< unsigned int, CLight* >::iterator itLight = this->m_map_pLights.begin();
		//	  itLight != this->m_map_pLights.end(); itLight++ )
		for ( std::vector< CLight* >::iterator itLight = this->m_vecLights.begin();
			  itLight != this->m_vecLights.end(); itLight++ )
		{
			delete (*itLight);
		}
		this->m_vecLights.clear(); 
	}

	unsigned int startingID = this->getIDofNextLight();

	std::vector< CLight* > vecTemp; 
	for ( int index = startingID; index != this->m_maxNumberOfLights; index++ )
	{
		CLight* pTempLight = new CLight();
		pTempLight->setID( index );
		if ( index == 0 )	{ pTempLight->bIsEnabled = true; }
		this->m_vecLights.push_back(pTempLight);
	}

	return;
}


// If you pass a smaller number than you have, the extra lights are deleted
void CLightManager::setMaxLights(unsigned int newMaxLights)
{
	this->m_maxNumberOfLights = newMaxLights;
	// Copy the lights into a temp vector
	std::vector<CLight*> vecTempLight;
	for ( std::vector<CLight*>::iterator itLight = this->m_vecLights.begin();
		  itLight != this->m_vecLights.end(); itLight++ )
	{
		vecTempLight.push_back( *itLight );
	}

	this->m_vecLights.clear();
	this->m_vecLights.reserve( this->m_maxNumberOfLights );
	for ( int index = 0; index != this->m_maxNumberOfLights; index++ )
	{
		this->m_vecLights.push_back(0);
	}
	
	unsigned int index = 0;
	for ( std::vector<CLight*>::iterator itLight = vecTempLight.begin(); itLight != vecTempLight.end(); 
		  itLight++, index++ )
	{
		if ( index < this->m_maxNumberOfLights )
		{
			CLight* pTempLight = 
			this->m_vecLights[index] = *itLight;
		}
		else
		{	// There are too many original lights, so delete this extra light
			delete (*itLight);
		}
	}
	return;
}

unsigned int CLightManager::getMaxLights(void)
{
	return this->m_maxNumberOfLights;
}

unsigned int CLightManager::getLastLightID(void)
{
	return this->m_maxNumberOfLights - 1;
}
void CLightManager::m_appendErrorString( std::string nextErrorText )
{
	std::stringstream ss;
	ss << this->m_lastError << nextErrorText;
	this->m_lastError = ss.str();
	return;
}

void CLightManager::m_appendErrorStringLine( std::string nextErrorTextLine )
{
	std::stringstream ss;
	ss << this->m_lastError << std::endl;
	ss << nextErrorTextLine << std::endl;
	this->m_lastError = ss.str();
	return;
}

std::string CLightManager::getLastError(void)
{
	std::string errorText = this->m_lastError;
	this->m_lastError = "";
	return errorText;
}

