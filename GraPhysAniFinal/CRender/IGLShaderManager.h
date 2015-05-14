// Written by Michael Feeney, Fanshawe College, 2010
// mfeeney@fanshawec.on.ca
// It may be distributed under the terms of the General Public License:
// http://www.fsf.org/licenses/gpl.html
// Use this code at your own risk. It is indented only as a learning aid.
//
#ifndef _IGLShaderManager_HG_
#define _IGLShaderManager_HG_

//#include <gl\glext.h>		// OpenGL Extensions
//#include <gl\freeglut.h>
#include <GL\glew.h>			// Because it complains when we don't include it
#include <GL\freeglut.h>		// Because of the GLuint
#include "ShaderTypes.h"

class IGLShaderManager
{
public:
	virtual bool CreateShaderProgramFromSource( CShaderProgramDescription &shaderProgDescription ) = 0;
	virtual bool CreateShaderProgramFromFile( CShaderProgramDescription &shaderProgDescription ) = 0;

	virtual bool UseShaderProgram( GLuint shaderID ) = 0;
	virtual bool UseShaderProgram( std::string shaderName ) = 0;

	// Returns zero (0) and blank string if no valid shader exists
	virtual GLuint getCurrentShaderID(void) = 0;
	virtual GLuint getLastShaderID(void) = 0;
	virtual std::string getCurrentShaderName(void) = 0;
	virtual std::string getLastShaderName(void) = 0;

	// Returns false if the shader isn't present 
	// NOTE: Used shaderID unless ID == 0, then tries with the shader name
	virtual bool GetShaderProgramInfo( CShaderProgramDescription &shaderProgDescription ) = 0;

	// Uses glGetActiveAttrib and glGetActiveUniform to determine the active variables in use in the shader
	// BIG FAT NOTE: This is only the "active" ones - many times, they are profiled out by the compiler
	virtual bool LoadActiveShaderVariables( GLuint shaderID ) = 0;
	virtual bool LoadActiveShaderVariables( std::string shaderName ) = 0;
	//
	// Returns -1 (standardish OpenGL invalid index value) if the shader isn't found
	virtual GLint GetShaderIDFromName( std::string shaderName ) = 0;

	virtual bool IsShaderOK( GLuint shaderProgramID ) = 0;
	virtual bool IsShaderOK( std::string shaderName ) = 0;

	virtual std::string getLastError( void ) = 0;

	//virtual bool DeleteShader( GLuint shaderProgramID, int &error ) = 0;
	//virtual bool DeleteShader( std::string shaderName, int &error ) = 0;
	////
	////
	//virtual bool LoadAttributeUniform( std::string name ) = 0;
	//virtual bool LoadAttributeVarying( std::string name ) = 0;


	/*************************************************************
		After this is the are endless repeated variations of the 
		varying and uniform variable accessors...
	*************************************************************/


	//      _ _   _      _  __               ___  __
	// __ _| | | | |_ _ (_)/ _|___ _ _ _ __ / \ \/ /
	/// _` | | |_| | ' \| |  _/ _ \ '_| '  \| |>  < 
	//\__, |_|\___/|_||_|_|_| \___/_| |_|_|_|_/_/\_\
	//|___/                                         
	// glUniform1f...
	virtual bool SetUniformVar1f( std::string shaderName, std::string varName, GLfloat value ) = 0;			
	virtual bool SetUniformVar1f( GLuint shaderID, std::string varName, GLfloat value ) = 0;	
	// glUniform1i...
	virtual bool SetUniformVar1i( std::string shaderName, std::string varName, GLint value ) = 0;
	virtual bool SetUniformVar1i( GLuint shaderID, std::string varName, GLint value ) = 0;
	// glUniform1ui... (OpenGL 3.0)
	virtual bool SetUniformVar1ui( std::string shaderName, std::string varName, GLint value ) = 0;	// OpenGL 3.0
	virtual bool SetUniformVar1ui( GLuint shaderID, std::string varName, GLint value ) = 0;			// OpenGL 3.0

	 //      _ _   _      _  __               _____  __
	 // __ _| | | | |_ _ (_)/ _|___ _ _ _ __ |_  ) \/ /
	 /// _` | | |_| | ' \| |  _/ _ \ '_| '  \ / / >  < 
	 //\__, |_|\___/|_||_|_|_| \___/_| |_|_|_/___/_/\_\
	 //|___/                                           
	// glUniform2f
	virtual bool SetUniformVar2f( std::string shaderName, std::string varName, GLfloat v1, GLfloat v2 ) = 0;
	virtual bool SetUniformVar2f( std::string shaderName, std::string varName, GLfloat v[2] ) = 0;
	virtual bool SetUniformVar2f( GLuint shaderID, std::string varName, GLfloat v1, GLfloat v2 ) = 0;
	virtual bool SetUniformVar2f( GLuint shaderID, std::string varName, GLfloat v[2] ) = 0;
	// glUniform2i
	virtual bool SetUniformVar2i( std::string shaderName, std::string varName, GLint v1, GLint v2 ) = 0;
	virtual bool SetUniformVar2i( std::string shaderName, std::string varName, GLint v[2] ) = 0;
	virtual bool SetUniformVar2i( GLuint shaderID, std::string varName, GLint v1, GLint v2 ) = 0;
	virtual bool SetUniformVar2i( GLuint shaderID, std::string varName, GLint v[2] ) = 0;
	// glUniform2ui
	virtual bool SetUniformVar2ui( std::string shaderName, std::string varName, GLuint v1, GLuint v2 ) = 0;	// OpenGL 3.0
	virtual bool SetUniformVar2ui( std::string shaderName, std::string varName, GLuint v[2] ) = 0;	// OpenGL 3.0
	virtual bool SetUniformVar2ui( GLuint shaderID, std::string varName, GLuint v1, GLuint v2 ) = 0;		// OpenGL 3.0
	virtual bool SetUniformVar2ui( GLuint shaderID, std::string varName, GLuint v[2] ) = 0;		// OpenGL 3.0

	
	//      _ _   _      _  __               ______  __
	// __ _| | | | |_ _ (_)/ _|___ _ _ _ __ |__ /\ \/ /
	/// _` | | |_| | ' \| |  _/ _ \ '_| '  \ |_ \ >  < 
	//\__, |_|\___/|_||_|_|_| \___/_| |_|_|_|___//_/\_\
	//|___/                                            
	// glUniform3f
	virtual bool SetUniformVar3f( std::string shaderName, std::string varName, GLfloat v1, GLfloat v2, GLfloat v3 ) = 0;
	virtual bool SetUniformVar3f( std::string shaderName, std::string varName, GLfloat v[3] ) = 0;
	virtual bool SetUniformVar3f( GLuint shaderID, std::string varName, GLfloat v1, GLfloat v2, GLfloat v3 ) = 0;
	virtual bool SetUniformVar3f( GLuint shaderID, std::string varName, GLfloat [3] ) = 0;
	// glUniform3i
	virtual bool SetUniformVar3i( std::string shaderName, std::string varName, GLint v1, GLint v2, GLint v3 ) = 0;
	virtual bool SetUniformVar3i( std::string shaderName, std::string varName, GLint v[3] ) = 0;
	virtual bool SetUniformVar3i( GLuint shaderID, std::string varName, GLint v1, GLint v2, GLint v3 ) = 0;
	virtual bool SetUniformVar3i( GLuint shaderID, std::string varName, GLint v[3] ) = 0;
	// glUniform3ui
	virtual bool SetUniformVar3ui( std::string shaderName, std::string varName, GLuint v1, GLuint v2, GLuint v3 ) = 0;	// OpenGL 3.0
	virtual bool SetUniformVar3ui( std::string shaderName, std::string varName, GLuint v[3] ) = 0;	// OpenGL 3.0
	virtual bool SetUniformVar3ui( GLuint shaderID, std::string varName, GLuint v1, GLuint v2, GLuint v3 ) = 0;			// OpenGL 3.0
	virtual bool SetUniformVar3ui( GLuint shaderID, std::string varName, GLuint v[3] ) = 0;			// OpenGL 3.0


	//      _ _   _      _  __              _ ___  __
	// __ _| | | | |_ _ (_)/ _|___ _ _ _ __| | \ \/ /
	/// _` | | |_| | ' \| |  _/ _ \ '_| '  \_  _>  < 
	//\__, |_|\___/|_||_|_|_| \___/_| |_|_|_||_/_/\_\
	//|___/                                          
	// glUniform4f
	virtual bool SetUniformVar4f( std::string shaderName, std::string varName, GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4 ) = 0;
	virtual bool SetUniformVar4f( std::string shaderName, std::string varName, GLfloat v[4] ) = 0;
	virtual bool SetUniformVar4f( GLuint shaderID, std::string varName, GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4 ) = 0;
	virtual bool SetUniformVar4f( GLuint shaderID, std::string varName, GLfloat v[4] ) = 0;
	// glUniform4i
	virtual bool SetUniformVar4i( std::string shaderName, std::string varName, GLint v1, GLint v2, GLint v3, GLint v4 ) = 0;
	virtual bool SetUniformVar4i( std::string shaderName, std::string varName, GLint v[4] ) = 0;
	virtual bool SetUniformVar4i( GLuint shaderID, std::string varName, GLint v1, GLint v2, GLint v3, GLint v4 ) = 0;
	virtual bool SetUniformVar4i( GLuint shaderID, std::string varName, GLint v[4] ) = 0;
	// glUniform4ui
	virtual bool SetUniformVar4ui( std::string shaderName, std::string varName, GLuint v1, GLuint v2, GLuint v3, GLuint v4 ) = 0;		// OpenGL 3.0
	virtual bool SetUniformVar4ui( std::string shaderName, std::string varName, GLuint v[4] ) = 0;		// OpenGL 3.0
	virtual bool SetUniformVar4ui( GLuint shaderID, std::string varName, GLuint v1, GLuint v2, GLuint v3, GLuint v4 ) = 0;			// OpenGL 3.0
	virtual bool SetUniformVar4ui( GLuint shaderID, std::string varName, GLuint v[4] ) = 0;			// OpenGL 3.0

	virtual bool SetUniformMatrix4fv( std::string shaderName, std::string varName, GLsizei count, GLboolean transpose, const GLfloat* value ) = 0;
	virtual bool SetUniformMatrix4fv( GLuint shaderID, std::string varName, GLsizei count, GLboolean transpose, const GLfloat* value ) = 0;

	//PFNGLUNIFORM2IPROC				glUniform2i = 0;
	//PFNGLUNIFORM2UIPROC				glUniform2ui = 0;
};

#endif
