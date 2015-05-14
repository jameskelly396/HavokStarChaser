#include "global.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

void glutReshape_callback(int Width, int Height)
{
	::g_screenWidth = Width;	// CurrentWidth = Width;
	::g_screenHeight = Height;	// CurrentHeight = Height;

	//glViewport(0, 0, CurrentWidth, CurrentHeight);
	glViewport(0, 0, ::g_screenWidth, ::g_screenHeight);


	//ProjectionMatrix =
	//  CreateProjectionMatrix(
	//    60,
	//    (float)CurrentWidth / CurrentHeight,
	//    1.0f,
	//    100.0f
	//  );

	::g_ShaderUniformVariables.matProjection = glm::mat4(1.0f);
	//g_matProjection = glm::perspective( glm::radians(45.0f), (float)CurrentWidth / CurrentHeight, 0.1f, 100.f);
	::g_ShaderUniformVariables.matProjection 
		= glm::perspective( glm::radians(45.0f), 
		(float)::g_screenWidth / ::g_screenHeight, 
		0.5f,		// Near plane
		// ** CHANGED THIS TO FIT SKYBOX **
		10000.f);		// Far plane

	//glUseProgram(g_ShaderIds[0]);
	//glUseProgram( g_ShaderProgram_ID );
	GLuint uberShaderID = ::g_pShaderManager->GetShaderIDFromName("UberShader");
	if ( !::g_pShaderManager->UseShaderProgram( uberShaderID ) )
	{
		return;
	}

	CShaderProgramDescription shader;
	shader.ID = uberShaderID;
	::g_pShaderManager->GetShaderProgramInfo(shader);

	//GLuint matProjID = shader.getUniformVarIDFromName( "ProjectionMatrix" );
	//glUniformMatrix4fv( matProjID, 1, GL_FALSE, glm::value_ptr(::g_ShaderUniformVariables.matProjection));

	::g_pShaderManager->SetUniformMatrix4fv( uberShaderID, "ProjectionMatrix", 1, GL_FALSE, 
		                                     glm::value_ptr(::g_ShaderUniformVariables.matProjection) );



	glUseProgram(0);


	return;
}
