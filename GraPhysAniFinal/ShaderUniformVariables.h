#ifndef _ShaderUniformVariables_HG_
#define _ShaderUniformVariables_HG_

#include <GL\glew.h>			// Because it complains when we don't include it
#include <GL\freeglut.h>		// Because of the GLuint

#define GLM_FORCE_CXX98    
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp> // glm::mat4


// From Fragment shader:
//struct LightProperties
//{
//	bool isEnabled;
//	bool isLocal;
//	bool isSpot;
//	vec3 ambient;
//	vec3 color;
//	vec3 position;
//	vec3 halfVector;
//	vec3 coneDirection;
//	float spotCosCutoff;
//	float spotExponent;
//	float constantAttenuation;
//	float linearAttenuation;
//	float quadraticAttenuation;
//};

struct CLightProperties			//struct LightProperties
{
public:
	CLightProperties() {}; 
		//isEnabled(GL_FALSE), isLocal(GL_FALSE), isSpot(GL_FALSE),
		//spotCosCutoff(0.0f), spotExponent(0.0f), 
		//constantAttenuation(0.0f), linearAttenuation(0.0f), quadraticAttenuation(0.0f) {};
			
	GLuint isEnabled_LocID;				/*GLboolean isEnabled;*/		//	bool isEnabled;
	GLuint isLocal_LocID;				/*GLboolean isLocal;*/			//	bool isLocal;
	GLuint isSpot_LocID;				/*GLboolean isSpot;*/						//	bool isSpot;
	GLuint ambient_LocID;				/*glm::vec3 ambient;*/						//	vec3 ambient;
	GLuint color_LocID;					/*glm::vec3 color;*/							//	vec3 color;
	GLuint position_LocID;				/*glm::vec3 position;*/						//	vec3 position;
	GLuint halfVector_LocID;			/*glm::vec3 halfVector;*/					//	vec3 halfVector;
	GLuint coneDirection_LocID;			/*glm::vec3 coneDirection;*/				//	vec3 coneDirection;
	GLuint spotCosCutoff_LocID;			/*GLfloat spotCosCutoff;*/					//	float spotCosCutoff;
	GLuint spotExponent_LocID;			/*GLfloat spotExponent;*/					//	float spotExponent;
	GLuint constantAttenuation_LocID;	/*GLfloat constantAttenuation;*/		//	float constantAttenuation;
	GLuint linearAttenuation_LocID;		/*GLfloat linearAttenuation;*/				//	float linearAttenuation;
	GLuint quadraticAttenuation_LocID;	/*GLfloat quadraticAttenuation;*/		//	float quadraticAttenuation;
};

const int MaxLights = 10;

struct CShaderUniformVariables
{
	// From Vertex shader:
	glm::mat4 matProjection;				// uniform mat4 ProjectionMatrix;
	GLuint matProjection_LocationID;		// GLuint g_ProjectionMatrixUniformLocation;			

	glm::mat4 matView;						// uniform mat4 ViewMatrix;
	GLuint matView_LocationID;				// GLuint g_ViewMatrixUniformLocation;		

	glm::mat4 matWorld;						// uniform mat4 ModelMatrix;  
	GLuint matWorld_LocationID;				// GLuint g_ModelMatrixUniformLocation;		

	// From Fragment shader:
	//GLfloat Shininess;						
	GLuint Shininess_LocationID;			// uniform float Shininess;

	//GLfloat Strength;						
	GLuint Strength_LocationID;				// uniform float Strength;

	//glm::vec3 EyeDirection;					
	GLuint EyeDirection_LocationID;			// uniform vec3 EyeDirection

	static const int MAXLIGHTS = 2;		// --HAS-- to match the shader   //Also change max light value in CLightManager.h and OpenGL.LightTexSkyUber.fragment_texture.glsl
	CLightProperties Lights[MAXLIGHTS];		// uniform LightProperties Lights[MaxLights];

	// ******************************************************************
	// ********* Can override the colour on screen (for debugging) ******
	GLuint DebugObjectColourOverride_Locaiton_ID;	// uniform vec4 DebugObjectColourOverride;	// If the w is != 0, this colour is used
};


bool g_SetShaderUniformVariables(void);		

//
//// the set of lights to apply, per invocation of this shader
//const int MaxLights = 10;
//uniform LightProperties Lights[MaxLights];
//
//uniform float Shininess;
//uniform float Strength;
//uniform vec3 EyeDirection;


// From Vertex shader:
//
		
//extern GLuint g_ObjectColourUniformLocation;				
//
//extern GLuint g_slot_LightPosition;	//uniform vec4 LightPosition;	
//extern GLuint g_slot_LightColour;	//uniform vec4 LightColour;		
//extern GLuint g_slot_attenuation;	//uniform float attenuation;	

#endif
