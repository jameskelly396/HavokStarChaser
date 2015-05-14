#ifndef _CNameValuePair_HG_
#define _CNameValuePair_HG_

// Written by Michael Feeney, Fanshawe College, (c) 2014, mfeeney(at)fanshawec.ca
// For the 'Game Development - Advanced Programming' curriculum:  www.fanshawec.ca/en/GDP1
// It may be distributed under the terms of the MIT License: http://opensource.org/licenses/MIT
// Use this code at your own risk. It is indented only as a learning aid.

#include <string>
#include "CCommonTypes/CVector4f.h"


// This is sort of like JSON (which maybe got the idea from id software, 
//	who likely got the idea from someone else. All communication between
//	objects in Quake is through a "string-float" pair of items. Keep in 
//	mind that this was circa 1995, with 90MHz machines with 16 Mbytes of RAM.
//	...and it was fast enough then.)
class CNameValuePair
{
public:
	CNameValuePair() : iValue(0), fValue(0.0f) {};
	// A few helpful constructors
	CNameValuePair( std::string init_name );
	CNameValuePair( int init_iValue );	// Note there's no name here
	CNameValuePair( float init_fValue );	// Note there's no name here
	CNameValuePair( CVector4f init_vec4Value );
	CNameValuePair( std::string init_name, std::string init_sValue );
	CNameValuePair( std::string init_name, int init_iValue );
	CNameValuePair( std::string init_name, float init_fValue );	
	CNameValuePair( std::string init_name, CVector4f init_vec4Value );

	~CNameValuePair() {};

	std::string name;
	std::string sValue;
	int iValue;
	float fValue;
	CVector4f vec4Value;
};

#endif