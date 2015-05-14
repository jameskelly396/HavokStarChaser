//
// Written by Michael Feeney, Fanshawe College, (c) 2010-2014, mfeeney(at)fanshawec.ca
// For the 'Game Development - Advanced Programming' curriculum:  www.fanshawec.ca/en/GDP1
// It may be distributed under the terms of the MIT License: http://opensource.org/licenses/MIT
// Use this code at your own risk. It is indented only as a learning aid.
//
#include "ShaderTypes.h"
#include <sstream>

CShaderAttributeDescription::CShaderAttributeDescription() 
{
	this->Clear();
}

void CShaderAttributeDescription::Clear( void )
{
	this->programName = "";
	this->programID = 0;
	this->name = "";
	this->index = 0;
	this->size = 0;
	this->type = GL_ID_UNKNOWN;
	this->typeAsString = "";
}

CShaderUniformDescription::CShaderUniformDescription() 
{
	this->Clear();
}

void CShaderUniformDescription::Clear( void )
{
	this->programName = "";
	this->programID = 0;
	this->name = "";
	this->index = 0;
	this->size = 0;
	this->type = GL_ID_UNKNOWN;
	this->typeAsString = "";
}


CShaderDescription::CShaderDescription()
{
	this->ID = 0;
	this->name = "";
	this->type = GLSHADERTYPES::UNKNOWN;
	this->source = "";
	this->filename = "";
	this->bIsOK = false;
};

CShaderDescription::CShaderDescription( std::string name, GLSHADERTYPES::enumShaderType shaderType, 
									    std::string source, std::string fileName )
{
	this->ID = 0;
	this->name = name;
	this->type = shaderType;
	this->source = source;
	this->filename = fileName;
	this->bIsOK = false;
}


CShaderDescription::~CShaderDescription() { }

CShaderProgramDescription::CShaderProgramDescription()
{
	this->ID = 0;
	this->name = "";
	this->bIsOK = false;
}

CShaderProgramDescription::CShaderProgramDescription( std::string shaderProgName, 
													  CShaderDescription vShader, 
													  CShaderDescription fShader )
{
	this->ID = 0;
	this->name = shaderProgName;
	this->bIsOK = false;
	this->vShader = vShader;
	this->fShader = fShader;
}

CShaderProgramDescription::CShaderProgramDescription( std::string shaderProgName, 
													  CShaderDescription vShader, 
													  CShaderDescription fShader, 
													  CShaderDescription gShader )
{
	this->ID = 0;
	this->name = shaderProgName;
	this->bIsOK = false;
	this->vShader = vShader;
	this->fShader = fShader;
	this->gShader = gShader;
}


CShaderProgramDescription::~CShaderProgramDescription() { }

std::string CShaderProgramDescription::getErrorString( void )
{
	std::stringstream ss;
	std::vector< std::string >::iterator itErr;
	// Copy the various shader errors to the main error string 
	// Vertex shader errors...
	if ( !this->vShader.vecShaderErrors.empty() )
	{
		ss << "Vertex shader errors:" << std::endl;
		for ( itErr = this->vShader.vecShaderErrors.begin(); itErr != this->vShader.vecShaderErrors.end(); itErr++ )
		{
			ss << *itErr << std::endl;
		}
		ss << std::endl;
	}
	// Fragment shader errors...
	if ( !this->fShader.vecShaderErrors.empty() )
	{
		ss << "Fragment shader errors:" << std::endl;
		for ( itErr = this->fShader.vecShaderErrors.begin(); itErr != this->fShader.vecShaderErrors.end(); itErr++ )
		{
			ss << *itErr << std::endl;
		}
		ss << std::endl;
	}
	// Geometry shader errors...
	if ( !this->gShader.vecShaderErrors.empty() )
	{
		ss << "Geometry shader errors:" << std::endl;
		for ( itErr = this->gShader.vecShaderErrors.begin(); itErr != this->gShader.vecShaderErrors.end(); itErr++ )
		{
			ss << *itErr << std::endl;
		}
		ss << std::endl;
	}
	// And program (linker) errors:
	ss << "General program errors:" << std::endl;
	for ( itErr = this->vecErrors.begin(); itErr != this->vecErrors.end(); itErr++ )
	{
		ss << *itErr << std::endl;
	}
	return ss.str();
}
