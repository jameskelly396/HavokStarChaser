#include "CNameValuePair.h" 

// Written by Michael Feeney, Fanshawe College, (c) 2014, mfeeney(at)fanshawec.ca
// For the 'Game Development - Advanced Programming' curriculum:  www.fanshawec.ca/en/GDP1
// It may be distributed under the terms of the MIT License: http://opensource.org/licenses/MIT
// Use this code at your own risk. It is indented only as a learning aid.

CNameValuePair::CNameValuePair( std::string init_name )
{
	this->name = init_name;
	return;
}

CNameValuePair::CNameValuePair( int init_iValue )	// Note there's no name here
{
	this->iValue = init_iValue;
	return;
}

CNameValuePair::CNameValuePair( float init_fValue )	// Note there's no name here
{
	this->fValue = init_fValue;
	return;
}

CNameValuePair::CNameValuePair( std::string init_name, std::string init_sValue )
{
	this->name = init_name;
	this->sValue = init_sValue;
	return;
}

CNameValuePair::CNameValuePair( std::string init_name, int init_iValue )
{
	this->name = init_name;
	this->iValue = init_iValue;
	return;
}

CNameValuePair::CNameValuePair( std::string init_name, float init_fValue )
{
	this->name = init_name;
	this->fValue = init_fValue;
	return;
}

CNameValuePair::CNameValuePair( CVector4f init_vec4Value )
{
	this->vec4Value = init_vec4Value;
	return;
}

CNameValuePair::CNameValuePair( std::string init_name, CVector4f init_vec4Value )
{
	this->name = init_name;
	this->vec4Value = init_vec4Value;
	return;
}
