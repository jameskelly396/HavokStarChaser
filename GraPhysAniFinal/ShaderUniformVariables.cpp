#include "global.h"

#include <vector>
#include <fstream>
#include <iostream>	// for printing out error messages
#include <sstream>  // For the SetLightingUniforms() function

// Note: this file used to be called LoadPlyFileIntoGLBuffers

bool SetLightingUniforms(void);


bool g_SetShaderUniformVariables(void)
{
	// From the vertex shader
	::g_ShaderUniformVariables.matWorld_LocationID 
		= glGetUniformLocation( g_ShaderProgram_ID, "WorldMatrix");
	if ( ::g_ShaderUniformVariables.matWorld_LocationID == -1 )
	{
		//std::cout << "Warning: Can't find WorldMatrix uniform variable in shader program." << std::endl;
	}
	ExitOnGLError("ERROR: Could not get shader uniform locations");

	::g_ShaderUniformVariables.matView_LocationID = glGetUniformLocation(g_ShaderProgram_ID, "ViewMatrix");
	if ( ::g_ShaderUniformVariables.matView_LocationID == -1 )
	{
		//std::cout << "Warning: Can't find ViewMatrix uniform variable in shader program." << std::endl;
	}

	::g_ShaderUniformVariables.matProjection_LocationID = glGetUniformLocation(g_ShaderProgram_ID, "ProjectionMatrix");
	if ( ::g_ShaderUniformVariables.matProjection_LocationID == -1 )
	{
		//std::cout << "Warning: Can't find ProjectionMatrix uniform variable in shader program." << std::endl;
	}

	// From the Fragment shader
	::g_ShaderUniformVariables.Shininess_LocationID 
		= glGetUniformLocation(g_ShaderProgram_ID, "Shininess");
	if ( ::g_ShaderUniformVariables.matProjection_LocationID == -1 )
	{
		//std::cout << "Warning: Can't find Shininess uniform variable in shader program." << std::endl;
	}	

	::g_ShaderUniformVariables.Strength_LocationID = glGetUniformLocation(g_ShaderProgram_ID, "Shininess");
	if ( ::g_ShaderUniformVariables.Strength_LocationID == -1 )
	{
		//std::cout << "Warning: Can't find Shininess uniform variable in shader program." << std::endl;
	}	


	::g_ShaderUniformVariables.EyeDirection_LocationID = glGetUniformLocation(g_ShaderProgram_ID, "EyeDirection");
	if ( ::g_ShaderUniformVariables.EyeDirection_LocationID == -1 )
	{
		//std::cout << "Warning: Can't find EyeDirection uniform variable in shader program." << std::endl;
	}		
	
	// Set up the GAZILLION uniforms for the lighting
	// (This can be replaced with something called a "uniform block", which we'll look at later in the course
	if ( !SetLightingUniforms() )
	{
		//std::cout << "Warning: Can't find (one or more) Lights uniform variable in shader program." << std::endl;
	}		


	// Used to override the colour (for debugging)
	::g_ShaderUniformVariables.DebugObjectColourOverride_Locaiton_ID = glGetUniformLocation( g_ShaderProgram_ID , "DebugObjectColourOverride");
	if ( ::g_ShaderUniformVariables.DebugObjectColourOverride_Locaiton_ID == -1 )
	{
		//std::cout << "Warning: Can't find ObjectColourHACK uniform variable in shader program." << std::endl;
	}	
	

	// Added on September 29. 
	// Ask OpenGL the location of our uniform variable.
	//g_ObjectColourUniformLocation = glGetUniformLocation( g_ShaderProgram_ID, "objectColour" );

	////g_slot_LightPosition = glGetUniformLocation( g_ShaderIds[0], "LightPosition" );//uniform vec4 LightPosition;
	//g_slot_LightPosition = glGetUniformLocation( g_ShaderProgram_ID, "LightPosition" );//uniform vec4 LightPosition;
	////g_slot_LightColour   = glGetUniformLocation( g_ShaderIds[0], "LightColour" );	 //uniform vec4 LightColour; 
	//g_slot_LightColour   = glGetUniformLocation( g_ShaderProgram_ID, "LightColour" );	 //uniform vec4 LightColour; 
	////g_slot_attenuation   = glGetUniformLocation( g_ShaderIds[0], "attenuation" );  //uniform float attenuation;
	//g_slot_attenuation   = glGetUniformLocation( g_ShaderProgram_ID, "attenuation" );  //uniform float attenuation;
	ExitOnGLError("ERROR: Could not get shader uniform locations");

	return true;
}

// Note: this entire thing can be replaced by a different type of buffer (which we'll talk about later in the course)
bool bHasLightsBeenSet = false;
bool SetLightingUniforms(void)
{
	bool bNoErrors = true;
	if (bHasLightsBeenSet)
		return true;

	::g_ShaderUniformVariables.Lights[0].isEnabled_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[0].isEnabled");
	::g_ShaderUniformVariables.Lights[0].isLocal_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[0].isLocal");
	::g_ShaderUniformVariables.Lights[0].isSpot_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[0].isSpot");
	::g_ShaderUniformVariables.Lights[0].ambient_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[0].ambient");
	::g_ShaderUniformVariables.Lights[0].color_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[0].color");
	::g_ShaderUniformVariables.Lights[0].position_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[0].position");
	::g_ShaderUniformVariables.Lights[0].halfVector_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[0].halfVector");
	::g_ShaderUniformVariables.Lights[0].coneDirection_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[0].coneDirection");
	::g_ShaderUniformVariables.Lights[0].spotCosCutoff_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[0].spotCosCutoff");
	::g_ShaderUniformVariables.Lights[0].spotExponent_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[0].spotExponent");
	::g_ShaderUniformVariables.Lights[0].constantAttenuation_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[0].constantAttenuation");
	::g_ShaderUniformVariables.Lights[0].linearAttenuation_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[0].linearAttenuation");
	::g_ShaderUniformVariables.Lights[0].quadraticAttenuation_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[0].quadraticAttenuation");

	::g_ShaderUniformVariables.Lights[1].isEnabled_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[1].isEnabled");
	::g_ShaderUniformVariables.Lights[1].isLocal_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[1].isLocal");
	::g_ShaderUniformVariables.Lights[1].isSpot_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[1].isSpot");
	::g_ShaderUniformVariables.Lights[1].ambient_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[1].ambient");
	::g_ShaderUniformVariables.Lights[1].color_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[1].color");
	::g_ShaderUniformVariables.Lights[1].position_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[1].position");
	::g_ShaderUniformVariables.Lights[1].halfVector_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[1].halfVector");
	::g_ShaderUniformVariables.Lights[1].coneDirection_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[1].coneDirection");
	::g_ShaderUniformVariables.Lights[1].spotCosCutoff_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[1].spotCosCutoff");
	::g_ShaderUniformVariables.Lights[1].spotExponent_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[1].spotExponent");
	::g_ShaderUniformVariables.Lights[1].constantAttenuation_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[1].constantAttenuation");
	::g_ShaderUniformVariables.Lights[1].linearAttenuation_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[1].linearAttenuation");
	::g_ShaderUniformVariables.Lights[1].quadraticAttenuation_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[1].quadraticAttenuation");

	::g_ShaderUniformVariables.Lights[2].isEnabled_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[2].isEnabled");
	::g_ShaderUniformVariables.Lights[2].isLocal_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[2].isLocal");
	::g_ShaderUniformVariables.Lights[2].isSpot_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[2].isSpot");
	::g_ShaderUniformVariables.Lights[2].ambient_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[2].ambient");
	::g_ShaderUniformVariables.Lights[2].color_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[2].color");
	::g_ShaderUniformVariables.Lights[2].position_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[2].position");
	::g_ShaderUniformVariables.Lights[2].halfVector_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[2].halfVector");
	::g_ShaderUniformVariables.Lights[2].coneDirection_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[2].coneDirection");
	::g_ShaderUniformVariables.Lights[2].spotCosCutoff_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[2].spotCosCutoff");
	::g_ShaderUniformVariables.Lights[2].spotExponent_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[2].spotExponent");
	::g_ShaderUniformVariables.Lights[2].constantAttenuation_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[2].constantAttenuation");
	::g_ShaderUniformVariables.Lights[2].linearAttenuation_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[2].linearAttenuation");
	::g_ShaderUniformVariables.Lights[2].quadraticAttenuation_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[2].quadraticAttenuation");

	::g_ShaderUniformVariables.Lights[3].isEnabled_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[3].isEnabled");
	::g_ShaderUniformVariables.Lights[3].isLocal_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[3].isLocal");
	::g_ShaderUniformVariables.Lights[3].isSpot_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[3].isSpot");
	::g_ShaderUniformVariables.Lights[3].ambient_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[3].ambient");
	::g_ShaderUniformVariables.Lights[3].color_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[3].color");
	::g_ShaderUniformVariables.Lights[3].position_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[3].position");
	::g_ShaderUniformVariables.Lights[3].halfVector_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[3].halfVector");
	::g_ShaderUniformVariables.Lights[3].coneDirection_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[3].coneDirection");
	::g_ShaderUniformVariables.Lights[3].spotCosCutoff_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[3].spotCosCutoff");
	::g_ShaderUniformVariables.Lights[3].spotExponent_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[3].spotExponent");
	::g_ShaderUniformVariables.Lights[3].constantAttenuation_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[3].constantAttenuation");
	::g_ShaderUniformVariables.Lights[3].linearAttenuation_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[3].linearAttenuation");
	::g_ShaderUniformVariables.Lights[3].quadraticAttenuation_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[3].quadraticAttenuation");

	::g_ShaderUniformVariables.Lights[4].isEnabled_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[4].isEnabled");
	::g_ShaderUniformVariables.Lights[4].isLocal_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[4].isLocal");
	::g_ShaderUniformVariables.Lights[4].isSpot_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[4].isSpot");
	::g_ShaderUniformVariables.Lights[4].ambient_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[4].ambient");
	::g_ShaderUniformVariables.Lights[4].color_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[4].color");
	::g_ShaderUniformVariables.Lights[4].position_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[4].position");
	::g_ShaderUniformVariables.Lights[4].halfVector_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[4].halfVector");
	::g_ShaderUniformVariables.Lights[4].coneDirection_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[4].coneDirection");
	::g_ShaderUniformVariables.Lights[4].spotCosCutoff_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[4].spotCosCutoff");
	::g_ShaderUniformVariables.Lights[4].spotExponent_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[4].spotExponent");
	::g_ShaderUniformVariables.Lights[4].constantAttenuation_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[4].constantAttenuation");
	::g_ShaderUniformVariables.Lights[4].linearAttenuation_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[4].linearAttenuation");
	::g_ShaderUniformVariables.Lights[4].quadraticAttenuation_LocID = glGetUniformLocation(g_ShaderProgram_ID, "Lights[4].quadraticAttenuation");

	bHasLightsBeenSet = true;

	// ...  and so on...

	return bNoErrors;
}