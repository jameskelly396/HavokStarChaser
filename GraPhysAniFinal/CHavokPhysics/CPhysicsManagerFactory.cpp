#include "CPhysicsManagerFactory.h"

// ALSO include the actual thingies you are going to create.
#include "CHavokPhysics.h"

//static 
IPhysics* CPhysicsManagerFactory::CreatePhysicsMananger( std::wstring managerType )
{
	
	if ( managerType == L"Havok" )
	{
		std::cout << "Creating Havok" << std::endl; std::cout.flush();
		return new CHavokPhysics();
	}
	std::cout << "Failed Creating Havok" << std::endl; std::cout.flush();
	// It's all ending in tears...
	return 0;
}


void CNameValuePair2::Clear(void)
{
	this->name = L"";
	this->fValue = 0.0f;
	this->bValue = false;
	this->sValue = L"";
	return;
}

CNameValuePair2::CNameValuePair2()
{
	this->Clear();
	return;
}
CNameValuePair2::CNameValuePair2( std::wstring initName, float initfValue )
{
	this->Clear();
	this->name = initName;
	this->fValue = initfValue;
	return;
};	

CNameValuePair2::CNameValuePair2( std::wstring initName, bool initbValue )
{
	this->Clear();
	this->name = initName;
	this->bValue = initbValue;
	return;
}

CNameValuePair2::CNameValuePair2( std::wstring initName, std::wstring initsValue )
{
	this->Clear();
	this->name = initName;
	this->sValue = initsValue;
	return;
}


