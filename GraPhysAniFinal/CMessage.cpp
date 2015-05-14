#include "CMessage.h"

CMessage::CMessage()
{
	return;
}

CMessage::CMessage( std::vector< CNameValuePair > init_vecNVPairs )
{
	this->vecNVPairs = init_vecNVPairs;
	return;
}

CMessage::CMessage( CNameValuePair initNVPair )
{
	this->vecNVPairs.push_back( initNVPair );
	return;
}


CMessage::~CMessage()
{
	return;
}

bool CMessage::findNVPairByName( std::string nameToFind, CNameValuePair &NVPair )
{
	for ( int index = 0; index != static_cast<int>( this->vecNVPairs.size() ); index++ )
	{	// The one?
		if ( this->vecNVPairs[index].name == nameToFind )
		{	// Yup
			NVPair = this->vecNVPairs[index];
			return true;
		}
	}
	return false;	// Sorry it didn't work out. 
}

