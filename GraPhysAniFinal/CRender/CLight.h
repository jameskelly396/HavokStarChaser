#ifndef _CLight_HG_
#define _CLight_HG_

// This reperesents a light in the model
#include "../CCommonTypes/CVector3f.h"
#include <string>

class CLight
{
public:
	enum enumLightType
	{
		DIRECTIONAL,
		POINT,		// Default
		SPOT
	};
	enumLightType getLightType(void);
	std::string getLightTypeString(void);
	void setLightType( enumLightType newType );
	CLight();
	~CLight();
	bool bIsEnabled;							unsigned int ID_bIsEnabled;	

	bool get_bHasShadow(void);
	void set_bHasShadow(unsigned int shadowTextureID);
	void clear_bHasShadow(void);
	unsigned int getShadowTextureID(void);

	CVector3f position;							unsigned int ID_position;
	std::string getPositionString(void);		
	CVector3f halfVector;						unsigned int ID_halfVector;
	CVector3f spotConeDirection;				unsigned int ID_spotConeDirection;
	float spotCosCutoff;						unsigned int ID_spotCosCutoff;
	float spotExponent;							unsigned int ID_spotExponent;
	float constantAttenuation;					unsigned int ID_constantAttenuation;
	float linearAttenuation;					unsigned int ID_linearAttenuation;
	float quadraticAttenuation;					unsigned int ID_quadraticAttenuation;
	// Lights effective range (for performance)
	float range;								unsigned int ID_range;
	// Calculates a range based on the attenuation values (according to the OpenGL Programming Guide 
	//	basic shader - with all three attenuations. blackValue is what 'black' is, usually the ambient value)
	void calculateRangeFromAttenuation( float maxRange, float blackValue );
	CVector3f ambientColour;					unsigned int ID_ambientColour;
	CVector3f diffuseColour;					unsigned int ID_diffuseColour;
	void CalculateHalfVectorForDirectionalLights(CVector3f eye);	// normalize( lightDirection + EyeDirection );

	// FALSE for directional lights	
	bool bIsLocal;								unsigned int ID_bIsLocal;
	bool bIsSpot;								unsigned int ID_bIsSpot;

	unsigned int getID(void) const; 
	void setID( unsigned int newID );		// This can be used as the OpenGL light array index
private:
	//unsigned int m_OpenGLShaderID;
	unsigned int m_ID;
	//static unsigned int m_nextID;
	enumLightType m_lightType;
	bool m_bHasShadow;
	unsigned int m_ShadowTextureID;
};

#endif
