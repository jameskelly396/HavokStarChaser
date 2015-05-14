#include "CLight.h"
#include <sstream>

CLight::CLight()
{
	this->bIsEnabled = false;
	this->m_lightType = CLight::POINT;
	this->spotExponent = 0.0f;
	this->spotConeDirection = CVector3f( 0.0f, 0.0f, 1.0f );	// Down the z-axis
	// These give a pretty decent range
	this->constantAttenuation = 0.510407f; //1.67f;	//
	this->linearAttenuation = 0.00701723f;
	this->quadraticAttenuation = 0.00150008f;
	this->range = 1000.0f;		// This is WAY too large

	this->diffuseColour = CVector3f(1.0f, 1.0f, 1.0f);
	this->ambientColour = CVector3f(0.2f, 0.2f, 0.2f);

	//this->m_ID = this->m_nextID;
	//this->m_OpenGLShaderID = 0;
	this->m_ID;

	this->ID_bIsEnabled = 0;
	this->ID_bIsLocal = 0;
	this->ID_bIsSpot = 0;
	this->ID_ambientColour = 0;
	this->ID_diffuseColour = 0;
	this->ID_position = 0;
	this->ID_halfVector = 0;
	this->ID_spotConeDirection = 0;
	this->ID_spotExponent = 0;
	this->ID_constantAttenuation = 0;
	this->ID_linearAttenuation = 0;
	this->ID_quadraticAttenuation = 0;
	this->ID_range = 0;

	this->m_bHasShadow = false;
	this->m_ShadowTextureID = 0;	// 0 is invalid

	return;
}



CLight::~CLight()
{
	return;
}

// Calculates a range based on the attenuation values (according to the OpenGL Programming Guide 
//	basic shader - with all three attenuations. blackValue is what 'black' is, usually the ambient value)
void CLight::calculateRangeFromAttenuation( float maxRange, float blackValue )
{
	float curRange = 0.0f; 
	float attenuationValue = 1.0f;
	while ( ( curRange < maxRange ) && ( attenuationValue >= blackValue ) )
	{
		attenuationValue = 1.0f / 
			               (  this->constantAttenuation + 
						     (this->linearAttenuation * curRange) + 
						     (this->quadraticAttenuation * curRange * curRange ) );
		curRange += 1.0f;
	};
	this->range = curRange;
	return;
}

CLight::enumLightType CLight::getLightType(void)
{
	return this->m_lightType;
}

std::string CLight::getLightTypeString(void)
{
	switch ( this->m_lightType )
	{
	case CLight::POINT:
		return "point";
		break;
	case CLight::DIRECTIONAL:
		return "directional";
		break;
	case CLight::SPOT:
		return "spot";
		break;
	}
	return "Unknown or error";
}


void CLight::setLightType( enumLightType newType )
{
	switch ( this->m_lightType )
	{
	case CLight::POINT:
		this->bIsLocal = true;
		this->bIsSpot = false;
		break;
	case CLight::DIRECTIONAL:
		this->bIsLocal = false;
		this->bIsSpot = false;
		break;
	case CLight::SPOT:
		this->bIsLocal = true;
		this->bIsSpot = true;
		break;
	}
	return;
}

// TODO: do the half-vector calcultion for directional lights
void CLight::CalculateHalfVectorForDirectionalLights(CVector3f eye)	// normalize( lightDirection + EyeDirection );
{
	// TODO: Perform calculation here
	return;
}

/*static*/
//unsigned int CLight::m_nextID = 0;

unsigned int CLight::getID(void) const
{
	return this->m_ID;
}

void CLight::setID( unsigned int newID )
{
	this->m_ID = newID;
	return;
}

std::string CLight::getPositionString(void)
{
	std::stringstream ss;
	ss << "(" << this->position.x << ", " << this->position.y << ", " << this->position.z << ")";
	return ss.str();
}

bool CLight::get_bHasShadow(void)
{
	return this->m_bHasShadow;
}

void CLight::set_bHasShadow(unsigned int shadowTextureID)
{
	this->m_ShadowTextureID = shadowTextureID;
	this->m_bHasShadow = true;
	return;	
}

void CLight::clear_bHasShadow(void)
{
	this->m_ShadowTextureID = 0;
	this->m_bHasShadow = false;
	return;
}

unsigned int CLight::getShadowTextureID(void)
{
	return this->m_ShadowTextureID;
}
