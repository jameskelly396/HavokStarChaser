#ifndef _CLightManager_HG_
#define _CLightManager_HG_

#include "CLight.h"
#include <vector>
#include <map>

class CLightManager
{
public:
	CLightManager();
	CLightManager(unsigned int maxNumberOfLights);
	~CLightManager();
	bool GetLightInformation(unsigned int lightID, CLight &theLight);
	bool bIsLightPresent( unsigned int lightID );
	// Returns 0 (null) if bad
	CLight* GetLightPointer(unsigned int lightID);
	// Uses the ID (which is the array index in the shader) 
	bool SetLight(CLight &theLight, bool bOverwriteIfPresent = true);
	void GetFirstNLights(std::vector< CLight* > &vec_pLights);
	void GetAllLights(std::vector< CLight* > &vec_pLights);
	// Returns ID of light. Note that the ID is not the same as the OpenGL ID in the shader
	// (it can be, though)
	//unsigned int CreateLight( CLight::enumLightType lightType );
	void CreateBasicOpenGLLights(bool bEraseExistingLights);
	unsigned int getIDofLastLight(void);
	unsigned int getIDofNextLight(void);	// Last light + 1

	void setMaxLights(unsigned int newMaxLights);
	unsigned int getMaxLights(void);
	unsigned int getLastLightID(void);

	bool getHasShadows(void);
	// This is an EXPENSIVE operation. You call it once to create 
	//	the shadow maps, but -NOT- every frame
	// It's also SUPER expensive in terms of texture memory
	static const unsigned int DEFAULT_SHADOW_DEPTH_TEXTURE_SIZE = 4096;
	bool initShadowMaps( unsigned int numberOfLightsWithShadows, unsigned int textureSize );

	std::string getLastError(void);
private:
	static const unsigned int DEFAULT_MAX_NUMBER_OF_LIGHTS = 2;  //Also change max light value in ShaderUniformVariable.h and OpenGL.LightTexSkyUber.fragment_texture.glsl
	static const unsigned int DEFAULT_MAX_NUMBER_OF_LIGHTS_WITH_SHADOWS = 10;
	std::vector< CLight* > m_vecLights;
	//std::map<unsigned int, CLight*> m_map_pLights;
	unsigned int m_maxNumberOfLights;

	std::string m_lastError;
	void m_appendErrorString( std::string nextErrorText );
	void m_appendErrorStringLine( std::string nextErrorTextLine );

};

#endif
