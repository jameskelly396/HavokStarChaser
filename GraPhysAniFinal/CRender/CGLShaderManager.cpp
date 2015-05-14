// Written by Michael Feeney, Fanshawe College, 2010
// mfeeney@fanshawec.on.ca
// It may be distributed under the terms of the General Public License:
// http://www.fsf.org/licenses/gpl.html
// Use this code at your own risk. It is indented only as a learning aid.
//
#include "CGLShaderManager.h"
#include <sstream>
#include <fstream>
//#include "OpenGLExt.h"
#include <algorithm>	

CGLShaderManager::CGLShaderManager()
{
	this->m_currentShaderID = 0;
	this->m_lastShaderID = 0;
	return;
}

CGLShaderManager::~CGLShaderManager()
{
	return;
}
//

// If no vertex shader source, return false...
// Fragment and geometry shader is optional (i.e. no source = warning)
bool CGLShaderManager::CreateShaderProgramFromSource( CShaderProgramDescription &shaderProgDescription )
{
	if ( shaderProgDescription.vShader.source == "" )
	{	// No vertex source...
		std::stringstream ss;
		ss << "error: vertex shader '" << shaderProgDescription.vShader.name << "' has no source.";
		shaderProgDescription.vShader.vecShaderErrors.push_back( ss.str() );
		return false;	
	}

	// See if we've already added this shader... (check the name)
	if ( this->m_mapShaderName_to_ID.find( shaderProgDescription.name ) != this->m_mapShaderName_to_ID.end() )
	{	// We already have this shader loaded...
		

	}
		this->m_mapShaderName_to_ID[ shaderProgDescription.name ] = shaderProgDescription.ID;


	//
	shaderProgDescription.ID = ::glCreateProgram();
	
	//
	//__   __       _             ___ _            _         
	//\ \ / /__ _ _| |_ _____ __ / __| |_  __ _ __| |___ _ _ 
	// \ V / -_) '_|  _/ -_) \ / \__ \ ' \/ _` / _` / -_) '_|
	//  \_/\___|_|  \__\___/_\_\ |___/_||_\__,_\__,_\___|_|  
	//
	// Attempt to create and compile the vertex shader...
	shaderProgDescription.vShader.ID = ::glCreateShader( GL_VERTEX_SHADER );

	// Attach the shader(s) to the 'program' 
	::glAttachShader( shaderProgDescription.ID, shaderProgDescription.vShader.ID );
	// Get source as char array....
	GLcharARB* vShaderSource[1];
	vShaderSource[0] = (char *)shaderProgDescription.vShader.source.c_str();
	//...to set source...
	::glShaderSource( shaderProgDescription.vShader.ID,		// Handle to shader
					  1,					// Number of elements in string array (if using one big string, it's 1)
					  (const GLcharARB**) vShaderSource,	// Pointer to shader source (char array)
					  NULL);				// Length of array (0 or NULL if null terminated)
	// ...and compile.
	::glCompileShader( shaderProgDescription.vShader.ID );
	//
	// Did compile work?
	GLint statusOK; 
	::glGetObjectParameterivARB( shaderProgDescription.vShader.ID , GL_OBJECT_COMPILE_STATUS_ARB, &statusOK );
	if ( !statusOK )
	{
		shaderProgDescription.vShader.vecShaderErrors.push_back("Error: Could not compile vertex shader.");
		// This gets the 'last' error message for that shader (if there was one)
		GLcharARB infoLog[ GL_INFO_LOG_LENGTH ];	// defined in glext.h
		glGetInfoLogARB( shaderProgDescription.vShader.ID , GL_INFO_LOG_LENGTH, NULL, infoLog );
		std::stringstream ss;
		ss << infoLog << std::endl;
		shaderProgDescription.vShader.vecShaderErrors.push_back( ss.str() );
		return false;
	}
	//
	shaderProgDescription.vShader.bIsOK = true;


	// ___                            _     ___ _            _         
	//| __| _ __ _ __ _ _ __  ___ _ _| |_  / __| |_  __ _ __| |___ _ _ 
	//| _| '_/ _` / _` | '  \/ -_) ' \  _| \__ \ ' \/ _` / _` / -_) '_|
	//|_||_| \__,_\__, |_|_|_\___|_||_\__| |___/_||_\__,_\__,_\___|_|  
	//            |___/                                                
	//
	if ( shaderProgDescription.fShader.source != "" )
	{	//
		shaderProgDescription.fShader.ID = ::glCreateShader( GL_FRAGMENT_SHADER );

		// Attach the shader(s) to the 'program' 
		::glAttachShader( shaderProgDescription.ID, shaderProgDescription.fShader.ID );
		// Get source as char array....
		GLcharARB* fShaderSource[1];
		fShaderSource[0] = (char *)shaderProgDescription.fShader.source.c_str();
		//...to set source...
		::glShaderSource( shaderProgDescription.fShader.ID,		// Handle to shader
						  1,					// Number of elements in string array (if using one big string, it's 1)
						  (const GLcharARB**) fShaderSource,	// Pointer to shader source (char array)
						  NULL);				// Length of array (0 or NULL if null terminated)
		// ...and compile.
		::glCompileShader( shaderProgDescription.fShader.ID );
		//
		// Did compile work?
		GLint statusOK; 
		::glGetObjectParameterivARB( shaderProgDescription.fShader.ID , GL_OBJECT_COMPILE_STATUS_ARB, &statusOK );
		if ( !statusOK )
		{
			shaderProgDescription.fShader.vecShaderErrors.push_back("Error: Could not compile fragment shader.");
			// This gets the 'last' error message for that shader (if there was one)
			GLcharARB infoLog[ GL_INFO_LOG_LENGTH ];	// defined in glext.h
			glGetInfoLogARB( shaderProgDescription.fShader.ID , GL_INFO_LOG_LENGTH, NULL, infoLog );
			std::stringstream ss;
			ss << infoLog << std::endl;
			shaderProgDescription.fShader.vecShaderErrors.push_back( ss.str() );
			return false;
		}
		//
		shaderProgDescription.fShader.bIsOK = true;
	} // if ( shaderProgDescription.fShader.source != "" )

	//  ___                   _              ___ _            _         
	// / __|___ ___ _ __  ___| |_ _ _ _  _  / __| |_  __ _ __| |___ _ _ 
	//| (_ / -_) _ \ '  \/ -_)  _| '_| || | \__ \ ' \/ _` / _` / -_) '_|
	// \___\___\___/_|_|_\___|\__|_|  \_, | |___/_||_\__,_\__,_\___|_|  
	//                                |__/                              
	if ( shaderProgDescription.gShader.source != "" )
	{	//
		shaderProgDescription.gShader.ID = ::glCreateShader( GL_GEOMETRY_SHADER );

		// Attach the shader(s) to the 'program' 
		::glAttachShader( shaderProgDescription.ID, shaderProgDescription.gShader.ID );
		// Get source as char array....
		GLcharARB* gShaderSource[1];
		gShaderSource[0] = (char *)shaderProgDescription.gShader.source.c_str();
		//...to set source...
		::glShaderSource( shaderProgDescription.gShader.ID,		// Handle to shader
						  1,					// Number of elements in string array (if using one big string, it's 1)
						  (const GLcharARB**) gShaderSource,	// Pointer to shader source (char array)
						  NULL);				// Length of array (0 or NULL if null terminated)
		// ...and compile.
		::glCompileShader( shaderProgDescription.gShader.ID );
		//
		// Did compile work?
		GLint statusOK; 
		::glGetObjectParameterivARB( shaderProgDescription.gShader.ID , GL_OBJECT_COMPILE_STATUS_ARB, &statusOK );
		if ( !statusOK )
		{
			shaderProgDescription.gShader.vecShaderErrors.push_back("Error: Could not compile geometry shader.");
			// This gets the 'last' error message for that shader (if there was one)
			GLcharARB infoLog[ GL_INFO_LOG_LENGTH ];	// defined in glext.h
			glGetInfoLogARB( shaderProgDescription.gShader.ID , GL_INFO_LOG_LENGTH, NULL, infoLog );
			std::stringstream ss;
			ss << infoLog << std::endl;
			shaderProgDescription.gShader.vecShaderErrors.push_back( ss.str() );
			return false;
		}
		//
		shaderProgDescription.gShader.bIsOK = true;
	} // if ( shaderProgDescription.fShader.source != "" )

	// Now link the program...
	::glLinkProgramARB( shaderProgDescription.ID );
	// NOTE: This can still work, for instance if you have no shaders at all, but 
	//	will fail if there has been a complie error...
	::glGetObjectParameterivARB( shaderProgDescription.ID, GL_LINK_STATUS, &statusOK );
	if ( !statusOK )
	{
		std::stringstream ss;
		ss << "error: could not link shader program." << std::endl;
		GLcharARB infoLog[ GL_INFO_LOG_LENGTH ];	// defined in glext.h
		glGetInfoLogARB( shaderProgDescription.ID, GL_INFO_LOG_LENGTH, NULL, infoLog );
		ss << infoLog;
		shaderProgDescription.vecErrors.push_back( ss.str() );
		return false;
	}

	// Place the shader program into the container of shaders...
	//	( std::map< GLuint, CShaderProgramDescription > )
	this->m_mapShaderProgram[ shaderProgDescription.ID ] = shaderProgDescription;	
	// 
	//	( std::map< std::string, GLuint> m_ShaderName_to_MapID; )
	this->m_mapShaderName_to_ID[ shaderProgDescription.name ] = shaderProgDescription.ID;

	// At this point the shader is good to go...
	shaderProgDescription.bIsOK = true;
	// 
	
	// Now load up the atrribute and uniform variables...
	//	(Note that this 'shouldn't' fail if the compile and link was OK
	if ( !this->LoadActiveShaderVariables( shaderProgDescription.ID ) ) 
	{
		std::stringstream ss;
		ss << "Unexpected fatal error: could not load shader attribute descriptions for shader "
			<< shaderProgDescription.name << ".";
		this->m_LastError = ss.str();
		return false;
	}



	return true;
}

// **************************************************************************************************
// This loads the source from the files, then calls CreateShaderProgramFromSource...
// If there is a problem loading the file, returns false and loads the appropriate 
//	vector of errors with the error text...
// NOTES: 
//	* If vertex shader fails to load, returns false
//	* Fragment and Geometry shader is optional: if no filename specified, skips with warning
//	* If a file name IS specified for the Fragment and-or Geometry shader AND
//	  there is a problem loading it, returns false.
//
bool CGLShaderManager::CreateShaderProgramFromFile( CShaderProgramDescription &shaderProgDescription )
{
	//__   __       _             ___ _            _         
	//\ \ / /__ _ _| |_ _____ __ / __| |_  __ _ __| |___ _ _ 
	// \ V / -_) '_|  _/ -_) \ / \__ \ ' \/ _` / _` / -_) '_|
	//  \_/\___|_|  \__\___/_\_\ |___/_||_\__,_\__,_\___|_|  
	//
	// Attempt to load the vertex shader...
	if ( shaderProgDescription.vShader.filename == "")
	{	// No file name given
		std::stringstream ss;
		ss << "error: vertex shader '" << shaderProgDescription.vShader.name << "' has no filename specified.";
		shaderProgDescription.vShader.vecShaderErrors.push_back( ss.str() );
		return false;
	}
	// Has a file name, so attempt to load it...
	if ( !this->m_LoadShaderFromFile( shaderProgDescription.vShader.filename, shaderProgDescription.vShader.source ) )
	{	// Didn't load
		std::stringstream ss;
		ss << "error: Can't find vertex shader source file '" << shaderProgDescription.vShader.filename << "'";
		shaderProgDescription.vShader.vecShaderErrors.push_back( ss.str() );
		return false;
	}

	// NOTE: Fragment and geometry shader is optional... 
	//	If there is no file name specified, then just continue...
	// ___                            _     ___ _            _         
	//| __| _ __ _ __ _ _ __  ___ _ _| |_  / __| |_  __ _ __| |___ _ _ 
	//| _| '_/ _` / _` | '  \/ -_) ' \  _| \__ \ ' \/ _` / _` / -_) '_|
	//|_||_| \__,_\__, |_|_|_\___|_||_\__| |___/_||_\__,_\__,_\___|_|  
	//            |___/                                                
	//
	// Attempt to load the fragment shader...
	if ( shaderProgDescription.fShader.filename == "")
	{	// No file name given - GIVE WARNING (fragment) and continue...
		std::stringstream ss;
		ss << "warning: fragment shader '" << shaderProgDescription.fShader.name << "' has no filename specified.";
		shaderProgDescription.fShader.vecShaderErrors.push_back( ss.str() );
	}
	else
	{	// Has a file name, so attempt to load it...
		if ( !this->m_LoadShaderFromFile( shaderProgDescription.fShader.filename, shaderProgDescription.fShader.source ) )
		{	// Didn't load
			std::stringstream ss;
			ss << "error: Can't find fragment shader source file '" << shaderProgDescription.fShader.filename << "'";
			shaderProgDescription.fShader.vecShaderErrors.push_back( ss.str() );
			return false;
		}
	}			
	
	//  ___                   _              ___ _            _         
	// / __|___ ___ _ __  ___| |_ _ _ _  _  / __| |_  __ _ __| |___ _ _ 
	//| (_ / -_) _ \ '  \/ -_)  _| '_| || | \__ \ ' \/ _` / _` / -_) '_|
	// \___\___\___/_|_|_\___|\__|_|  \_, | |___/_||_\__,_\__,_\___|_|  
	//                                |__/                              
	//
	// Attempt to load the geometry shader...
	if ( shaderProgDescription.gShader.filename == "")
	{	// No file name given - GIVE WARNING (geometry) and continue...
		std::stringstream ss;
		ss << "warning: geometry shader '" << shaderProgDescription.gShader.name << "' has no filename specified.";
		shaderProgDescription.gShader.vecShaderErrors.push_back( ss.str() );
	}
	else
	{	// Has a file name, so attempt to load it...
		if ( !this->m_LoadShaderFromFile( shaderProgDescription.gShader.filename, shaderProgDescription.gShader.source ) )
		{	// Didn't load
			std::stringstream ss;
			ss << "error: Can't find geometry shader source file '" << shaderProgDescription.gShader.filename << "'";
			shaderProgDescription.gShader.vecShaderErrors.push_back( ss.str() );
			return false;
		}
	}			

	// All source loaded...now load the shader...	
	return this->CreateShaderProgramFromSource( shaderProgDescription );
}

GLuint CGLShaderManager::getCurrentShaderID(void)
{
	return this->m_currentShaderID;
}

GLuint CGLShaderManager::getLastShaderID(void)
{
	return this->m_lastShaderID;
}

std::string CGLShaderManager::getCurrentShaderName(void)
{
	for ( std::map< std::string, GLuint>::iterator itShader = this->m_mapShaderName_to_ID.begin();
		  itShader != this->m_mapShaderName_to_ID.end(); itShader++ )
	{
		if ( itShader->second == this->m_currentShaderID )
		{
			// It's the one
			return itShader->first;
		}
	}
	return "";	// No current shader
}

std::string CGLShaderManager::getLastShaderName(void)
	{
	for ( std::map< std::string, GLuint>::iterator itShader = this->m_mapShaderName_to_ID.begin();
		  itShader != this->m_mapShaderName_to_ID.end(); itShader++ )
	{
		if ( itShader->second == this->m_lastShaderID )
		{
			// It's the one
			return itShader->first;
		}
	}
	return "";	// No current shader
}

//bool CGLShaderManager::m_LoadShaderFromFile( std::string shaderFileName, std::string &shaderSource )
//{
//	std::ifstream theShaderFile( shaderFileName.c_str() );
//	if ( !theShaderFile.is_open() )
//	{
//		return false;
//	}
//	// File is open
//	std::stringstream ss;
//	std::string tempText;
//	while ( theShaderFile >> tempText )	
//	{
//		ss << tempText << " ";
//	}
//	shaderSource = ss.str();
//	return true;
//}


bool CGLShaderManager::m_LoadShaderFromFile( std::string shaderFileName, std::string &shaderSource )
{
	std::ifstream theShaderFile( shaderFileName.c_str() );
	if ( !theShaderFile.is_open() )
	{
		return false;
	}
	// File is open
	std::stringstream ss;
	std::string tempText;
	//const int MAX_BUFFERLENGTH = 1024;
	char buffer[CGLShaderManager::MAX_SHADER_CHAR_BUFFER_SIZE];	// Buffer

	while ( ! theShaderFile.eof() )	
	{
		// Clear buffer (likely not needed, but hey, can't hurt...)
		std::fill( buffer, buffer + CGLShaderManager::MAX_SHADER_CHAR_BUFFER_SIZE, ' ');
		theShaderFile.getline( buffer, CGLShaderManager::MAX_SHADER_CHAR_BUFFER_SIZE );
		ss << buffer << " \n";
	}
	shaderSource = ss.str();
	return true;
}


bool CGLShaderManager::UseShaderProgram( GLuint shaderID )
{
	// If zero (0) is passed, then just set it (i.e. "no" shader
	if ( shaderID == 0 )
	{
		::glUseProgram( 0 );
		this->m_lastShaderID = this->m_currentShaderID;
		this->m_currentShaderID = 0;
		return true;
	}

	// find it
	if ( this->m_mapShaderProgram.find( shaderID ) == this->m_mapShaderProgram.end() )
	{
		std::stringstream ss;
		ss << "Could not find shader program #" << shaderID;
		this->m_LastError = ss.str();
		return false;
	}
	//
	::glUseProgram( shaderID );
	this->m_lastShaderID = this->m_currentShaderID;
	this->m_currentShaderID = shaderID;

	return true;
}

bool CGLShaderManager::UseShaderProgram( std::string shaderName )
{
	// Is it there? 
	if ( this->m_mapShaderName_to_ID.find( shaderName ) == this->m_mapShaderName_to_ID.end() )
	{
		std::stringstream ss;
		ss << "Could not find '" << shaderName << "' shader program.";
		this->m_LastError = ss.str();
		return false;
	}
	GLuint shaderID = this->m_mapShaderName_to_ID[ shaderName ];
	// 
	::glUseProgram( shaderID );

	return true;
}

std::string CGLShaderManager::getLastError( void )
{
	return this->m_LastError;
}

//bool CGLShaderManager::GetShaderProgramInfo( std::string shaderName,  
//											 CShaderProgramDescription &shaderProgDescription )
//{
//	// Is it there? 
//	if ( this->m_mapShaderName_to_ID.find( shaderName ) == this->m_mapShaderName_to_ID.end() )
//	{
//		std::stringstream ss;
//		ss << "Could not find '" << shaderName << "' shader program.";
//		this->m_LastError = ss.str();
//		return false;
//	}
//	GLuint shaderID = this->m_mapShaderName_to_ID[ shaderName ];
//	shaderProgDescription = this->m_mapShaderProgram[ shaderID ];
//	return true;
//}
//
//bool CGLShaderManager::GetShaderProgramInfo( GLuint shaderID, 
//											 CShaderProgramDescription &shaderProgDescription )
//{
//	// find it
//	if ( this->m_mapShaderProgram.find( shaderID ) == this->m_mapShaderProgram.end() )
//	{
//		std::stringstream ss;
//		ss << "Could not find shader program #" << shaderID;
//		this->m_LastError = ss.str();
//		return false;
//	}
//	//
//	shaderProgDescription = this->m_mapShaderProgram[ shaderID ];
//
//	return true;
//}

bool CGLShaderManager::IsShaderOK( GLuint shaderProgramID )
{
	// Do we have this shader?
	if ( this->m_mapShaderProgram.find( shaderProgramID ) == this->m_mapShaderProgram.end() )
	{	// Nope.
		std::stringstream ss;
		ss << "Could not find '" << shaderProgramID << "' shader program.";
		this->m_LastError = ss.str();
		return false;
	}
	// Found it. Are all the contained shaders OK?
	CShaderProgramDescription shaderProg = this->m_mapShaderProgram[ shaderProgramID ];
	bool bProgIsOK = true;	// Assume true...
	if ( shaderProg.vShader.source != "" )
	{
		if ( !shaderProg.vShader.bIsOK )	{	bProgIsOK = false;	}
	}
	if ( shaderProg.gShader.source != "" )
	{
		if ( !shaderProg.gShader.bIsOK )	{	bProgIsOK = false;	}
	}
	if ( shaderProg.fShader.source != "" )
	{
		if ( !shaderProg.fShader.bIsOK )	{	bProgIsOK = false;	}
	}
	// Did program link?
	if ( ( ! shaderProg.bIsOK ) || ( ! bProgIsOK ) )
	{ 
		shaderProg.bIsOK = false;
	}

	return shaderProg.bIsOK;
}

bool CGLShaderManager::IsShaderOK( std::string shaderName )
{
	// Is it there? 
	if ( this->m_mapShaderName_to_ID.find( shaderName ) == this->m_mapShaderName_to_ID.end() )
	{
		std::stringstream ss;
		ss << "Could not find '" << shaderName << "' shader program.";
		this->m_LastError = ss.str();
		return false;
	}
	// Found it... 
	GLint shaderID = this->m_mapShaderName_to_ID[ shaderName ];
	
	return this->IsShaderOK( shaderID );
}


// Uses glGetActiveAttrib and glGetActiveUniform to determine the active variables in use in the shader
// BIG FAT NOTE: This is only the "active" ones - many times, they are profiled out by the compiler
bool CGLShaderManager::LoadActiveShaderVariables( std::string shaderName )
{
	if ( this->m_mapShaderName_to_ID.find( shaderName ) == this->m_mapShaderName_to_ID.end() )
	{
		std::stringstream ss;
		ss << "Could not find '" << shaderName << "' shader program.";
		this->m_LastError = ss.str();
		return false;
	}
	// Found that name, so load the ID...
	GLuint shaderID = this->m_mapShaderName_to_ID[ shaderName ];
	//
	return this->LoadActiveShaderVariables( shaderID ) ;
}

// Called after link is OK...
// Uses glGetActiveAttrib and glGetActiveUniform to determine the active variables in use in the shader
// BIG FAT NOTE: This is only the "active" ones - many times, they are profiled out by the compiler
//bool CGLShaderManager::m_LoadShaderVariableDescriptions( CShaderProgramDescription &shaderProgDescription )
bool CGLShaderManager::LoadActiveShaderVariables( GLuint shaderID )
{
	// See if the ID is valid...
	if ( this->m_mapShaderProgram.find( shaderID ) == this->m_mapShaderProgram.end() )
	{	// Nope.
		std::stringstream ss;
		ss << "Could not find '" << shaderID << "' shader program.";
		this->m_LastError = ss.str();
		return false;
	}

	// At this point, the shader ID is valid...
	//      _   _       _ _         _                      _      _    _        
	// __ _| |_| |_ _ _(_) |__ _  _| |_ ___  __ ____ _ _ _(_)__ _| |__| |___ ___
	/// _` |  _|  _| '_| | '_ \ || |  _/ -_) \ V / _` | '_| / _` | '_ \ / -_|_-<
	//\__,_|\__|\__|_| |_|_.__/\_,_|\__\___|  \_/\__,_|_| |_\__,_|_.__/_\___/__/
	//
	// Get number of active (i.e. linked and in use) attribute variables...
	GLint numActiveVars = -1;
	glGetProgramiv( shaderID, GL_ACTIVE_ATTRIBUTES, &numActiveVars );
	if ( numActiveVars > 0 )
	{	// Load the variables....
		GLchar	name[ CGLShaderManager::MAX_SHADER_CHAR_BUFFER_SIZE ];	// buffer to store attribute name
		GLsizei nameSizeReturned = 0;
		for ( GLuint index = 0; index != static_cast<GLuint>( numActiveVars); index++ )
		{
			CShaderAttributeDescription theAttrib;
			theAttrib.programID = shaderID;
			theAttrib.programName = this->m_mapShaderProgram[ shaderID ].name;
			glGetActiveAttrib( shaderID,	// Shader program ID
							   index,						// attribute variable index
							   CGLShaderManager::MAX_SHADER_CHAR_BUFFER_SIZE,						// buffer size
							   &nameSizeReturned,			// actual size of name in buffer (returned)
							   &(theAttrib.size),			// size of attribute
							   &(theAttrib.type),			// type (GLenum)			   
							   name );						// name (in character array)
			// Convert...
			theAttrib.name.append( name );
			// Decode the type...
			theAttrib.typeAsString = this->DecodeTypeNameFromGLenum( theAttrib.type ); 
			theAttrib.index = index;
			// Update the map of shaders with this new information
			this->m_mapShaderProgram[ shaderID ].mapShaderAttribs[ theAttrib.name ] = theAttrib;
		}
	}
	//	         _  __                              _      _    _        
	// _  _ _ _ (_)/ _|___ _ _ _ __   __ ____ _ _ _(_)__ _| |__| |___ ___
	//| || | ' \| |  _/ _ \ '_| '  \  \ V / _` | '_| / _` | '_ \ / -_|_-<
	// \_,_|_||_|_|_| \___/_| |_|_|_|  \_/\__,_|_| |_\__,_|_.__/_\___/__/
	//                                                                   
	// Get number of active (i.e. linked and in use) attribute variables...
	GLint numUniformVars = -1;
	glGetProgramiv( shaderID, GL_ACTIVE_UNIFORMS, &numUniformVars );
	if ( numUniformVars > 0 )
	{	// Load the variables....
		GLchar	name[ CGLShaderManager::MAX_SHADER_CHAR_BUFFER_SIZE ];	// buffer to store attribute name
		GLsizei nameSizeReturned = 0;
		for ( GLuint index = 0; index != static_cast<GLuint>( numUniformVars); index++ )
		{
			CShaderUniformDescription theUniform;
			theUniform.programID = shaderID;
			theUniform.programName = this->m_mapShaderProgram[ shaderID ].name;
			glGetActiveUniform( shaderID,	// Shader program ID
							    index,						// attribute variable index
							    CGLShaderManager::MAX_SHADER_CHAR_BUFFER_SIZE,						// buffer size
							    &nameSizeReturned,			// actual size of name in buffer (returned)
							    &(theUniform.size),			// size of attribute
							    &(theUniform.type),			// type (GLenum)			   
							    name );						// name (in character array)
			// Convert...
			theUniform.name.append( name );
			// Decode the type...
			theUniform.typeAsString = this->DecodeTypeNameFromGLenum( theUniform.type ); 
			theUniform.index = index;
			// Update the map of shaders with this new information
			this->m_mapShaderProgram[ shaderID ].mapShaderUniforms[ theUniform.name ] = theUniform;
		}
	}

	return true;
}

GLint CGLShaderManager::GetShaderIDFromName( std::string shaderName )
{
	if ( this->m_mapShaderName_to_ID.find( shaderName ) == this->m_mapShaderName_to_ID.end() )
	{
		return -1;
	}
	// Found that name, so load the ID...
	return this->m_mapShaderName_to_ID[ shaderName ];
}


bool CGLShaderManager::GetShaderProgramInfo( CShaderProgramDescription &shaderProgDescription )
{
	// If shaderProgramID != 0, look for that ID...
	if ( shaderProgDescription.ID == 0 )
	{	// See if there is a match to the program name...
		if ( this->m_mapShaderName_to_ID.find( shaderProgDescription.name ) == this->m_mapShaderName_to_ID.end() )
		{
			std::stringstream ss;
			ss << "Could not find '" << shaderProgDescription.name << "' shader program.";
			this->m_LastError = ss.str();
			return false;
		}
		// Found that name, so load the ID...
		shaderProgDescription.ID = this->m_mapShaderName_to_ID[ shaderProgDescription.name ];
	}
	else
	{	// See if we have that ID...
		if ( this->m_mapShaderProgram.find( shaderProgDescription.ID ) == this->m_mapShaderProgram.end() )
		{	// Nope.
			std::stringstream ss;
			ss << "Could not find '" << shaderProgDescription.ID << "' shader program.";
			this->m_LastError = ss.str();
			return false;
		}
		// We have that ID, so load the shader program name
		shaderProgDescription.name = this->m_mapShaderProgram[ shaderProgDescription.ID ].name;
	}
	// At this point, the shader ID (and program name) are both valid...
	shaderProgDescription = this->m_mapShaderProgram[ shaderProgDescription.ID ];

	return true;
}



std::string CGLShaderManager::DecodeTypeNameFromGLenum( GLenum type )
{
	switch ( type )
	{
	case GL_FLOAT:
		return "GL_FLOAT";			break;
	case GL_FLOAT_VEC2:
		return "GL_FLOAT_VEC2";		break;
	case GL_FLOAT_VEC3:
		return "GL_FLOAT_VEC3";		break;
	case GL_FLOAT_VEC4:
		return "GL_FLOAT_VEC4";		break;
	case GL_FLOAT_MAT2:
		return "GL_FLOAT_MAT2";		break;
	case GL_FLOAT_MAT3:
		return "GL_FLOAT_MAT3";		break;
	case GL_FLOAT_MAT4:
		return "GL_FLOAT_MAT4";		break;
	case GL_FLOAT_MAT2x3:	// OpenGL 2.1 only
		return "GL_FLOAT_MAT2x3";	break;
	case GL_FLOAT_MAT2x4:	// OpenGL 2.1 only
		return "GL_FLOAT_MAT2x4";	break;
	case GL_FLOAT_MAT3x2:	// OpenGL 2.1 only 
		return "GL_FLOAT_MAT3x2";	break;
	case GL_FLOAT_MAT3x4:	// OpenGL 2.1 only 
		return "GL_FLOAT_MAT3x4";	break;
	case GL_FLOAT_MAT4x2:	// OpenGL 2.1 only
		return "GL_FLOAT_MAT4x2";	break;
	case GL_FLOAT_MAT4x3:	// OpenGL 2.1 only
		return "GL_FLOAT_MAT4x3";	break;
	}
	return "GL_ID_UNKNOWN";
}


