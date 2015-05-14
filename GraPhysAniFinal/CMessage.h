#ifndef _CMessage_HG_
#define _CMessage_HG_

// Written by Michael Feeney, Fanshawe College, (c) 2014, mfeeney(at)fanshawec.ca
// For the 'Game Development - Advanced Programming' curriculum:  www.fanshawec.ca/en/GDP1
// It may be distributed under the terms of the MIT License: http://opensource.org/licenses/MIT
// Use this code at your own risk. It is indented only as a learning aid.

// This object is passed around the objects for communication

#include "CNameValuePair.h"
#include <vector>

class CMessage
{
public:
	CMessage();
	CMessage( std::vector< CNameValuePair > init_vecNVPairs );
	CMessage( CNameValuePair initNVPair );
	~CMessage();

	std::vector< CNameValuePair > vecNVPairs;

	bool findNVPairByName( std::string nameToFind, CNameValuePair &NVPair );
};

#endif
