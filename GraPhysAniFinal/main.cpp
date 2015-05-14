//Physics 2 Assignment 4 by James Kelly

// Some code originally from : http://openglbook.com/the-book.html 

// Comments are from Michael Feeney (mfeeney(at)fanshawec.ca)

#include "global.h"

//https://github.com/mmmovania/HavokPhysicsTutorials/blob/master/GettingStarted/main.cpp
//http://mmmovania.blogspot.ca/2014/03/havok-physics-engine-tutorial-series.html
#define WINDOW_TITLE_PREFIX "OpenGL for the win!"
#include <iostream>		// Added
#include <sstream>		// Added

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <vector>
#include <fstream>

#include "CRender/CGLShaderManager.h"
#include "cMeshGenerator_PCN.h"

void LoadShaders(void);

void SetUpTextures(void);

void SetupLights(void);

void ReadInModelsInfo(std::string fileLocation);

void setUpShadowTexture(void);

//#define FOREACH( type, it, list ) \
//	for( type it = (list).begin(); it != (list).end(); it++ )

GLuint g_shadow_depthTexture_FrameBufferObjectID = -1;
GLuint g_shadow_depthTexture_ID = -1;


void setUpShadowTexture(void)
{
	glGenFramebuffers(1, &g_shadow_depthTexture_FrameBufferObjectID);
	glBindFramebuffer(GL_FRAMEBUFFER, g_shadow_depthTexture_FrameBufferObjectID);

	glGenTextures(1, &g_shadow_depthTexture_ID);
	glBindTexture(GL_TEXTURE_2D, g_shadow_depthTexture_ID);
	glTexStorage2D(GL_TEXTURE_2D, 11, GL_DEPTH_COMPONENT32F, SHADOW_DEPTH_TEXTURE_SIZE, SHADOW_DEPTH_TEXTURE_SIZE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, g_shadow_depthTexture_ID, 0);

	glDrawBuffer(GL_NONE); // No color buffer is drawn to.
 
	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		return;
	}

	ExitOnGLError("There was a problem setting up the shadow texture.");

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return;
}

void makeHavokObject(int ID)
{
	std::vector<CMeshDesc> vecMeshDescs;
	g_pFactoryMediator->getRenderingPropertiesByID(ID, vecMeshDescs);

	CPhysicalProp physicalProp;
	g_pFactoryMediator->getPhysicalPropertiesByID(ID, physicalProp);

	// Is the object a ball?
	if (vecMeshDescs[0].ObjectType == "Ball" || vecMeshDescs[0].ObjectType == "Star")
	{	
		float radius = vecMeshDescs[0].scale.x * 0.5f;
		float mass = radius * 0.5f;
		CVector3f position(physicalProp.position);
		CVector4f Qrotation(physicalProp.orientation.x, physicalProp.orientation.y,
			physicalProp.orientation.z, 0.0f);
		std::wstringstream ss;
		ss << ID;
		if (vecMeshDescs[0].ObjectType == "Star")
			::g_pPhysicsManager->CreateRigidBody(ss.str(), radius, mass, position, Qrotation, true, true);// Size == max (in this example)
		else
			::g_pPhysicsManager->CreateRigidBody(ss.str(), radius, mass, position, Qrotation, true, false);// Size == max (in this example)
	}

	else if (vecMeshDescs[0].ObjectType == "Box")
	{	
		CVector3f boxSize(vecMeshDescs[0].scale);
		CVector3f position(physicalProp.position);
		CVector4f Qrotation(physicalProp.orientation.x, physicalProp.orientation.y,
			physicalProp.orientation.z, 0.0f);
		float mass = 5.f;
		float friction = 0.5f;
		std::wstringstream ss;
		ss << ID;
		::g_pPhysicsManager->CreateRigidBody(ss.str(), boxSize, mass, friction, position, Qrotation, true);
	}

	else if (vecMeshDescs[0].ObjectType == "Vehicle")
	{	
		CPlyInfo modelInfo;
		g_pModelLoader->GetRenderingInfoByModelFileName(vecMeshDescs[0].modelName, modelInfo);
		float truckWidth = modelInfo.maxExtentX.x - modelInfo.minExtentX.x;
		float truckHeight = modelInfo.maxExtentY.y - modelInfo.minExtentY.y; 
		float truckLength = modelInfo.maxExtentZ.z - modelInfo.minExtentZ.z;

		truckWidth *= vecMeshDescs[0].scale.x;
		truckHeight *= vecMeshDescs[0].scale.y;
		truckLength *= vecMeshDescs[0].scale.z;

		float mass = 5.f;
		CVector3f boxSize(truckWidth, truckHeight, truckLength);
		CVector3f position(physicalProp.position);
		CVector4f Qrotation(physicalProp.orientation.x, physicalProp.orientation.y,
			physicalProp.orientation.z, 0.0f);
		std::wstringstream ss;
		ss << ID;
		bool isPlayer;
		if (ID == ::g_Player_ID)
			isPlayer = true;
		else isPlayer = false;
		::g_pPhysicsManager->BuildVehicle(ss.str(), boxSize, mass, position, Qrotation, true, isPlayer);
	}

	else if (vecMeshDescs[0].ObjectType == "GroundFloor" || vecMeshDescs[0].ObjectType == "IceFloor")
	{	
		CVector3f boxSize(vecMeshDescs[0].scale.x, 1.0f, vecMeshDescs[0].scale.x);
		CVector3f position(physicalProp.position.x, physicalProp.position.y, physicalProp.position.z);
		CVector4f Qrotation(physicalProp.orientation.x, physicalProp.orientation.y,
			physicalProp.orientation.z, 0.0f);

		float friction = 0.6f;
		if (vecMeshDescs[0].ObjectType == "IceFloor")
			friction = 0.1f;
		std::wstringstream ss;
		ss << ID;
		::g_pPhysicsManager->CreateRigidBody(ss.str(), boxSize, 0.0f,	// Will be infinite (unmoving)
			friction, position, Qrotation, false);	// Infinite mass
	}

	else if (vecMeshDescs[0].ObjectType == "CheckPoint")
	{
		/* Text file entry
		<Model>
			<Type> CheckPoint </Type>
			<PlyFile> Cube2.ply </PlyFile>
			<Position> 0.0 0.0 -10.0 </Position>
		</Model>*/
		CVector3f minSize(physicalProp.position);
		CVector3f maxSize(physicalProp.position.x + 5.0f, physicalProp.position.y + 5.0f, physicalProp.position.z + 5.0f);
		std::wstringstream ss;
		ss << ID;
		::g_pPhysicsManager->CreatePhantomCheckPoint(ss.str(), minSize, maxSize);
	}
}


int main(int argc, char* argv[])
{
	bool bIsbowlingMode = false;
	std::wstring input2 = L"Hello! Please enter 1 if you wish to try the bowling mode. Enter anything else to play checkpoint mode";
	std::wcout << input2 << std::endl;
	std::wstring input;
	std::getline(std::wcin, input);
	if (input == L"1")
	{
		bIsbowlingMode = true;
	}

	bool bLoadTires = false;
	std::wstring input3 = L"Would you like to load the tire models? (loading tires may cause slowdown) y/n";
	std::wcout << input3 << std::endl;
	std::wstring input4;
	std::getline(std::wcin, input4);
	if (input4 == L"y")
	{
		bLoadTires = true;
	}


	printTheWhatsThisProgramAboutBlurb();

	::g_gameState.currentGameMode = CGameState::GAME_LOADING;

	::OpenGL_Initialize( argc, argv, 1200, 800 );		// Initialize(argc, argv);
	//::OpenGL_Initialize( argc, argv, 640, 480 );		// Initialize(argc, argv);


	// CModelLoaderManager
	g_pModelLoader = new CModelLoaderManager();
	g_pModelLoader->SetRootDirectory( "assets/models" );

	std::vector< CModelLoaderManager::CLoadInfo > vecModelsToLoad;

	if (bIsbowlingMode)
		ReadInModelsInfo("assets/BowlingScene.txt");
	else 
		ReadInModelsInfo("assets/Scene.txt");

	for (unsigned int i = 0; i < vecModelsInfo.size(); i++)
	{
		::g_mapModelTypes[vecModelsInfo[i].type] = vecModelsInfo[i].file;
		if (vecModelsInfo[i].tex.size() > 0)
			::g_mapTextures[i] = vecModelsInfo[i].tex[0];
		//vecModelsToLoad.push_back(CModelLoaderManager::CLoadInfo(vecModelsInfo[i].file, 1.0f, true));
	}
	for (unsigned int i = 0; i < vecModelsInfo.size(); i++)
	{
		for (std::map<std::string, std::string>::iterator jIterator = g_mapModelTypes.begin(); jIterator != ::g_mapModelTypes.end(); jIterator++)
		{
			if (vecModelsInfo[i].type == jIterator->first){
				if (vecModelsInfo[i].isEnvironment){
					vecModelsToLoad.push_back(CModelLoaderManager::CLoadInfo(jIterator->second));
				}
				else
					vecModelsToLoad.push_back(CModelLoaderManager::CLoadInfo(jIterator->second, 1.0f, true));
			}
		}
	}

	vecModelsToLoad.push_back( CModelLoaderManager::CLoadInfo("sphere_UV_xyz.ply", 1.0f, true) );
	vecModelsToLoad.push_back( CModelLoaderManager::CLoadInfo("Cube.ply", 1.0f, true ) );
	vecModelsToLoad.push_back( CModelLoaderManager::CLoadInfo("Cube2.ply", 1.0f, true));
	vecModelsToLoad.push_back( CModelLoaderManager::CLoadInfo("1x1_6_Star_2_Sided.ply", 1.0f, true ) );
	vecModelsToLoad.push_back( CModelLoaderManager::CLoadInfo("truck.ply", 1.0f, true));
	//vecModelsToLoad.push_back( CModelLoaderManager::CLoadInfo("truck2.ply", 1.0f, true));
	if (bLoadTires)
		vecModelsToLoad.push_back( CModelLoaderManager::CLoadInfo("tire.ply", 1.0f, true));

	if ( ! g_pModelLoader->LoadModels( vecModelsToLoad ) )
	{
		std::cout << "Can't load one or more models. Sorry it didn't work out." << std::endl;
		return -1;
	}

	g_pShaderManager = new CGLShaderManager();
	//LoadShaders();		// Moved from CreateCube
	CShaderDescription uberVertex;
	uberVertex.filename = "assets/shaders/OpenGL.LightTexSkyUber.vertex.glsl";
	uberVertex.name = "UberVertex";

	CShaderDescription uberFragment;
	uberFragment.filename = "assets/shaders/OpenGL.LightTexSkyUber.fragment_texture.glsl";
	uberFragment.name = "UberFragment";

	CShaderProgramDescription uberShader("UberShader", uberVertex, uberFragment );

	if ( !g_pShaderManager->CreateShaderProgramFromFile( uberShader ) )
	{
		std::cout << "Error compiling one or more shaders..." << std::endl;
		std::cout << g_pShaderManager->getLastError() << std::endl;
		std::cout << uberShader.getErrorString() << std::endl;
		return -1;
	}

	::SetUpTextures();

	std::cout << "Starting Havok" << std::endl;
	g_pFactoryMediator = new CFactoryMediator();
	// ******************************************************
	// Set up the Havok physics thing
	// Create a Havok Physic thingie:
	// Passing "Havok" gives us a physics manager using Havok. 
	// Anything else is an error...
	// if crashing on release, right click on Desktop->NVIDIA Contol Panel->Set PhysX configuration. Select a PhysX dropdown menu->CPU
	//::g_pPhysicsManager = CPhysicsManagerFactory::CreatePhysicsMananger( L"Havok" ); 

	if ((::g_pPhysicsManager = CPhysicsManagerFactory::CreatePhysicsMananger(L"Havok")) == 0){
		std::cout << "Failed Creating Havok" << std::endl;  std::cout.flush();
	}
	// 
	std::cout << "Havok Created" << std::endl; std::cout.flush();
	std::vector< CNameValuePair2 > vecParams;
	vecParams.push_back( CNameValuePair2( L"VisualDebugger", true ) );

	::g_pPhysicsManager->Init(vecParams); 
	// ******************************************************************
	
	std::cout << "Done with physics." << std::endl;	std::cout.flush();

	static const float oneDegreeInRadians = static_cast<float>(PI) / 180.0f;

	for (unsigned i = 0; i < vecModelsInfo.size(); i++)
	{
		CPhysicalProp physicalProp;
		physicalProp.position = vecModelsInfo[i].pos;
		physicalProp.setOrientationEulerAngles(CVector3f(oneDegreeInRadians * vecModelsInfo[i].rot.x, oneDegreeInRadians * vecModelsInfo[i].rot.y, oneDegreeInRadians * vecModelsInfo[i].rot.z));
		physicalProp.rotStep = vecModelsInfo[i].rot;

		CMeshDesc mesh(vecModelsInfo[i].file);
		mesh.scale = vecModelsInfo[i].scale;
		mesh.bIsSkybox = vecModelsInfo[i].isSkybox;
		mesh.bIsPlayer = vecModelsInfo[i].isPlayer;
		mesh.modelID = i;
		mesh.blend = vecModelsInfo[i].blend;
		mesh.debugColour = CVector4f(vecModelsInfo[i].col.x, vecModelsInfo[i].col.y, vecModelsInfo[i].col.z, 1.0f);
		mesh.bIsParticle = vecModelsInfo[i].isParticle;

		if (vecModelsInfo[i].isTransparent){
			mesh.bIsTransparent = true;
			mesh.tranparency = vecModelsInfo[i].transparency;
		}
		else{
			mesh.bIsTransparent = false;
			mesh.tranparency = 1.0f;
		}

		if (vecModelsInfo[i].tex.size() > 0)
		{
			mesh.bHasTexture = true;
			mesh.firstTex = vecModelsInfo[i].firstTex;
		}


		mesh.ObjectType = vecModelsInfo[i].type;
		if (vecModelsInfo[i].isSkybox || vecModelsInfo[i].isParticle)
			mesh.bIsHavok = false;

		vecModelsInfo[i].ID = g_pFactoryMediator->CreateObjectByType(vecModelsInfo[i].type, physicalProp, mesh);

		makeHavokObject(vecModelsInfo[i].ID);

		if (vecModelsInfo[i].isPlayer)
			::g_FloorHavokID = vecModelsInfo[i].ID;
		if (vecModelsInfo[i].isSkybox)
			::g_skyBoxID = vecModelsInfo[i].ID;
		//if (vecModelsInfo[i].isLightBall)
		//	::g_lightModelID = vecModelsInfo[i].ID;
		if (vecModelsInfo[i].isParticle)
			::g_vecParticleID.push_back(vecModelsInfo[i].ID);
	}
	
	

	CMeshDesc sphereMesh("sphere_UV_xyz.ply");
	sphereMesh.debugColour = CVector4f(0.75f, 0.5f, 0.3f, 0.0f);
	sphereMesh.bIsTransparent = true;
	sphereMesh.tranparency = 0.4f;
	CPhysicalProp sphereProps( CVector3f( 0.0f, 5.0f, 4.0f ) );
	sphereProps.position = (CVector3f(0.0f, 5.0f, 0.0f));
	::g_lightModelID = g_pFactoryMediator->CreateObjectByType( "Sphere UV", sphereProps, sphereMesh );

	CMeshDesc sphereHavokMesh("sphere_UV_xyz.ply");
	sphereHavokMesh.debugColour = CVector4f(0.75f, 0.5f, 0.3f, 0.0f);
	sphereHavokMesh.ObjectType = "Star";
	sphereHavokMesh.bIsHavok = false;
	CPhysicalProp sphereHavokProps( CVector3f( 0.0f, 5.0f, -10.0f ) );
	::g_StarHavokID = g_pFactoryMediator->CreateObjectByType( "Sphere UV", sphereHavokProps, sphereHavokMesh );

	//makeHavokObject(::g_StarHavokID);




	//for (int i = 1; i < 4; i++){
	//	for (int j = 1; j < 4; j++){
	//		CMeshDesc floorHavokMesh("Cube2.ply");
	//		floorHavokMesh.debugColour = CVector4f(0.75f, 0.5f, 0.3f, 0.0f);

	//		floorHavokMesh.bHasTexture = true;
	//		if (i == 2 && j == 2){
	//			floorHavokMesh.ObjectType = "IceFloor";
	//		}
	//		else {
	//			floorHavokMesh.ObjectType = "GroundFloor";
	//		}
	//		floorHavokMesh.scale = floorSize;
	//		//CPhysicalProp floorHavokProps(CVector3f(-(floorSize / 2.0f), -floorSize, -(floorSize / 2.0f)));
	//		CVector3f floorPosition;
	//		floorPosition.x = i*(-(floorSize / 2.0f));
	//		floorPosition.z = j*(-(floorSize / 2.0f));
	//		floorPosition.y = -floorSize;
	//		CPhysicalProp floorHavokProps(floorPosition);
	//		unsigned int floorID = g_pFactoryMediator->CreateObjectByType("Cube UV", floorHavokProps, floorHavokMesh);

	//		makeHavokObject(floorID);
	//	}
	//}

	CMeshDesc truckMesh("truck.ply");
	truckMesh.ObjectType = "Vehicle";
	truckMesh.scale = 5.0f;

	CPhysicalProp truckProps(CVector3f(0.0f, 5.0f, 4.0f));
	truckProps.position = (CVector3f(0.0f, 5.0f, 0.0f));
	::g_Player_ID = g_pFactoryMediator->CreateObjectByType("Player", truckProps, truckMesh);

	makeHavokObject(::g_Player_ID);

	if (bLoadTires){
		CMeshDesc tireMesh("tire.ply");
		tireMesh.bIsHavok = false;
		tireMesh.scale = 1.0f;
		g_pFactoryMediator->addMeshDescription(::g_Player_ID, tireMesh);
		g_pFactoryMediator->addMeshDescription(::g_Player_ID, tireMesh);
		g_pFactoryMediator->addMeshDescription(::g_Player_ID, tireMesh);
		g_pFactoryMediator->addMeshDescription(::g_Player_ID, tireMesh);
	}
	



	/*CMeshDesc checkpointHavokMesh("Cube2.ply");
	checkpointHavokMesh.debugColour = CVector4f(0.75f, 0.5f, 0.3f, 0.0f);
	checkpointHavokMesh.ObjectType = "CheckPoint";
	CPhysicalProp checkpointHavokProps(CVector3f(0.0f, 0.0f, 0.0f));
	unsigned int checkpoint_ID = g_pFactoryMediator->CreateObjectByType("Sphere UV", checkpointHavokProps, checkpointHavokMesh);

	makeHavokObject(checkpoint_ID);*/

	



	g_pPhysicsManager->SetCheckPoints(vecCheckPointsInfo);

	//g_pPhysicsManager

	//GenerateAABBWorld();

	// Added October 3, 2014
	g_pCamera = new CCamera();
	// Camera expects an IMediator*, so cast it as that
	g_pCamera->SetMediator( (IMediator*)g_pFactoryMediator );

	g_pCamera->eye.x = 0.0f;		// Centred (left and right)
	g_pCamera->eye.y = 5.0f;		// 2.0 units "above" the "ground"
	g_pCamera->eye.z = -20.0f;		// .0funits0 from "back" the origin    .  1, 2.5, 1.8
	g_pCamera->target.x = 50.0f;		// Centred (left and right)
	g_pCamera->target.y = 350.0f;		// 2.0 units "above" the "ground"
	g_pCamera->target.z = 50.0f;		// 0.0 units "back" from the origin
	g_pCamera->cameraAngleYaw = 2.8f;
	g_pCamera->cameraAnglePitch = -0.2f;

	g_pCamera->up.x = 0.0f;
	g_pCamera->up.y = 1.0f;				// The Y axis is "up and down"

	g_pCamera->m_LEFPLookupMode = CCamera::LERP;

	g_pCamera->setMode_FollowAtDistance( ::g_Player_ID );
	float followSpeed = 30.0f;		// 1.0f whatever per second
	float followMinDistance = 3.0f;
	float followMaxSpeedDisance = 100.0f;
	g_pCamera->setFollowDistances( followMinDistance, followMaxSpeedDisance );
	g_pCamera->setFollowMaxSpeed( followSpeed );

	// A "fly through" camera
	// These numbers are sort of in the direction of the original camera
	g_pCamera->orientation = glm::fquat( 0.0960671529f, 0.972246766f, -0.0900072306f, -0.193443686f );
	glm::normalize( g_pCamera->orientation );
	//g_pCamera->setMode_IndependentFreeLook();


	g_pMouseState = new CMouseState();

	// Set up the basic lighting
	ExitOnGLError("Error setting light values");
	SetupLights();

		//
	if ( !::g_p_LightManager->initShadowMaps( 1, CLightManager::DEFAULT_SHADOW_DEPTH_TEXTURE_SIZE ) )
	{
		std::cout << "Error setting up the shadow textures: " << std::endl;
		std::cout << ::g_p_LightManager->getLastError();
	}

	setUpShadowTexture();


	g_p_GameControllerManager = CGameControllerManager::getGameControllerManager();
	g_p_GameController_0 = g_p_GameControllerManager->getController(0);
	if ( ( g_p_GameController_0 != 0 ) && 
		 ( g_p_GameController_0->bIsConnected() ) )
	{
		g_p_GameController_0->AddVibrationSequence( IGameController::CVibStep::LEFT, 0.5f, 2.0f );
		g_p_GameController_0->AddVibrationSequence( IGameController::CVibStep::RIGHT, 0.5f, 1.0f );
		std::cout << "Game controller 0 found!" << std::endl;
	}
	else 
	{
		std::cout << "Didn't get an ID for the game controller; is there one plugged in?" << std::endl;
	}


	//***************************************
	g_p_checkpointEmitter = new CParticleEmitter();
	g_p_timerEmitter = new CParticleEmitter();
	//**************************************

	CPhysicalProp starProps;
	::g_pFactoryMediator->getPhysicalPropertiesByID(::g_StarHavokID, starProps);
	g_p_checkpointEmitter->SetLocation(starProps.position);
	::g_p_checkpointEmitter->GenerateParticles(1, /*number of particles*/
		CVector3f(0.0f, 0.0f, 0.0f), /*Init veloc.*/
		0.1f, /*dist from source */
		5.0f, /*seconds*/
		true);
	::g_p_checkpointEmitter->SetAcceleration(CVector3f(0.0f, 0.0f, 0.0f));

	g_p_timerEmitter->SetLocation(truckProps.position);
	::g_p_timerEmitter->GenerateParticles(1, /*number of particles*/
		CVector3f(0.0f, 1.0f, 0.0f), /*Init veloc.*/
		0.01f, /*dist from source */
		1.0f, /*seconds*/
		true);
	::g_p_timerEmitter->SetAcceleration(CVector3f(0.0f, 1.0f, 0.0f));



	::g_p_PhysicsThingy = new CPhysicsCalculatron();


	// Added in animation on Sept 19
	::g_simTimer.Reset();
	::g_simTimer.Start();		// Start "counting"

	if (bIsbowlingMode)
	{
		::g_gameState.currentGameMode = CGameState::GAME_BOWLING;
	}
	else
		::g_gameState.currentGameMode = CGameState::GAME_RUNNING;
	
	// FULL SCREEN, Mr. Data!
	//::glutFullScreen();

	ExitOnGLError("Error getting uniform light variables");

	glutMainLoop();

	std::cout << "Shutting down..." << std::endl;

	ShutDown();
  
	exit(EXIT_SUCCESS);
}

void ShutDown(void)
{
	delete ::g_p_PhysicsThingy;
	delete ::g_pCamera;
	delete ::g_pMouseState;
	delete ::g_pFactoryMediator;
	return;
}

std::string GetModeString(void)
{
	std::stringstream ss;

	switch ( ::g_gameState.currentGameMode )
	{
	case CGameState::UNKNOWN:
		break;
	case CGameState::GAME_RUNNING:
		ss << "(game running)";
		break;
	case CGameState::GAME_LOADING:
		ss << "(game loading)";
		break;
	case CGameState::EDITING_LIGHTS:
		ss << "(editing light# " << ::g_gameState.selectedLightID << " )";
		break;
	case CGameState::EDITING_OBJECTS:
		ss << "(editing object# " << ::g_gameState.selectedObjectID << " )";
		break;
	};

	return ss.str();
}


void TimerFunction(int Value)
{
  if (0 != Value) 
  {
	std::stringstream ss;
	ss << WINDOW_TITLE_PREFIX << ": " 
		<< ::g_FrameCount * 4 << " FPS, screen( "
		<< ::g_screenWidth << ", "					// << CurrentWidth << " x "
		<< ::g_screenHeight <<" )  ";					// << CurrentHeight;

	ss << GetModeString();
	
	// Set the "mode" title
	glutSetWindowTitle(ss.str().c_str());
  }

  ::g_FrameCount = 0;	// FrameCount = 0;
  glutTimerFunc(250, TimerFunction, 1);

  return;
}

void SetupLights(void)
{
	g_p_LightManager = new CLightManager();

	g_p_LightManager->setMaxLights(::g_ShaderUniformVariables.MAXLIGHTS);

	g_p_LightManager->CreateBasicOpenGLLights(true);

	g_p_LightManager->GetLightPointer(0)->setLightType(CLight::POINT);
	g_p_LightManager->GetLightPointer(0)->position = CVector3f(224.0f, 328.0f, 244.0f);
	g_p_LightManager->GetLightPointer(0)->bIsEnabled = true;

	g_p_LightManager->GetLightPointer(1)->setLightType(CLight::POINT);
	g_p_LightManager->GetLightPointer(1)->position = CVector3f(0.0f, 10.0f, 0.0f);
	g_p_LightManager->GetLightPointer(1)->ambientColour = CVector3f(0.0f, 0.0f, 1.0f);
	
	g_p_LightManager->GetLightPointer(1)->bIsEnabled = true;


	//for (unsigned int i = 0; i < vecLightsInfo.size(); i++)
	//{
	//	g_p_LightManager->GetLightPointer(i)->setLightType(vecLightsInfo[i].type);
	//	g_p_LightManager->GetLightPointer(i)->position = vecLightsInfo[i].pos;
	//	g_p_LightManager->GetLightPointer(i)->bIsEnabled = true;
	//	g_p_LightManager->GetLightPointer(i)->diffuseColour.x = vecLightsInfo[i].col.x;
	//	g_p_LightManager->GetLightPointer(i)->diffuseColour.y = vecLightsInfo[i].col.y;
	//	g_p_LightManager->GetLightPointer(i)->diffuseColour.z = vecLightsInfo[i].col.z;
	//	g_p_LightManager->GetLightPointer(i)->constantAttenuation = vecLightsInfo[i].constAtten;
	//	g_p_LightManager->GetLightPointer(i)->linearAttenuation = vecLightsInfo[i].linAtten;
	//	g_p_LightManager->GetLightPointer(i)->quadraticAttenuation = vecLightsInfo[i].quadAtten;
	//	g_p_LightManager->GetLightPointer(i)->calculateRangeFromAttenuation(vecLightsInfo[i].attenRangeStart, vecLightsInfo[i].attenRangeEnd);
	//}

	return;
}

void SetUpTextures(void)
{
	g_p_TextureManager = new CTextureManager();

	unsigned int totalTextures = 0;
	for (unsigned i = 0; i < vecModelsInfo.size(); i++)
	{
		if (vecModelsInfo[i].isSkybox)
		{
			::g_p_TextureManager->setBasePath("assets/textures/SkyBoxes_by_Michael");
			if (!g_p_TextureManager->CreateCubeTextureFromBMPFiles(vecModelsInfo[i].tex[0][0],
				vecModelsInfo[i].tex[0][1], vecModelsInfo[i].tex[0][2],
				vecModelsInfo[i].tex[0][3], vecModelsInfo[i].tex[0][4],
				vecModelsInfo[i].tex[0][5], vecModelsInfo[i].tex[0][6],
				true, true))
			{
				std::cout << "Didn't load the sky box texture(s):" << std::endl;
				std::cout << ::g_p_TextureManager->getLastError() << std::endl;
			}
			totalTextures++;
			ExitOnGLError("Skymap didn't load.");
		}
		else if (vecModelsInfo[i].tex.size() > 0)
		{
			g_p_TextureManager->setBasePath("assets/textures");
			for (unsigned j = 0; j < vecModelsInfo[i].tex[0].size(); j++)
			{
				if (!g_p_TextureManager->Create2DTextureFromBMPFile(vecModelsInfo[i].tex[0][j], true))
				{
					std::cout << ::g_p_TextureManager->getLastError() << std::endl;
				}
				if (j == 0)
					vecModelsInfo[i].firstTex = totalTextures;
				totalTextures++;
			}
		}
	}

	return;
}

void ReadInModelsInfo(std::string fileLocation)
{
	std::ifstream myFile((fileLocation).c_str());

	//Check if file is open
	if (!myFile.is_open())
	{
		ExitOnGLError("ERROR: Could not open text file");
		return;
	}

	//Model tempModel;
	CVector3f tempLight;
	const Model blankModel;
	const CVector3f blankLight;
	std::string tempString;
	bool bKeepReading = true;

	myFile >> tempString;

	//Read in the file
	while (bKeepReading)
	{
		if (tempString == "<Model>")
		{
			Model tempModel;
			while (true)
			{
				myFile >> tempString;

				if (tempString == "<Type>")
				{
					myFile >> tempModel.type;
					myFile >> tempString;
				}
				else if (tempString == "<PlyFile>")
				{
					myFile >> tempModel.file;
					myFile >> tempString;
				}
				else if (tempString == "<Texture>")
				{
					myFile >> tempString;
					tempModel.tex.push_back(std::vector<std::string>(0));
					tempModel.tex[0].push_back(tempString);
					myFile >> tempString;
				}
				else if (tempString == "<Blend>")
				{
					myFile >> (unsigned int)tempModel.blend;
					myFile >> tempString;
				}
				else if (tempString == "<SkyboxTexture>")
				{
					myFile >> tempString;
					if (tempString == "<Name>")
					{
						myFile >> tempString;
						tempModel.tex.push_back(std::vector<std::string>(0));
						tempModel.tex[0].push_back(tempString);
						myFile >> tempString;
					}
					else
					{
						ExitOnGLError("ERROR: Formatting error in model reader.");
						return;
					}

					for (unsigned i = 0; i < 6; i++)
					{
						myFile >> tempString;
						if (tempString == "<Texture>")
						{
							myFile >> tempString;
							tempModel.tex[0].push_back(tempString);
							myFile >> tempString;
						}
						else
						{
							ExitOnGLError("ERROR: Formatting error in model reader.");
							return;
						}
					}
					myFile >> tempString;
				}
				else if (tempString == "<Rotation>")
				{
					myFile >> (float)tempModel.rot.x >> (float)tempModel.rot.y >> (float)tempModel.rot.z;
					myFile >> tempString;
				}
				else if (tempString == "<Colour>")
				{
					myFile >> (float)tempModel.col.x >> (float)tempModel.col.y >> (float)tempModel.col.z;
					myFile >> tempString;
				}
				else if (tempString == "<Scale>")
				{
					myFile >> (float)tempModel.scale;
					myFile >> tempString;
				}
				else if (tempString == "<Transparency>")
				{
					myFile >> (float)tempModel.transparency;
					if (tempModel.transparency < 1.0f)
						tempModel.isTransparent = true;
					myFile >> tempString;
				}
				else if (tempString == "<Position>")
				{
					myFile >> (float)tempModel.pos.x >> (float)tempModel.pos.y >> (float)tempModel.pos.z;
					myFile >> tempString;
				}
				else if (tempString == "<IsPlayer>")
				{
					myFile >> tempString;
					if (tempString == "True" || tempString == "true" || tempString == "TRUE")
						tempModel.isPlayer = true;
					else
						tempModel.isPlayer = false;
					myFile >> tempString;
				}
				else if (tempString == "<IsSkybox>")
				{
					myFile >> tempString;
					if (tempString == "True" || tempString == "true" || tempString == "TRUE")
						tempModel.isSkybox = true;
					else
						tempModel.isSkybox = false;
					myFile >> tempString;
				}
				else if (tempString == "<IsEnviro>")
				{
					myFile >> tempString;
					if (tempString == "True" || tempString == "true" || tempString == "TRUE")
						tempModel.isEnvironment = true;
					else
						tempModel.isEnvironment = false;
					myFile >> tempString;
				}
				else if (tempString == "<IsLightBall>")
				{
					myFile >> tempString;
					if (tempString == "True" || tempString == "true" || tempString == "TRUE")
						tempModel.isLightBall = true;
					else
						tempModel.isLightBall = false;
					myFile >> tempString;
				}
				else if (tempString == "<IsParticle>")
				{
					myFile >> tempString;
					if (tempString == "True" || tempString == "true" || tempString == "TRUE")
						tempModel.isParticle = true;
					else
						tempModel.isParticle = false;
					myFile >> tempString;
				}
				else if (tempString == "</Model>"){
					vecModelsInfo.push_back(tempModel);
					//tempModel = blankModel;
					break;
				}
				else
				{
					ExitOnGLError("ERROR: Formatting error in model section.");
					return;
				}
			}
		}
		else if (tempString == "<CheckPoint>")
		{
			while (true)
			{
				myFile >> tempString;

				if (tempString == "<Position>")
				{
					myFile >> (float)tempLight.x >> (float)tempLight.y >> (float)tempLight.z;
					myFile >> tempString;
				}
				
				else if (tempString == "</CheckPoint>")
					break;
				else
				{
					ExitOnGLError("ERROR: Formatting error in model section.");
					return;
				}
			}
		}

		if (tempString == "</Model>")
		{
			//vecModelsInfo.push_back(tempModel);
			//tempModel = blankModel;
		}
		else if (tempString == "</CheckPoint>")
		{
			vecCheckPointsInfo.push_back(tempLight);
			tempLight = blankLight;
		}
		else
		{
			ExitOnGLError("ERROR: Root model formatting error.");
			return;
		}

		myFile >> tempString;
		if (tempString != "<Model>" && tempString != "<CheckPoint>")
			bKeepReading = false;
	}
}