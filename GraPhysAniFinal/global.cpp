#include "global.h"

// ***************************************
CParticleEmitter* g_p_checkpointEmitter;
CParticleEmitter* g_p_timerEmitter;
// ***************************************

//Havok headers
// Keycode
//#include <Common/Base/keycode.cxx>
//#include <Common/Base/Config/hkProductFeatures.cxx>

IPhysics* g_pPhysicsManager;

CShaderUniformVariables g_ShaderUniformVariables;

//glm::mat4 g_matProjection = glm::mat4(1.0f);	// 1.0f gives identity matrix
//glm::mat4 g_matView = glm::mat4(1.0f);
//glm::mat4 g_matWorld = glm::mat4(1.0f);		// aka "Model" matrix

//GLuint g_ProjectionMatrixUniformLocation = 0;			
//GLuint g_ViewMatrixUniformLocation = 0;				
//GLuint g_ModelMatrixUniformLocation = 0;				
//GLuint g_ObjectColourUniformLocation = 0;				

GLuint g_slot_LightPosition = 0;	//uniform vec4 LightPosition;	
GLuint g_slot_LightColour = 0;	//uniform vec4 LightColour;		
GLuint g_slot_attenuation = 0;	//uniform float attenuation;	


// Same sort of thing as the "name" of the buffer, but these are the 
// "names" of the shaders we are loading
//GLuint g_ShaderIds[3] = { 0 };		
// Or these might be a little more clear...
GLuint g_ShaderProgram_ID = 0;
GLuint g_VertexShader_ID = 0;
GLuint g_FragmentShader_ID = 0;


IGLShaderManager* g_pShaderManager = 0;

CTextureManager* g_p_TextureManager = 0;

std::map<std::string, std::string> g_mapModelTypes;
std::map<unsigned int, std::vector<std::string> > g_mapTextures;

int g_screenWidth = 1400;
int g_screenHeight = 900;
int g_windowHandle = 0;
unsigned int g_FrameCount = 0;
clock_t g_LastTime = 0;	

// ***********************************************
// *** WE'LL BE MOVING THIS STUFF OUT OF HERE ****
//int g_numberOfTriangles[2] = {0}; 
//GLuint g_VertexPositionBufferID[2] = {0};		// BufferIds[0]		
//GLuint g_VertexColourBufferID[2] = {0};			// BufferIds[1]		
//GLuint g_IndexBufferID[2] = {0};					// BufferIds[2]		
// ***********************************************

//Havok variables
//hkpPhysicsContext* g_pContext;
//hkVisualDebugger* g_pVdb;
//
//hkJobQueue* g_pJobQueue;
//hkJobThreadPool* g_pThreadPool;
//
//hkpWorld* g_pWorld;			// Physics world.
//hkpWorldCinfo g_pWorldInfo; // Info about global simulation parameters.
//bool g_bVdbEnabled = true;

// TEXTURE IDs
//GLuint g_texture_0_ID = 0;
//GLuint g_texture_1_ID = 0;
//GLuint g_texture_skymap = 0;

GLuint g_sampler_0 = 0;
GLuint g_sampler_1 = 0;
GLuint g_sampler_skymap = 0;

// Some texturing stuff:
//CTextureFromBMP* g_p_Texture00 = 0;
//CTextureFromBMP* g_p_Texture01 = 0;
//CTextureFromBMP* g_p_TextureSkyMap = 0;

float g_MyMixValue = 0.0f;

CCamera* g_pCamera = 0;

CMouseState* g_pMouseState = 0;

CModelLoaderManager*  g_pModelLoader = 0;	// set pointers to zero just in case

CFactoryMediator* g_pFactoryMediator = 0;			
unsigned int g_Galactica_ID = 0;
unsigned int g_Player_ID = 0;				// Used to locate the player
unsigned int g_lightModelID = 0;
unsigned int g_StarHavokID = 0;
unsigned int g_FloorHavokID = 0;
unsigned int g_skyBoxID = 0;
unsigned int g_enviroID = 0;
unsigned int g_ParticleID = 0;
std::vector<unsigned int> g_vecParticleID;

unsigned int g_tire1ID = 0;
unsigned int g_tire2ID = 0;
unsigned int g_tire3ID = 0;
unsigned int g_tire4ID = 0;

std::vector<Model> vecModelsInfo;      //list of all the loaded objects
std::vector<CVector3f> vecCheckPointsInfo;

std::map< unsigned int, CAABB* > g_mapCAABB;

CHRTimer g_simTimer;

CLightManager* g_p_LightManager = 0;
bool isFocusedOnLight = false;
bool drawHitbox = false;
bool drawParticles = false;
bool g_bApplyBrakes = false;
bool g_SavingData = false;

// *****************************
// *** For the Physics demo ****
// These are the "debug" objects (the balls, etc.) that we are also drawing
std::vector< CGameObject* > g_vecPhysDebugObjects;
std::vector< CGameObject* > g_vecShipHitBoxes;
// *****************************


#include <iostream>	// for LoadShader, which we'll be replacing soon
void ExitOnGLError(const char* error_message)
{
  const GLenum ErrorValue = glGetError();

  if (ErrorValue != GL_NO_ERROR)
  {
    //fprintf(stderr, "%s: %s\n", error_message, gluErrorString(ErrorValue));
	std::cout << gluErrorString(ErrorValue) << std::endl;
    exit(EXIT_FAILURE);
  }
}


CGameState g_gameState;

CGameControllerManager*		g_p_GameControllerManager = 0;
IGameController*			g_p_GameController_0 = 0;


CPhysicsCalculatron*		g_p_PhysicsThingy = 0;


//GLuint LoadShader(const char* filename, GLenum shader_type)
//{
//  GLuint shader_id = 0;
//  FILE* file;
//  long file_size = -1;
//  char* glsl_source;		// std::string shaderSourceText;
//
//  if (NULL != (file = fopen(filename, "rb")) &&
//    0 == fseek(file, 0, SEEK_END) &&
//    -1 != (file_size = ftell(file)))
//  {
//    rewind(file);
//    
//    if (NULL != (glsl_source = (char*)malloc(file_size + 1)))
//    {
//      if (file_size == (long)fread(glsl_source, sizeof(char), file_size, file))
//      {
//        glsl_source[file_size] = '\0';
//
//        if (0 != (shader_id = glCreateShader(shader_type)))
//        {
//          glShaderSource(shader_id, 1, &glsl_source, NULL);
//          glCompileShader(shader_id);
//          ExitOnGLError("Could not compile a shader");
//        }
//        else
//          fprintf(stderr, "ERROR: Could not create a shader.\n");
//      }
//      else
//        fprintf(stderr, "ERROR: Could not read file %s\n", filename);
//
//      free(glsl_source);
//    }
//    else
//      fprintf(stderr, "ERROR: Could not allocate %i bytes.\n", file_size);
//
//    fclose(file);
//  }
//  else
//  {
//    if (NULL != file)
//      fclose(file);
//    fprintf(stderr, "ERROR: Could not open file %s\n", filename);
//  }
//
//  return shader_id;
//}