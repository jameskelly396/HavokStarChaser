#ifndef _CTextureManager_HG_
#define _CTextureManager_HG_

// Loads various textures into memory
// Prepares and loads textures into OpenGL texture units
// Stores texture to textureID look up

#include <string>
#include "CTextureFromBMP.h"
#include <map>
#include <string>

class CTextureManager
{
public:
	CTextureManager();
	~CTextureManager();

	void setBasePath( std::string basePath );
	bool loadTexture( std::string fileName );

	bool Create2DTextureFromBMPFile( std::string fileName, bool bGenerateMIPMap );
	bool CreateCubeTextureFromBMPFiles( std::string cubeMapName, 
		                                std::string posX_fileName, std::string negX_fileName, 
	                                    std::string posY_fileName, std::string negY_fileName, 
										std::string posZ_fileName, std::string negZ_fileName, 
										bool bGenerateMipmap, bool bIsSeamless );
	// returns zero if no texture
	CTextureFromBMP* getTextureFromTextureName( std::string textureName );

	std::string getLastError(void);

private:
	std::map< std::string, CTextureFromBMP* > m_map_TexNameToTexture;
	GLuint m_nextTextureUnitOffset;
	static const GLuint m_BASETEXTURE = GL_TEXTURE0;
	std::string m_basePath;
	std::string m_lastError;
	void m_appendErrorString( std::string nextErrorText );
	void m_appendErrorStringLine( std::string nextErrorTextLine );
};

#endif




