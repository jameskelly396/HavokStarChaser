// Written by Michael Feeney, Fanshawe College, 2010
// mfeeney@fanshawec.on.ca
// It may be distributed under the terms of the General Public License:
// http://www.fsf.org/licenses/gpl.html
// Use this code at your own risk. It is indented only as a learning aid.
//
#ifndef _CGLShaderManager_HG_
#define _CGLShaderManager_HG_

#include "IGLShaderManager.h"
// ***************************************************************
// ** Objects that use this should include the IGLShaderManager **
// ** Objects that need to create this (should only be one),    **
// **   should include the CGLShaderManager.                    **
// ***************************************************************

#include <map>

class CGLShaderManager : public IGLShaderManager
{
public:
	CGLShaderManager();
	virtual ~CGLShaderManager();
	//
	virtual bool CreateShaderProgramFromSource( CShaderProgramDescription &shaderProgDescription );
	virtual bool CreateShaderProgramFromFile( CShaderProgramDescription &shaderProgDescription );

	virtual bool UseShaderProgram( GLuint shaderID );
	virtual bool UseShaderProgram( std::string shaderName );

	// Returns zero (0) and blank string if no valid shader exists
	virtual GLuint getCurrentShaderID(void);
	virtual GLuint getLastShaderID(void);
	virtual std::string getCurrentShaderName(void);
	virtual std::string getLastShaderName(void);

	virtual bool GetShaderProgramInfo( CShaderProgramDescription &shaderProgDescription );

	// Uses glGetActiveAttrib and glGetActiveUniform to determine the active variables in use in the shader
	// BIG FAT NOTE: This is only the "active" ones - many times, they are profiled out by the compiler
	virtual bool LoadActiveShaderVariables( GLuint shaderID );
	virtual bool LoadActiveShaderVariables( std::string shaderName );

	virtual GLint GetShaderIDFromName( std::string shaderName );

	virtual bool IsShaderOK( GLuint shaderProgramID );
	virtual bool IsShaderOK( std::string shaderName );

	//virtual bool GetAttributeInfo( CShaderAttributeDescription &attributeDescription );

	virtual std::string DecodeTypeNameFromGLenum( GLenum type );

	virtual std::string getLastError( void );

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
	virtual bool SetUniformVar1f( std::string shaderName, std::string varName, GLfloat value );			
	virtual bool SetUniformVar1f( GLuint shaderID, std::string varName, GLfloat value );	
	// glUniform1i...
	virtual bool SetUniformVar1i( std::string shaderName, std::string varName, GLint value );
	virtual bool SetUniformVar1i( GLuint shaderID, std::string varName, GLint value );
	// glUniform1ui... (OpenGL 3.0)
	virtual bool SetUniformVar1ui( std::string shaderName, std::string varName, GLint value );	// OpenGL 3.0
	virtual bool SetUniformVar1ui( GLuint shaderID, std::string varName, GLint value );			// OpenGL 3.0

	//      _ _   _      _  __               _____  __
	// __ _| | | | |_ _ (_)/ _|___ _ _ _ __ |_  ) \/ /
	/// _` | | |_| | ' \| |  _/ _ \ '_| '  \ / / >  < 
	//\__, |_|\___/|_||_|_|_| \___/_| |_|_|_/___/_/\_\
	//|___/                                           
	// glUniform2f
	virtual bool SetUniformVar2f( std::string shaderName, std::string varName, GLfloat v1, GLfloat v2 );
	virtual bool SetUniformVar2f( std::string shaderName, std::string varName, GLfloat v[2] );
	virtual bool SetUniformVar2f( GLuint shaderID, std::string varName, GLfloat v1, GLfloat v2 );
	virtual bool SetUniformVar2f( GLuint shaderID, std::string varName, GLfloat v[2] );

	// glUniform2i
	virtual bool SetUniformVar2i( std::string shaderName, std::string varName, GLint v1, GLint v2 );
	virtual bool SetUniformVar2i( std::string shaderName, std::string varName, GLint v[2] );
	virtual bool SetUniformVar2i( GLuint shaderID, std::string varName, GLint v1, GLint v2 );
	virtual bool SetUniformVar2i( GLuint shaderID, std::string varName, GLint v[2] );
	// glUniform2i
	virtual bool SetUniformVar2ui( std::string shaderName, std::string varName, GLuint v1, GLuint v2 );	// OpenGL 3.0 - shader version 130
	virtual bool SetUniformVar2ui( std::string shaderName, std::string varName, GLuint v[2] );	// OpenGL 3.0 - shader version 130
	virtual bool SetUniformVar2ui( GLuint shaderID, std::string varName, GLuint v1, GLuint v2 );		// OpenGL 3.0 - shader version 130
	virtual bool SetUniformVar2ui( GLuint shaderID, std::string varName, GLuint v[2] );		// OpenGL 3.0 - shader version 130

	//      _ _   _      _  __               ______  __
	// __ _| | | | |_ _ (_)/ _|___ _ _ _ __ |__ /\ \/ /
	/// _` | | |_| | ' \| |  _/ _ \ '_| '  \ |_ \ >  < 
	//\__, |_|\___/|_||_|_|_| \___/_| |_|_|_|___//_/\_\
	//|___/                                            
	// glUniform3f
	virtual bool SetUniformVar3f( std::string shaderName, std::string varName, GLfloat v1, GLfloat v2, GLfloat v3 );
	virtual bool SetUniformVar3f( std::string shaderName, std::string varName, GLfloat v[3] );
	virtual bool SetUniformVar3f( GLuint shaderID, std::string varName, GLfloat v1, GLfloat v2, GLfloat v3 );
	virtual bool SetUniformVar3f( GLuint shaderID, std::string varName, GLfloat v[3] );
	// glUniform3i
	virtual bool SetUniformVar3i( std::string shaderName, std::string varName, GLint v1, GLint v2, GLint v3 );
	virtual bool SetUniformVar3i( std::string shaderName, std::string varName, GLint v[3] );
	virtual bool SetUniformVar3i( GLuint shaderID, std::string varName, GLint v1, GLint v2, GLint v3 );
	virtual bool SetUniformVar3i( GLuint shaderID, std::string varName, GLint v[3] );
	// glUniform3ui
	virtual bool SetUniformVar3ui( std::string shaderName, std::string varName, GLuint v1, GLuint v2, GLuint v3 );	// OpenGL 3.0 - shader version 130
	virtual bool SetUniformVar3ui( std::string shaderName, std::string varName, GLuint v[3] );	// OpenGL 3.0 - shader version 130
	virtual bool SetUniformVar3ui( GLuint shaderID, std::string varName, GLuint v1, GLuint v2, GLuint v3 );			// OpenGL 3.0 - shader version 130
	virtual bool SetUniformVar3ui( GLuint shaderID, std::string varName, GLuint v[3] );			// OpenGL 3.0 - shader version 130


	//      _ _   _      _  __              _ ___  __
	// __ _| | | | |_ _ (_)/ _|___ _ _ _ __| | \ \/ /
	/// _` | | |_| | ' \| |  _/ _ \ '_| '  \_  _>  < 
	//\__, |_|\___/|_||_|_|_| \___/_| |_|_|_||_/_/\_\
	//|___/                                          
	// glUniform4f
	virtual bool SetUniformVar4f( std::string shaderName, std::string varName, GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4 );
	virtual bool SetUniformVar4f( std::string shaderName, std::string varName, GLfloat v[4] );
	virtual bool SetUniformVar4f( GLuint shaderID, std::string varName, GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4 );
	virtual bool SetUniformVar4f( GLuint shaderID, std::string varName, GLfloat v[4] );
	// glUniform4i
	virtual bool SetUniformVar4i( std::string shaderName, std::string varName, GLint v1, GLint v2, GLint v3, GLint v4 );
	virtual bool SetUniformVar4i( std::string shaderName, std::string varName, GLint v[4] );
	virtual bool SetUniformVar4i( GLuint shaderID, std::string varName, GLint v1, GLint v2, GLint v3, GLint v4 );
	virtual bool SetUniformVar4i( GLuint shaderID, std::string varName, GLint v[4] );
	// glUniform4ui
	virtual bool SetUniformVar4ui( std::string shaderName, std::string varName, GLuint v1, GLuint v2, GLuint v3, GLuint v4 );	// OpenGL 3.0 - shader version 130
	virtual bool SetUniformVar4ui( std::string shaderName, std::string varName, GLuint v[4] );	// OpenGL 3.0 - shader version 130
	virtual bool SetUniformVar4ui( GLuint shaderID, std::string varName, GLuint v1, GLuint v2, GLuint v3, GLuint v4 );			// OpenGL 3.0 - shader version 130
	virtual bool SetUniformVar4ui( GLuint shaderID, std::string varName, GLuint v[4] );			// OpenGL 3.0 - shader version 130

	virtual bool SetUniformMatrix4fv( std::string shaderName, std::string varName, GLsizei count, GLboolean transpose, const GLfloat* value );
	virtual bool SetUniformMatrix4fv( GLuint shaderID, std::string varName, GLsizei count, GLboolean transpose, const GLfloat* value );

	//PFNGLUNIFORM2FPROC				glUniform2f = 0;
	//PFNGLUNIFORM2IPROC				glUniform2i = 0;
	//PFNGLUNIFORM2UIPROC				glUniform2ui = 0;

private:
	std::map< GLuint, CShaderProgramDescription >	m_mapShaderProgram;			// Holds the shader information
	std::map< std::string, GLuint>					m_mapShaderName_to_ID;		// Used by UseShaderProgram (by name)

	bool m_LoadShaderFromFile( std::string shaderFileName, std::string &shaderSource );

	std::string m_LastError;

	GLuint m_currentShaderID;
	GLuint m_lastShaderID;

	static const unsigned int MAX_SHADER_CHAR_BUFFER_SIZE = 4096;
};



#endif
