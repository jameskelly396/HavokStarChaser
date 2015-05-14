#include "CTextureManager.h"
#include <sstream>

CTextureManager::CTextureManager()
{
	this->m_nextTextureUnitOffset = 0;
	return;
}

CTextureManager::~CTextureManager()
{
	return;
}

void CTextureManager::setBasePath( std::string basePath )
{
	this->m_basePath = basePath;
	return;
}

void CTextureManager::m_appendErrorString( std::string nextErrorText )
{
	std::stringstream ss;
	ss << this->m_lastError << nextErrorText;
	this->m_lastError = ss.str();
	return;
}

void CTextureManager::m_appendErrorStringLine( std::string nextErrorTextLine )
{
	std::stringstream ss;
	ss << this->m_lastError << std::endl;
	ss << nextErrorTextLine << std::endl;
	this->m_lastError = ss.str();
	return;
}

bool CTextureManager::Create2DTextureFromBMPFile( std::string fileName, bool bGenerateMIPMap )
{
	std::string fileToLoad = this->m_basePath + "/" + fileName;

	// #define GL_TEXTURE0 0x84C0
	GLuint textureUnit = this->m_BASETEXTURE + this->m_nextTextureUnitOffset;
	
	// Gone over the GL_TEXTURE31? ('cause we can't load any more, then)
	if ( textureUnit > GL_TEXTURE31 )		// #define GL_TEXTURE31 0x84DF
	{
		this->m_appendErrorStringLine( "Too many textures are loaded. GL_TEXTURE31 is maximum." );
		return false;
	}

	CTextureFromBMP* pTempTexture = new CTextureFromBMP();
	if ( ! pTempTexture->CreateNewTextureFromBMPFile2( fileToLoad, textureUnit, bGenerateMIPMap ) )
	{
		this->m_appendErrorString( "Can't load " );
		this->m_appendErrorString( fileToLoad );
		this->m_appendErrorString( "\n" );
		return false;
	}

	// Texture is loaded OK
	this->m_nextTextureUnitOffset++;
	
	this->m_map_TexNameToTexture[ fileName ] = pTempTexture;

	return true;
}



bool CTextureManager::CreateCubeTextureFromBMPFiles( std::string cubeMapName, 
													 std::string posX_fileName, std::string negX_fileName, 
	                                                 std::string posY_fileName, std::string negY_fileName, 
									                 std::string posZ_fileName, std::string negZ_fileName, 
									                 bool bGenerateMipmap, bool bIsSeamless )
{
	std::string posX_fileName_FULL = this->m_basePath + "/" + posX_fileName;
	std::string negX_fileName_FULL = this->m_basePath + "/" + negX_fileName;
	std::string posY_fileName_FULL = this->m_basePath + "/" + posY_fileName;
	std::string negY_fileName_FULL = this->m_basePath + "/" + negY_fileName;
	std::string posZ_fileName_FULL = this->m_basePath + "/" + posZ_fileName;
	std::string negZ_fileName_FULL = this->m_basePath + "/" + negZ_fileName;


	// #define GL_TEXTURE0 0x84C0
	GLuint textureUnit = this->m_BASETEXTURE + this->m_nextTextureUnitOffset;
	
	// Gone over the GL_TEXTURE31? ('cause we can't load any more, then)
	if ( textureUnit > GL_TEXTURE31 )		// #define GL_TEXTURE31 0x84DF
	{
		this->m_appendErrorStringLine( "Too many textures are loaded. GL_TEXTURE31 is maximum." );
		return false;
	}

	CTextureFromBMP* pTempTexture = new CTextureFromBMP();
	GLenum errorEnum = GL_NO_ERROR;
	std::string errorString;
	std::string errorDetails;
	if ( ! pTempTexture->CreateNewCubeTextureFromBMPFiles( posX_fileName_FULL, negX_fileName_FULL,
		                                                   posY_fileName_FULL, negY_fileName_FULL,
														   posZ_fileName_FULL, negZ_fileName_FULL,
														   textureUnit, bIsSeamless, 
														   errorEnum, errorString, errorDetails ) )
	{
		this->m_appendErrorString( "Can't load " );
		this->m_appendErrorString( cubeMapName );
		this->m_appendErrorString( "\n" );
		this->m_appendErrorStringLine( errorString );
		this->m_appendErrorStringLine( errorDetails );
		return false;
	}



	// Texture is loaded OK
	this->m_nextTextureUnitOffset++;
	
	this->m_map_TexNameToTexture[ cubeMapName ] = pTempTexture;

	return true;
}


// Returns zero if invalid
CTextureFromBMP* CTextureManager::getTextureFromTextureName( std::string textureName )
{
	std::map< std::string, CTextureFromBMP* >::iterator itTexture = this->m_map_TexNameToTexture.find(textureName);

	if ( itTexture == this->m_map_TexNameToTexture.end() )
	{
		return 0;
	}

	return itTexture->second;
}

std::string CTextureManager::getLastError(void)
{
	std::string errorText = this->m_lastError;
	this->m_lastError = "";
	return errorText;
}


