#include "global.h"

static int oldScrollWheelValue = 0;

void glutPassiveMotionCallback( int x, int y );

static const float lookSpeedScale = 0.005f;


void glutMouseCallback(int button, int state, int x, int y)
{
	//g_MouseState.bKeepWheelButtonCombinationsSeparate = true;
	::g_pMouseState->UpdateStateFromGLUT( button, state, x, y );

	//// Update the light
	if ( oldScrollWheelValue != ::g_pMouseState->getScrollWheel() )
	{
		// Update the automatic texture coordinate values for the bunny... or dolphin...
		float deltaScroll = static_cast<float>( oldScrollWheelValue - g_pMouseState->getScrollWheel() );

		//g_theCamera.z += ( g_theCamera.moveSpeed * deltaScroll );
		//g_theCamera.Move( 0.1f * deltaScroll );

		oldScrollWheelValue = ::g_pMouseState->getScrollWheel();
	}



	//if ( ::g_pMouseState->isLeftButtonDown() )
	//{
	//	static const float cameraSpeedForward = 0.5f;
	//	::g_pCamera->MoveForwardBackward( cameraSpeedForward );
	//}

	//// Move mouse (no matter what buttons) moves camera
	//static const float lookSpeedScale = 0.01f;
	//::g_pCamera->AdjustYaw( g_pMouseState->getDeltaX() * lookSpeedScale );
	//::g_pCamera->AdjustPitch( g_pMouseState->getDeltaY() * lookSpeedScale );
	//::g_pCamera->calcTargetFromOrientation();

}

// Called when a mouse button is down
void glutMotionCallback( int x, int y )
{
	::g_pMouseState->UpdateStateFromGLUT( x, y );


	// Move mouse (no matter what buttons) moves camera
	::g_pCamera->AdjustYaw( -g_pMouseState->getDeltaX() * lookSpeedScale );
	::g_pCamera->AdjustPitch( g_pMouseState->getDeltaY() * lookSpeedScale );
	::g_pCamera->calcTargetFromOrientation();


	if ( ::g_pMouseState->isLeftButtonDown() )
	{
	}
	if ( ::g_pMouseState->isMiddleButtonDown() )
	{
	}


}

// Called whenever mouse is in window (and buttons are UP)
void glutPassiveMotionCallback( int x, int y )
{
	::g_pMouseState->UpdateStateFromGLUT( x, y );
	//std::cout << g_MouseState.DEBUG_GetMouseStateString() << std::endl;

	//if ( ::g_pMouseState->isLeftButtonDown() )
	//{
	//	static const float cameraSpeedForward = 0.5f;
	//	::g_pCamera->MoveForwardBackward( cameraSpeedForward );
	//}

	// Move mouse (no matter what buttons) moves camera
	/*::g_pCamera->AdjustYaw( g_pMouseState->getDeltaX() * lookSpeedScale );
	::g_pCamera->AdjustPitch( g_pMouseState->getDeltaY() * lookSpeedScale );
	::g_pCamera->calcTargetFromOrientation();*/

}