#include "global.h"

#include <iostream>

bool InitWindow(int argc, char* argv[], int screenWidth, int screenHeight)
{
	glutInit(&argc, argv);

	::g_screenWidth = screenWidth;
	::g_screenHeight = screenHeight;
  
	glutInitContextVersion(4, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(
		GLUT_ACTION_ON_WINDOW_CLOSE,
		GLUT_ACTION_GLUTMAINLOOP_RETURNS		// Returns control to the main
	);
  
	glutInitWindowSize( ::g_screenWidth, ::g_screenHeight );

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	//WindowHandle = glutCreateWindow(WINDOW_TITLE_PREFIX);
	::g_windowHandle = glutCreateWindow( ::g_WindowTitleDefault.c_str() );

	if ( ::g_windowHandle < 1 )	//  if(WindowHandle < 1) 
	{
		//fprintf(
		//	stderr,
		//	"ERROR: Could not create a new rendering window.\n"
		//);
		std::cout << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
  
	// Tells GLUT what functions to call... ("callback" functions)
	glutReshapeFunc(glutReshape_callback);			// glutReshapeFunc(ResizeFunction);
	glutDisplayFunc(glutDisplay_callback);			// glutDisplayFunc(RenderFunction);
	glutIdleFunc(IdleFunction);
	glutTimerFunc(0, TimerFunction, 0);
	glutCloseFunc(glutClose_callback);				// glutCloseFunc(DestroyCube);

	glutKeyboardFunc( glutKeyboard_callback );		// glutKeyboardFunc( keyboardFunctionCallback );
	glutSpecialFunc( glutSpecialKey_callback );		// glutSpecialFunc( specialKeyCallback );
	glutKeyboardUpFunc(glutKeyboardUp_callback);
	glutSpecialUpFunc(glutSpecialUp_callback);

	// Mouse functions
	::glutMouseFunc(glutMouseCallback);
	::glutMotionFunc(glutMotionCallback);
	::glutPassiveMotionFunc(glutPassiveMotionCallback);


	return true;
}

bool OpenGL_Initialize(int argc, char* argv[], int screenWidth, int screenHeight)
{
	GLenum GlewInitResult;

	if (! InitWindow(argc, argv, screenWidth, screenHeight) )
	{
		return false;	// Error
	}	
  
	glewExperimental = GL_TRUE;
	GlewInitResult = glewInit();

	if (GLEW_OK != GlewInitResult) 
	{
		//fprintf(
		//  stderr,
		//  "ERROR: %s\n",
		//  glewGetErrorString(GlewInitResult)
		//);
		std::cout << "ERROR: " << glewGetErrorString(GlewInitResult) << std::endl;
		exit(EXIT_FAILURE);
	}
  
	  //fprintf(
	  //  stdout,
	  //  "INFO: OpenGL Version: %s\n",
	  //  glGetString(GL_VERSION)
	  //);
	std::cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	glGetError();
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	ExitOnGLError("ERROR: Could not set OpenGL depth testing options");

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	ExitOnGLError("ERROR: Could not set OpenGL culling options");

	//ModelMatrix = IDENTITY_MATRIX;
	//ProjectionMatrix = IDENTITY_MATRIX;
	//ViewMatrix = IDENTITY_MATRIX;

	// This line is moved into the Render function, because we are doing "lookAt"
	//TranslateMatrix(&ViewMatrix, 0, 0, -2);

	//if ( !CreateCube() ) 
	//{
	//	return false;	// Error
	//}



	return true;
}