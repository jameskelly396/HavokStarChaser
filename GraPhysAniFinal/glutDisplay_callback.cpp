#include "global.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "VertexTypes.h"		// For CVertex_fXYZ_fRGB, etc.

#include "CGameObjects/IGameObjectRender.h"

#include <iostream>
#include <sstream>

void SetLightingUniformIDs(GLuint shaderID);	// USES shadermanager
void CopyLigtDataToShader(GLuint shaderID);		// USES shadermanager


float Radians_to_Degrees(float radians)
{
	return radians * (180 / 3.14159265f);
}


void DrawMeshForShadow( CMeshDesc curMesh, glm::mat4 &matView, GLuint lightNumber )
{
	glm::mat4 matWorld(1.0f);


	matWorld = glm::translate( matWorld, glm::vec3( curMesh.relPosition.x, curMesh.relPosition.y, curMesh.relPosition.z) );

	// 
	// Added on October 16th, 2014
	// "Imposters" (usually 2D "flat" quads) often "face" the camera, so you only see one side.
	// This code will face the camera, but only aligned with the x-z plane (so ignoring y).
	// In other words, if the camera goes "above" the imposter, it WON'T face it.
	if ( curMesh.bIsImposter )
	{
		// http://www.rastertek.com/dx11tut34.html
		// 	// Calculate the rotation that needs to be applied to the billboard model to face the current camera position using the arc tangent function.
		// angle = atan2(modelPosition.x - cameraPosition.x, 
		//               modelPosition.z - cameraPosition.z) 
		//			     * (180.0 / D3DX_PI);

		// Convert rotation into radians.
		// rotation = (float)angle * 0.0174532925f

		float angle = atan2( curMesh.relPosition.x - ::g_pCamera->eye.x, 
							 curMesh.relPosition.z - ::g_pCamera->eye.z );
								//* ( 180.0 / PI );
		// Since we aren't paying any attention to the x and z rotations.
		//pObjectToDraw->rotation.y = angle;
		curMesh.orientation = glm::fquat( glm::vec3( 0.0f, angle, 0.0f ) );
	}
	
	glm::mat4 matRotation = glm::toMat4( curMesh.orientation );

	matWorld = matWorld * matRotation; 

	matWorld = glm::scale( matWorld, glm::vec3( curMesh.scale.x, 
												curMesh.scale.y, 
												curMesh.scale.z) );

	glCullFace( GL_BACK );


	GLuint uberShaderID = ::g_pShaderManager->GetShaderIDFromName("UberShader");
	if ( !::g_pShaderManager->UseShaderProgram( uberShaderID ) )
	{
		std::cout << "Can't switch to shader " << uberShaderID << std::endl;
		return;
	}


	// ___  _              _               __  __             
	/// __|| |_   __ _  __| | ___ __ __ __|  \/  | __ _  _ __ 
	//\__ \| ' \ / _` |/ _` |/ _ \\ V  V /| |\/| |/ _` || '_ \
	//|___/|_||_|\__,_|\__,_|\___/ \_/\_/ |_|  |_|\__,_|| .__/
	//                                                  |_|   

	::g_pShaderManager->SetUniformVar1i( uberShaderID, "bGenShadowFromLight", GL_TRUE );

	::g_pShaderManager->SetUniformMatrix4fv( uberShaderID, "WorldMatrix", 1, GL_FALSE, glm::value_ptr(matWorld) );
	ExitOnGLError("ERROR: Could not use the shader program");

	::g_pShaderManager->SetUniformMatrix4fv( uberShaderID, "ViewMatrix", 1, GL_FALSE, glm::value_ptr(matView) );


	ExitOnGLError("ERROR: Could not set the shader uniforms");

	// Based on the object "ply file" name, point to the appropriate buffer

	int numberOfIndicesToRender = 0;

	CPlyInfo tempPlyInfo;
	std::string plyFileNameDEBUG = curMesh.modelName;
	if ( !::g_pModelLoader->GetRenderingInfoByModelFileName( curMesh.modelName, 
																tempPlyInfo ) )
	{	// Model isn't present, which is a Very Bad Thing
		return;	// in a for loop, this will go to the next count, skipping everyint
	}
	// At this point, we have found a valid model (that was loaded)
	glBindVertexArray( tempPlyInfo.vertexBufferObjectID );					//  g_VertexPositionBufferID[1] );		// "Connects" to a vertex buffer
	glBindBuffer( GL_ARRAY_BUFFER, tempPlyInfo.vertexAttribBufferID );		//  g_VertexColourBufferID[1] );		// Also buffer (with colours)
	numberOfIndicesToRender = tempPlyInfo.numberOfElements * 3;			// g_numberOfTriangles[1] * 3; 
		

	glEnableVertexAttribArray(0);			// Position
	glEnableVertexAttribArray(1);			// Colour
	glEnableVertexAttribArray(2);			// Normal
	glEnableVertexAttribArray(3);			// Texture
	ExitOnGLError("ERROR: Could not enable vertex attributes");

	ExitOnGLError("ERROR: Could not set VAO attributes");

	glDrawElements(GL_TRIANGLES,  
					numberOfIndicesToRender, 
					GL_UNSIGNED_INT, (GLvoid*)0 );
	ExitOnGLError("ERROR: Could not draw the cube");
	// *** END of DRAW THE OBJECT FROM THE BUFFER ****

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);

	return;

}

void DrawMesh(CMeshDesc curMesh, glm::mat4 &matView)
{
	glm::mat4 matWorld(1.0f);


	matWorld = glm::translate( matWorld, glm::vec3( curMesh.relPosition.x, curMesh.relPosition.y, curMesh.relPosition.z) ); 

	// 
	// Added on October 16th, 2014
	// "Imposters" (usually 2D "flat" quads) often "face" the camera, so you only see one side.
	// This code will face the camera, but only aligned with the x-z plane (so ignoring y).
	// In other words, if the camera goes "above" the imposter, it WON'T face it.
	if ( curMesh.bIsImposter )
	{
		// http://www.rastertek.com/dx11tut34.html
		// 	// Calculate the rotation that needs to be applied to the billboard model to face the current camera position using the arc tangent function.
		// angle = atan2(modelPosition.x - cameraPosition.x, 
		//               modelPosition.z - cameraPosition.z) 
		//			     * (180.0 / D3DX_PI);

		// Convert rotation into radians.
		// rotation = (float)angle * 0.0174532925f

		float angle = atan2( curMesh.relPosition.x - ::g_pCamera->eye.x, 
							 curMesh.relPosition.z - ::g_pCamera->eye.z );
								//* ( 180.0 / PI );
		// Since we aren't paying any attention to the x and z rotations.
		//pObjectToDraw->rotation.y = angle;
		curMesh.orientation = glm::fquat( glm::vec3( 0.0f, angle, 0.0f ) );
	}
	
	// UPDATE*************************************************
	if (!curMesh.bIsHavok){
		glm::mat4 matRotation = glm::toMat4(curMesh.orientation);
		matWorld = matWorld * matRotation;
	}
	else
		matWorld = curMesh.matrix;

	matWorld = glm::scale( matWorld, glm::vec3( curMesh.scale.x, 
												curMesh.scale.y, 
												curMesh.scale.z) );
	
		
	if ( curMesh.bIsWireframe )
	{
		glPolygonMode( GL_FRONT_AND_BACK ,  GL_LINE );
	}
	else
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}
	ExitOnGLError("What the what?");

	glCullFace( GL_BACK );


	GLuint uberShaderID = ::g_pShaderManager->GetShaderIDFromName("UberShader");
	if ( !::g_pShaderManager->UseShaderProgram( uberShaderID ) )
	{
		std::cout << "Can't switch to shader " << uberShaderID << std::endl;
		return;
	}

	SetLightingUniformIDs( uberShaderID );
	CopyLigtDataToShader( uberShaderID );

	// Set the "EyeDirection" (aka the camera)
	::g_pShaderManager->SetUniformVar3f( uberShaderID, "EyeDirection", ::g_pCamera->eye.x, ::g_pCamera->eye.y, ::g_pCamera->eye.z );


	// Shininess and Strength...
	::g_pShaderManager->SetUniformVar1f( uberShaderID, "Shininess", curMesh.specularShininess );
	::g_pShaderManager->SetUniformVar1f( uberShaderID, "Strength", curMesh.specularStrength );
	
	// Add the quick-n-dirty "IsFlame" variable
	if (curMesh.bIsParticle)
		::g_pShaderManager->SetUniformVar1i(uberShaderID, "bIsParticle", GL_TRUE);
	else
		::g_pShaderManager->SetUniformVar1i(uberShaderID, "bIsParticle", GL_FALSE);




	// ___  _              _               __  __             
	/// __|| |_   __ _  __| | ___ __ __ __|  \/  | __ _  _ __ 
	//\__ \| ' \ / _` |/ _` |/ _ \\ V  V /| |\/| |/ _` || '_ \
	//|___/|_||_|\__,_|\__,_|\___/ \_/\_/ |_|  |_|\__,_|| .__/
	//                                                  |_|   

	// Render normally
	::g_pShaderManager->SetUniformVar1i( uberShaderID, "bGenShadowFromLight", GL_FALSE );

	unsigned int texCount = 0;
	for (std::map<unsigned int, std::vector<std::string> >::iterator itr = g_mapTextures.begin(); itr != g_mapTextures.end(); ++itr)
	{
		if (itr->second[0] == "Skybox")
		{
			::g_pShaderManager->SetUniformVar1i(uberShaderID, "skyMapTexture", 0);

			CTextureFromBMP* pTexture = ::g_p_TextureManager->getTextureFromTextureName(itr->second[0]);
			glActiveTexture(pTexture->getTextureUnit());
			GLuint textureID = pTexture->getTextureNumber();
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		}
		else if (curMesh.bHasTexture)
		{
			std::stringstream ss;
			if (texCount < 10)
				ss << "texture_0" << texCount;
			else
				ss << "texture_" << texCount;
			::g_pShaderManager->SetUniformVar1i(uberShaderID, ss.str(), texCount+1);	// 3rd param is slot

			CTextureFromBMP* pTexture = ::g_p_TextureManager->getTextureFromTextureName(itr->second[0]);
			glActiveTexture(pTexture->getTextureUnit());
			GLuint textureID = pTexture->getTextureNumber();
			glBindTexture(GL_TEXTURE_2D, textureID);
			texCount++;
		}
		ExitOnGLError("ERROR: Could not use the shader program");
	}

	::g_pShaderManager->SetUniformVar1i(uberShaderID, "textureID", curMesh.firstTex - 1);
	::g_pShaderManager->SetUniformVar1i(uberShaderID, "hasTexture", curMesh.bHasTexture);
	::g_pShaderManager->SetUniformVar1i(uberShaderID, "blend", curMesh.blend);
	::g_pShaderManager->SetUniformVar1i(uberShaderID, "bIsParticle", curMesh.bIsParticle);
	::g_pShaderManager->SetUniformVar1i(uberShaderID, "isTransparent", curMesh.bIsTransparent);
	::g_pShaderManager->SetUniformVar1f(uberShaderID, "transparency", curMesh.tranparency);

	if ( curMesh.bIsSkybox )
	{
		::g_pShaderManager->SetUniformVar1i( uberShaderID, "bIsSkyMap", GL_TRUE );
		glDisable( GL_CULL_FACE );	// draw ALL faces
	}
	else
	{
		::g_pShaderManager->SetUniformVar1i( uberShaderID, "bIsSkyMap", GL_FALSE );
		glEnable( GL_CULL_FACE );	// Turn on culling
	}
	ExitOnGLError("ERROR: Could not use the shader program");

	if ( curMesh.bUseDebugColour )
	{
		::g_pShaderManager->SetUniformVar1i( uberShaderID, "bUseDebugColour", GL_TRUE );
		::g_pShaderManager->SetUniformVar4f( uberShaderID, "debugObjectColour", 
											 curMesh.debugColour.x, curMesh.debugColour.y, 
											 curMesh.debugColour.z, curMesh.debugColour.a );
	}
	else
	{
		::g_pShaderManager->SetUniformVar1i( uberShaderID, "bUseDebugColour", GL_FALSE );
	}

	ExitOnGLError("ERROR: Could not use the shader program");


	::g_pShaderManager->SetUniformMatrix4fv( uberShaderID, "WorldMatrix", 1, GL_FALSE, glm::value_ptr(matWorld) );
	ExitOnGLError("ERROR: Could not use the shader program");


	::g_pShaderManager->SetUniformMatrix4fv( uberShaderID, "ViewMatrix", 1, GL_FALSE, glm::value_ptr(matView) );


	ExitOnGLError("ERROR: Could not set the shader uniforms");

	// Based on the object "ply file" name, point to the appropriate buffer
	int numberOfIndicesToRender = 0;

	CPlyInfo tempPlyInfo;
	std::string plyFileNameDEBUG = curMesh.modelName;
	if ( !::g_pModelLoader->GetRenderingInfoByModelFileName( curMesh.modelName, 
																tempPlyInfo ) )
	{	// Model isn't present, which is a Very Bad Thing
		return;	// in a for loop, this will go to the next count, skipping everyint
	}
	
	// At this point, we have found a valid model (that was loaded)
	glBindVertexArray( tempPlyInfo.vertexBufferObjectID );					//  g_VertexPositionBufferID[1] );		// "Connects" to a vertex buffer
	glBindBuffer( GL_ARRAY_BUFFER, tempPlyInfo.vertexAttribBufferID );		//  g_VertexColourBufferID[1] );		// Also buffer (with colours)
	numberOfIndicesToRender = tempPlyInfo.numberOfElements * 3;			// g_numberOfTriangles[1] * 3; 
		

	glEnableVertexAttribArray(0);			// Position
	glEnableVertexAttribArray(1);			// Colour
	glEnableVertexAttribArray(2);			// Normal
	glEnableVertexAttribArray(3);			// Texture
	ExitOnGLError("ERROR: Could not enable vertex attributes");


	ExitOnGLError("ERROR: Could not set VAO attributes");

	glDrawElements(GL_TRIANGLES,  
					numberOfIndicesToRender, 
					GL_UNSIGNED_INT, (GLvoid*)0 );
	ExitOnGLError("ERROR: Could not draw the cube");
	// *** END of DRAW THE OBJECT FROM THE BUFFER ****

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);

	return;
}

void DrawObject( IGameObjectRender* pObjectToDraw, glm::mat4 matView )
{
	glEnable( GL_BLEND );		// Enables "blending"
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );


	// Set up separate matrices for each type of tranformation
	// Then combine them (by multiplying) into the world matrix.
	// The order you do this impact what you see. 
	// (The last thing you do is the FIRST thing that's applied)
	// (TIP: Put scaling last)
	// (TIP: pre-Rotation, Translation, post-Rotation )

	std::vector< CMeshDesc > vecMeshes;
	pObjectToDraw->getMeshDescriptions(vecMeshes);

	for (std::vector<CMeshDesc>::iterator itCurMesh = vecMeshes.begin(); itCurMesh != vecMeshes.end(); itCurMesh++)
	{
		DrawMesh( *itCurMesh, matView);
	} // 	for ( std::vector<CMeshDesc>::iterator itCurMesh = vecMeshes.begin(); itCurMesh != vecMeshes.end(); itCurMesh++ )


	return;
}

void SortGameObjects(std::vector<IGameObjectRender*>& vec)
{
	std::vector<IGameObjectRender*> sorted;
	unsigned int originalSize = vec.size();
	float curfarthest;
	IGameObjectRender* cur;

	for (unsigned int i = 0; i < originalSize; i++)
	{
		if (vec.size() > 1)
		{
			unsigned int id = 0;
			curfarthest = 0;
			cur = vec[0];

			for (unsigned int j = 0; j < vec.size(); j++)
			{
				std::vector<CMeshDesc> desc;
				vec[j]->getMeshDescriptions(desc);
				float dist = CVector3f::DistanceSquared(g_pCamera->eye, desc[0].relPosition);
				if (dist > curfarthest)
				{
					cur = vec[j];
					id = j;
					curfarthest = dist;
				}
			}

			vec.erase(vec.begin() + id);
			sorted.push_back(cur);
		}
		else
		{
			sorted.push_back(vec[0]);
		}
	}

	vec = sorted;
}

void glutDisplay_callback(void)
{
	::g_FrameCount++;



	clock_t Now = clock();

	if ( ::g_LastTime == 0 )	// if (LastTime == 0)
	{
		::g_LastTime = Now;		// LastTime = Now;
	}

	// ___                _                 _              _                                  
	//| _ \ ___  _ _   __| | ___  _ _   ___| |_   __ _  __| | ___ __ __ __  _ __   __ _  _ __ 
	//|   // -_)| ' \ / _` |/ -_)| '_| (_-<| ' \ / _` |/ _` |/ _ \\ V  V / | '  \ / _` || '_ \
	//|_|_\\___||_||_|\__,_|\___||_|   /__/|_||_|\__,_|\__,_|\___/ \_/\_/  |_|_|_|\__,_|| .__/
	//                                                                                  |_|   
	// Draw depth buffer from light perspective
	glm::mat4 matViewFromLight(1.0f);
	glm::mat4 light_proj_matrix = glm::frustum( -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 10000.0f );

	std::vector< IGameObjectRender* > vec_pRenderedObjects;

	g_pFactoryMediator->getRenderedObjects( vec_pRenderedObjects, IFactory::ONLY_REGULAR_MESH_OBJECTS );
	SortGameObjects(vec_pRenderedObjects);

	for ( unsigned int lightID = 0; lightID != g_p_LightManager->getMaxLights(); lightID++ )
	{
		CLight light_props;
		::g_p_LightManager->GetLightInformation(lightID, light_props);
		if ( light_props.bIsEnabled && light_props.get_bHasShadow() )
		{
			// AKA The "view or camera" matrix, but from the lights perspective	
			glm::mat4 light_view_matrix = glm::lookAt( glm::vec3(light_props.position.x, light_props.position.y, light_props.position.z ), 
													   glm::vec3( 0.0f, 0.0f, 0.0f ), 
													   glm::vec3( 0.0f, 1.0f, 0.0f ) );
			for (std::vector< IGameObjectRender* >::iterator itGO = vec_pRenderedObjects.begin();
				itGO != vec_pRenderedObjects.end(); itGO++)
			{
				std::vector<CMeshDesc> vecMeshes;
				(*itGO)->getMeshDescriptions( vecMeshes );			
				for ( std::vector<CMeshDesc>::iterator itCurMesh = vecMeshes.begin(); itCurMesh != vecMeshes.end(); itCurMesh++ )
				{
					DrawMeshForShadow( *itCurMesh, matViewFromLight, lightID );
				}
			}
		}
	}// for ( unsigned int lightID...

	// ___                _                                       __                             _  _      __  
	//| _ \ ___  _ _   __| | ___  _ _   ___ __  ___  _ _   ___   / /_ _   ___  _ _  _ __   __ _ | || | _  _\ \ 
	//|   // -_)| ' \ / _` |/ -_)| '_| (_-</ _|/ -_)| ' \ / -_) | || ' \ / _ \| '_|| '  \ / _` || || || || || |
	//|_|_\\___||_||_|\__,_|\___||_|   /__/\__|\___||_||_|\___| | ||_||_|\___/|_|  |_|_|_|\__,_||_||_| \_, || |
	//                                                           \_\                                   |__//_/ 
	// *********************************
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 matView(1.0f);
	
	if (::g_pCamera->getCameraState() == CCamera::INDEPENDENT_FREE_LOOK)
	{
		if (g_pCamera->deltaAngleYaw != 0.0f){
			g_pCamera->cameraAngleYaw += g_pCamera->deltaAngleYaw;
			g_pCamera->deltaAngleYaw = 0.0f;
		}
		if (g_pCamera->deltaAnglePitch != 0.0f){
			g_pCamera->cameraAnglePitch += ::g_pCamera->deltaAnglePitch;
			g_pCamera->deltaAnglePitch = 0.0f;
		}
		if (g_pCamera->deltaMove != 0.0f){
			g_pCamera->eye.x += g_pCamera->deltaMove*sin(g_pCamera->cameraAngleYaw + g_pCamera->cameraMove);
			g_pCamera->eye.y += g_pCamera->deltaMove*tan(g_pCamera->cameraAnglePitch);
			g_pCamera->eye.z += g_pCamera->deltaMove*(-cos(g_pCamera->cameraAngleYaw + g_pCamera->cameraMove));
			g_pCamera->deltaMove = 0.0f;
			g_pCamera->cameraMove = 0.0f;
		}
		g_pCamera->target.x = g_pCamera->eye.x + sin(g_pCamera->cameraAngleYaw + g_pCamera->cameraMove);
		g_pCamera->target.y = g_pCamera->eye.y + tan(g_pCamera->cameraAnglePitch);
		g_pCamera->target.z = g_pCamera->eye.z + (-cos(g_pCamera->cameraAngleYaw + g_pCamera->cameraMove));
	}
	matView = glm::lookAt( glm::vec3(g_pCamera->eye.x, g_pCamera->eye.y, g_pCamera->eye.z),			// Camera (aka "Eye")
							glm::vec3(g_pCamera->target.x, g_pCamera->target.y, g_pCamera->target.z),		// At (aka "target")
							glm::vec3(g_pCamera->up.x, g_pCamera->up.y, g_pCamera->up.z) );		// Up


	ExitOnGLError("Unknown error");


	// ___                       ___  _          _                __        __  
	//|   \  _ _  __ _ __ __ __ / __|| |__ _  _ | |__  ___ __ __ / /___  ___\ \ 
	//| |) || '_|/ _` |\ V  V / \__ \| / /| || || '_ \/ _ \\ \ /| |/ -_)(_-< | |
	//|___/ |_|  \__,_| \_/\_/  |___/|_\_\ \_, ||_.__/\___//_\_\| |\___|/__/ | |
	//                                     |__/                  \_\        /_/ 	
	// Get the objects we are to render from the factory
	g_pFactoryMediator->getRenderedObjects( vec_pRenderedObjects, IFactory::ONLY_SKYBOXES );
	SortGameObjects(vec_pRenderedObjects);
	glDisable(GL_DEPTH_TEST);
	for (std::vector< IGameObjectRender* >::iterator itGO = vec_pRenderedObjects.begin();
		itGO != vec_pRenderedObjects.end(); itGO++)
	{
		DrawObject( *itGO, matView );
	}
	glEnable(GL_DEPTH_TEST);


	// __  __        _         ___                       _                    
	//|  \/  | __ _ (_) _ _   |   \  _ _  __ _ __ __ __ | |    ___  ___  _ __ 
	//| |\/| |/ _` || || ' \  | |) || '_|/ _` |\ V  V / | |__ / _ \/ _ \| '_ \
	//|_|  |_|\__,_||_||_||_| |___/ |_|  \__,_| \_/\_/  |____|\___/\___/| .__/
	//                                                                  |_|   
	g_pFactoryMediator->getRenderedObjects(vec_pRenderedObjects, IFactory::ONLY_REGULAR_MESH_OBJECTS);
	SortGameObjects(vec_pRenderedObjects);

	for (std::vector< IGameObjectRender* >::iterator itGO = vec_pRenderedObjects.begin();
		itGO != vec_pRenderedObjects.end(); itGO++)
	{
		DrawObject( *itGO, matView );
	}
	
	for (std::vector< CGameObject* >::iterator itGO = g_vecPhysDebugObjects.begin();
		itGO != g_vecPhysDebugObjects.end(); itGO++)
	{
		DrawObject(*itGO, matView);
	}
	// Debug object draw loop
	// Get the debug objects...
	std::vector< CMeshDesc > vecDebugMeshes;
	for (std::vector< IGameObjectRender* >::iterator itGO = vec_pRenderedObjects.begin();
		itGO != vec_pRenderedObjects.end(); itGO++)
	{
		vecDebugMeshes.clear();
		(*itGO)->getDebugMeshObjects( vecDebugMeshes, IGameObjectRender::AABB_CENTRED );
		// Then draw them
		for ( std::vector< CMeshDesc >::iterator itDO = vecDebugMeshes.begin();
			  itDO != vecDebugMeshes.end(); itDO++ )
		{
			DrawMesh( *itDO, matView );
		}
	}
	// for ( std::vector< IGameObjectRender* >::iterator itGO ...

	// vecDebugMeshes is stack based, so debug objects get deleted


	if (g_SavingData == true)
	{
		std::ofstream myFile;
		myFile.open("assets/Scene2.txt");

		for (unsigned index = 0; index < vecModelsInfo.size(); index++)
		{
			myFile << "<Model> " << std::endl;
			myFile << "\t<Type> " << vecModelsInfo[index].type << " </Type>" << std::endl;
			myFile << "\t<PlyFile> " << vecModelsInfo[index].file << " </PlyFile>" << std::endl;
			if (vecModelsInfo[index].isSkybox)
			{
				myFile << "\t<SkyboxTexture>" << std::endl;
				myFile << "\t\t<Name> " << vecModelsInfo[index].tex[0][0] << " </Name>" << std::endl;
				for (unsigned j = 1; j <= 6; j++)
					myFile << "\t\t<Texture> " << vecModelsInfo[index].tex[0][j] << " <Texture>" << std::endl;
				myFile << "\t</SkyboxTexture> " << std::endl;
				myFile << "\t<IsSkybox> True </IsSkybox> " << std::endl;
			}
			else
			{
				for (unsigned j = 0; j < vecModelsInfo[index].tex[0].size(); j++)
					myFile << "\t<Texture> " << vecModelsInfo[index].tex[0][j] << " </Texture>" << std::endl;
			}
			static CPhysicalProp physProps;
			::g_pFactoryMediator->getPhysicalPropertiesByID(vecModelsInfo[index].ID, physProps);
			myFile << "\t<Position> " << physProps.position.x << " " << physProps.position.y << " " << physProps.position.z << " </Position>" << std::endl;
			myFile << "\t<Colour> " << vecModelsInfo[index].col.x << " " << vecModelsInfo[index].col.y << " " << vecModelsInfo[index].col.z << " </Colour>" << std::endl;
			myFile << "\t<Rotation> " << physProps.rotStep.x << " " << physProps.rotStep.y << " " << physProps.rotStep.z << " </Rotation>" << std::endl;
			myFile << "\t<Blend> " << vecModelsInfo[index].blend << " </Blend>" << std::endl;
			if (vecModelsInfo[index].isTransparent)
				myFile << "\t<Transparency> " << vecModelsInfo[index].transparency << " </Transparency>" << std::endl;
			if (vecModelsInfo[index].isPlayer)
				myFile << "\t<IsPlayer>  True  </IsPlayer>" << std::endl;
			if (vecModelsInfo[index].isLightBall)
				myFile << "\t<IsLightBall>  True  </IsLightBall>" << std::endl;
			if (vecModelsInfo[index].isEnvironment)
				myFile << "\t<IsEnviro>  True  </IsEnviro>" << std::endl;
			else
				myFile << "\t<Scale> " << vecModelsInfo[index].scale << " </Scale>" << std::endl;
			myFile << "</Model> " << std::endl;
		}

		g_SavingData = false;
	}

	// Detatch from the vertex buffer (0 is reserved in OpenGL, so setting to "0" means "set to nothing", sort of)
	glBindVertexArray(0);
	// Detatch from the current shader program
	glUseProgram(0);


	// Now that everything is drawn, show the back buffer
	// (i.e. switch the 'back' framebuffer with the 'front'
	glutSwapBuffers();

	return;
}

// This is a relatively expensive call as it uses the string stream, etc.
// Only call this if the shader has changed
void SetLightingUniformIDs(GLuint shaderID)	// USES shadermanager
{
	CShaderProgramDescription theShader;
	theShader.ID = shaderID;
	::g_pShaderManager->GetShaderProgramInfo( theShader );

	ExitOnGLError("Error getting uniform light variables");
	for ( unsigned int index = 0; index != ::g_p_LightManager->getMaxLights(); index++ )
	{
		std::stringstream ssLight;
		ssLight << "Lights[" << index << "].";

		::g_p_LightManager->GetLightPointer(index)->ID_bIsEnabled = theShader.getUniformVarIDFromName( ssLight.str().append("isEnabled") );
		::g_p_LightManager->GetLightPointer(index)->ID_bIsLocal = theShader.getUniformVarIDFromName( ssLight.str().append("isLocal") );
		::g_p_LightManager->GetLightPointer(index)->ID_bIsSpot = theShader.getUniformVarIDFromName( ssLight.str().append("isSpot") );
		::g_p_LightManager->GetLightPointer(index)->ID_ambientColour = theShader.getUniformVarIDFromName( ssLight.str().append("ambient") );
		::g_p_LightManager->GetLightPointer(index)->ID_diffuseColour = theShader.getUniformVarIDFromName( ssLight.str().append("color") );
		::g_p_LightManager->GetLightPointer(index)->ID_position = theShader.getUniformVarIDFromName( ssLight.str().append("position") );
		::g_p_LightManager->GetLightPointer(index)->ID_halfVector = theShader.getUniformVarIDFromName( ssLight.str().append("halfVector") );
		::g_p_LightManager->GetLightPointer(index)->ID_spotConeDirection = theShader.getUniformVarIDFromName( ssLight.str().append("coneDirection") );
		::g_p_LightManager->GetLightPointer(index)->ID_spotCosCutoff = theShader.getUniformVarIDFromName( ssLight.str().append("spotCosCutoff") );
		::g_p_LightManager->GetLightPointer(index)->ID_spotExponent = theShader.getUniformVarIDFromName( ssLight.str().append("spotExponent") );
		::g_p_LightManager->GetLightPointer(index)->ID_constantAttenuation = theShader.getUniformVarIDFromName( ssLight.str().append("constantAttenuation") );
		::g_p_LightManager->GetLightPointer(index)->ID_linearAttenuation = theShader.getUniformVarIDFromName( ssLight.str().append("linearAttenuation") );
		::g_p_LightManager->GetLightPointer(index)->ID_quadraticAttenuation = theShader.getUniformVarIDFromName( ssLight.str().append("quadraticAttenuation") );
		::g_p_LightManager->GetLightPointer(index)->ID_range = theShader.getUniformVarIDFromName( ssLight.str().append("range") );
	}
	ExitOnGLError("Error getting uniform light variables");


	return;
}

void CopyLigtDataToShader(GLuint shaderID)		// USES shadermanager
{
	for ( int index = 0; index != ::g_p_LightManager->getMaxLights(); index++ )
	{
		CLight* pLight = ::g_p_LightManager->GetLightPointer(index);
		// Got a valid pointer?
		if ( pLight != 0 )
		{
			if ( pLight->bIsEnabled )	{	glUniform1i( static_cast<GLuint>(pLight->ID_bIsEnabled), GL_TRUE ); }
			else						{	glUniform1i( static_cast<GLuint>(pLight->ID_bIsEnabled), GL_FALSE ); }
			ExitOnGLError("Error setting light values");

			if ( pLight->bIsLocal )		{	glUniform1i( static_cast<GLuint>(pLight->ID_bIsLocal), GL_TRUE);	}
			else						{	glUniform1i( static_cast<GLuint>(pLight->ID_bIsLocal), GL_FALSE);	}

			if ( pLight->bIsSpot )		{	glUniform1i( static_cast<GLuint>(pLight->ID_bIsSpot), GL_TRUE);	}
			else						{	glUniform1i( static_cast<GLuint>(pLight->ID_bIsSpot), GL_FALSE);	}
	

			glUniform3f( static_cast<GLuint>(pLight->ID_ambientColour), pLight->ambientColour.x, pLight->ambientColour.y, pLight->ambientColour.z );

			glUniform3f( static_cast<GLuint>(pLight->ID_diffuseColour), pLight->diffuseColour.x, pLight->diffuseColour.y, pLight->diffuseColour.z );

			glUniform3f( static_cast<GLuint>(pLight->ID_position), pLight->position.x, pLight->position.y, pLight->position.z );

			glUniform3f( static_cast<GLuint>(pLight->ID_halfVector), pLight->halfVector.x, pLight->halfVector.y, pLight->halfVector.z );

			glUniform3f( static_cast<GLuint>(pLight->ID_spotConeDirection), pLight->spotConeDirection.x, pLight->spotConeDirection.y, pLight->spotConeDirection.z );

			glUniform1f( static_cast<GLuint>(pLight->ID_spotCosCutoff), pLight->spotCosCutoff );

			glUniform1f( static_cast<GLuint>(pLight->ID_spotExponent), pLight->spotExponent );

			glUniform1f( static_cast<GLuint>(pLight->ID_constantAttenuation), pLight->constantAttenuation );

			glUniform1f( static_cast<GLuint>(pLight->ID_linearAttenuation), pLight->linearAttenuation );

			glUniform1f( static_cast<GLuint>(pLight->ID_quadraticAttenuation), pLight->quadraticAttenuation );

			glUniform1f( static_cast<GLuint>(pLight->ID_range), pLight->range );

		}
	}// for ( int index = 0...

	return;
}