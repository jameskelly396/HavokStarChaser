#ifndef _CShaderDescription_HG_
#define _CShaderDescription_HG_
//
// Written by Michael Feeney, Fanshawe College, (c) 2010-2014, mfeeney(at)fanshawec.ca
// For the 'Game Development - Advanced Programming' curriculum:  www.fanshawec.ca/en/GDP1
// It may be distributed under the terms of the MIT License: http://opensource.org/licenses/MIT
// Use this code at your own risk. It is indented only as a learning aid.
//

#include <gl\freeglut.h>
#include <gl\glext.h>		// OpenGL Extensions
#include <string>
#include <vector>
#include <map>


namespace GLSHADERTYPES
{
	enum enumShaderType
	{
		VERTEX_SHADER,
		FRAGMENT_SHADER,
		GEOMETRY_SHADER,
		TESSELLATION_SHADER,
		UNKNOWN
	};
}

// Attribute types (they are type GLenum):
// (Note: There is no GL_UNKNOWN or something like that, so I chose "GL_ID_UNKNOWN" to
//		  indicate an attribute of unknown type.)
//
//	GL_FLOAT,			
//	GL_FLOAT_VEC2,		
//	GL_FLOAT_VEC3,		
//	GL_FLOAT_VEC4, 
//	GL_FLOAT_MAT2, 
//	GL_FLOAT_MAT3, 
//	GL_FLOAT_MAT4, 
//	GL_FLOAT_MAT2x3,		// OpenGL 2.1 only
//	GL_FLOAT_MAT2x4,		// OpenGL 2.1 only
//	GL_FLOAT_MAT3x2,		// OpenGL 2.1 only 
//	GL_FLOAT_MAT3x4,		// OpenGL 2.1 only 
//	GL_FLOAT_MAT4x2,		// OpenGL 2.1 only
//	GL_FLOAT_MAT4x3			// OpenGL 2.1 only
//
//	GL_ID_UNKNOWN
//
class CShaderAttributeDescription
{
public:
	CShaderAttributeDescription();
	std::string programName;
	GLuint		programID;
	std::string name;
	GLuint		index;
	GLenum		type;
	std::string typeAsString;		// English word for GLenum type...
	GLint		size;		// size based on type
	void Clear( void );
};

class CShaderUniformDescription
{
public:
	CShaderUniformDescription();
	std::string programName;
	GLuint		programID;
	std::string name;
	GLuint		index;
	GLenum		type;
	std::string typeAsString;		// English word for GLenum type...
	GLint		size;		// size based on type
	void Clear( void );
};

class CShaderDescription
{
public:
	CShaderDescription();
	CShaderDescription( std::string name, GLSHADERTYPES::enumShaderType shaderType, 
						std::string source, std::string fileName );
	~CShaderDescription();
	GLuint							ID;
	std::string						name;
	GLSHADERTYPES::enumShaderType	type;	// VERTEX_SHADER, FRAGMENT_SHADER, or GEOMETRY_SHADER
	std::string						source;
	std::string						filename;
	std::vector< std::string >		vecShaderErrors;
	std::string getErrorString( void );
	bool							bIsOK;		// Compiled without errors
};

class CShaderProgramDescription
{
public:
	CShaderProgramDescription();
	CShaderProgramDescription( std::string shaderProgName, 
							   CShaderDescription vShader, CShaderDescription fShader );
	CShaderProgramDescription( std::string shaderProgName, 
							   CShaderDescription vShader, CShaderDescription fShader, CShaderDescription gShader );
	~CShaderProgramDescription();
	GLuint						ID;
	std::string					name;
	CShaderDescription			vShader;
	CShaderDescription			fShader;
	CShaderDescription			gShader;
	// These store the attribute and uniform variables from the shader (after link)
	GLuint getUniformVarIDFromName( std::string variableName );
	std::map< std::string /*attribName*/, CShaderAttributeDescription > mapShaderAttribs;
	std::map< std::string /*uniformName*/, CShaderUniformDescription > mapShaderUniforms;
	//
	std::vector< std::string >	vecErrors;
	std::string getErrorString( void );
	bool						bIsOK;		// Compiled, linked, and ready to go.
};

#endif
