#include "CMouseState.h"
#include <sstream>

#include <gl\freeglut.h>


CMouseState::CMouseState()
{
	this->m_bLeftButtonDown = this->m_bMiddleButtonDown = this->m_bRightButtonDown = false;
	this->m_X = this->m_Y = 0;

	this->bKeepWheelButtonCombinationsSeparate = false;

	this->ResetScrollWheelValues();
}

void CMouseState::ResetScrollWheelValues(void)
{
	this->m_scrollWheel = 0;
	this->m_scrollWheel_OLD = 0;

	this->m_scrollWheel_Left = 0;
	this->m_scrollWheel_Middle = 0;
	this->m_scrollWheel_Right = 0;

	this->m_scrollWheel_Left_Middle = 0;			// Scroll wheel + left & middle buttons
	this->m_scrollWheel_Left_Right = 0;				// Scroll wheel + left & right buttons
	this->m_scrollWheel_Middle_Right = 0;			// Scroll wheel + middle & right buttons
	this->m_scrollWheel_Left_Middle_Right = 0;		// Scroll wheel + left & middle & right buttons
}

void CMouseState::UpdateStateFromGLUT( int x, int y )
{
	this->m_old_X = this->m_X;
	this->m_old_Y = this->m_Y;
	this->m_X = x;
	this->m_Y = y;
}

void CMouseState::UpdateStateFromGLUT( int button, int state, int x, int y )
{
	// NOTE: with the scroll wheel, GLUT sends a number of events that can 
	//	seem to make finding the 'current' state of the mouse very difficult.
	// For instance: 
	//	1 - you press the left mouse button 
	//			--> GLUT sends GLUT_LEFT_BUTTON with GLUT_DOWN
	//
	//  2 - WHILE the left button is down, you scroll the wheel up...
	//		(here's where it gets screwy)
	//			--> GLUT sends '5' (aka WHEEL_UP_LEFT) with GLUT_DOWN
	//			--> then IMMEDIATELY sends '5' again with GLUT_UP
	//		(then you say "Grrrr" in frustration)
	//
	// The good news: In fact, it's sort of clever. Since the mouse 
	//	button down and up events are sent as separate events, you 
	//	really don't have to pay attention to any of the combinations.
	// In other words, to keep the state of the buttons, only pay attention
	//	the actual lone button events and ignore anything else.
	//

	// Buttons first
	if ( button == GLUT_LEFT_BUTTON )
	{
		if ( state == GLUT_DOWN ) this->m_bLeftButtonDown = true; else this->m_bLeftButtonDown = false;
	}
	else if ( button == GLUT_MIDDLE_BUTTON )
	{
		if ( state == GLUT_DOWN) this->m_bMiddleButtonDown = true; else this->m_bMiddleButtonDown = false;
	}
	else if ( button == GLUT_RIGHT_BUTTON )
	{
		if ( state == GLUT_DOWN ) this->m_bRightButtonDown = true; else this->m_bRightButtonDown = false;
	}
	
	// The wheel - without buttons...
	if ( this->bKeepWheelButtonCombinationsSeparate )
	{	// Here we only update the SPECIFIC wheel + button combos...
		if ( (button == MOUSE_CONSTANTS::WHEEL_UP) && (state == GLUT_UP) )
		{
			this->m_scrollWheel_OLD = this->m_scrollWheel;
			this->m_scrollWheel++;
		}
		else if ( (button == MOUSE_CONSTANTS::WHEEL_UP_LEFT) && (state == GLUT_UP) )
		{
			this->m_scrollWheel_Left++;
		}
		else if ( (button == MOUSE_CONSTANTS::WHEEL_UP_MID) && (state == GLUT_UP) )
		{
			this->m_scrollWheel_Middle++;
		}
		else if ( (button == MOUSE_CONSTANTS::WHEEL_UP_RIGHT) && (state == GLUT_UP) )
		{
			this->m_scrollWheel_Right++;
		}
		else if ( (button == MOUSE_CONSTANTS::WHEEL_UP_LEFT_MID) && (state == GLUT_UP) )
		{
			this->m_scrollWheel_Left_Middle++;
		}
		else if ( (button == MOUSE_CONSTANTS::WHEEL_UP_LEFT_RIGHT) && (state == GLUT_UP) )
		{
			this->m_scrollWheel_Left_Right++;
		}
		else if ( (button == MOUSE_CONSTANTS::WHEEL_UP_MID_RIGHT) && (state == GLUT_UP) )
		{
			this->m_scrollWheel_Middle_Right++;
		}
		else if ( (button == MOUSE_CONSTANTS::WHEEL_UP_LEFT_MID_RIGHT) && (state == GLUT_UP) )
		{
			this->m_scrollWheel_Left_Middle_Right++;
		}
		else if ( (button == MOUSE_CONSTANTS::WHEEL_DOWN) && (state == GLUT_UP) )
		{
			this->m_scrollWheel_OLD = this->m_scrollWheel;
			this->m_scrollWheel--;
		}
		else if ( (button == MOUSE_CONSTANTS::WHEEL_DOWN_LEFT) && (state == GLUT_UP) )
		{
			this->m_scrollWheel_Left--;
		}
		else if ( (button == MOUSE_CONSTANTS::WHEEL_DOWN_MID) && (state == GLUT_UP) )
		{
			this->m_scrollWheel_Middle--;
		}
		else if ( (button == MOUSE_CONSTANTS::WHEEL_DOWN_RIGHT) && (state == GLUT_UP) )
		{
			this->m_scrollWheel_Right--;
		}
		else if ( (button == MOUSE_CONSTANTS::WHEEL_DOWN_LEFT_MID) && (state == GLUT_UP) )
		{
			this->m_scrollWheel_Left_Middle--;
		}
		else if ( (button == MOUSE_CONSTANTS::WHEEL_DOWN_LEFT_RIGHT) && (state == GLUT_UP) )
		{
			this->m_scrollWheel_Left_Right--;
		}
		else if ( (button == MOUSE_CONSTANTS::WHEEL_DOWN_MID_RIGHT) && (state == GLUT_UP) )
		{
			this->m_scrollWheel_Middle_Right--;
		}
		else if ( (button == MOUSE_CONSTANTS::WHEEL_DOWN_LEFT_MID_RIGHT) && (state == GLUT_UP) )
		{
			 this->m_scrollWheel_Left_Middle_Right--;
		}
	}
	else // *** DEFAULT BEHAVIOUR ***
	{	// Here we update the specific wheel + button combos AND the "subset" values
		// i.e. wheel+left+right would update scrollWheel_Left_Right, scrollWheel_Left,
		//	scrollWheel_Right, and scrollWheel together (default)
		if ( (button == MOUSE_CONSTANTS::WHEEL_UP) && (state == GLUT_UP) )
		{
			this->m_scrollWheel_OLD = this->m_scrollWheel;
			this->m_scrollWheel++;
		}
		else if ( (button == MOUSE_CONSTANTS::WHEEL_UP_LEFT) && (state == GLUT_UP) )
		{
			this->m_scrollWheel_Left++; //******
			this->m_scrollWheel_OLD = this->m_scrollWheel;
			this->m_scrollWheel++;
		}
		else if ( (button == MOUSE_CONSTANTS::WHEEL_UP_MID) && (state == GLUT_UP) )
		{
			this->m_scrollWheel_Middle++; //******
			this->m_scrollWheel_OLD = this->m_scrollWheel;
			this->m_scrollWheel++;
		}
		else if ( (button == MOUSE_CONSTANTS::WHEEL_UP_RIGHT) && (state == GLUT_UP) )
		{
			this->m_scrollWheel_Right++; //******
			this->m_scrollWheel_OLD = this->m_scrollWheel;
			this->m_scrollWheel++;
		}
		else if ( (button == MOUSE_CONSTANTS::WHEEL_UP_LEFT_MID) && (state == GLUT_UP) )
		{
			this->m_scrollWheel_Left_Middle++; //******
			this->m_scrollWheel_OLD = this->m_scrollWheel;
			this->m_scrollWheel++;
			this->m_scrollWheel_Left++;
			this->m_scrollWheel_Middle++;
		}
		else if ( (button == MOUSE_CONSTANTS::WHEEL_UP_LEFT_RIGHT) && (state == GLUT_UP) )
		{
			this->m_scrollWheel_Left_Right++; //******
			this->m_scrollWheel_OLD = this->m_scrollWheel;
			this->m_scrollWheel++;
			this->m_scrollWheel_Left++;
			this->m_scrollWheel_Right++;
		}
		else if ( (button == MOUSE_CONSTANTS::WHEEL_UP_MID_RIGHT) && (state == GLUT_UP) )
		{
			this->m_scrollWheel_Middle_Right++; //******
			this->m_scrollWheel_OLD = this->m_scrollWheel;
			this->m_scrollWheel++;
			this->m_scrollWheel_Right++;
			this->m_scrollWheel_Middle++;
		}
		else if ( (button == MOUSE_CONSTANTS::WHEEL_UP_LEFT_MID_RIGHT) && (state == GLUT_UP) )
		{
			this->m_scrollWheel_Left_Middle_Right++; //******
			this->m_scrollWheel_OLD = this->m_scrollWheel;
			this->m_scrollWheel++;
			this->m_scrollWheel_Left++;
			this->m_scrollWheel_Right++;
			this->m_scrollWheel_Middle++;
		}
		else if ( (button == MOUSE_CONSTANTS::WHEEL_DOWN) && (state == GLUT_UP) )
		{
			this->m_scrollWheel_OLD = this->m_scrollWheel;
			this->m_scrollWheel--; //******
		}
		else if ( (button == MOUSE_CONSTANTS::WHEEL_DOWN_LEFT) && (state == GLUT_UP) )
		{
			this->m_scrollWheel_Left--; //******
			this->m_scrollWheel_OLD = this->m_scrollWheel;
			this->m_scrollWheel--; 
		}
		else if ( (button == MOUSE_CONSTANTS::WHEEL_DOWN_MID) && (state == GLUT_UP) )
		{
			this->m_scrollWheel_Middle--; //******
			this->m_scrollWheel_OLD = this->m_scrollWheel;
			this->m_scrollWheel--; 
		}
		else if ( (button == MOUSE_CONSTANTS::WHEEL_DOWN_RIGHT) && (state == GLUT_UP) )
		{
			this->m_scrollWheel_Right--; //******
			this->m_scrollWheel_OLD = this->m_scrollWheel;
			this->m_scrollWheel--; 
		}
		else if ( (button == MOUSE_CONSTANTS::WHEEL_DOWN_LEFT_MID) && (state == GLUT_UP) )
		{
			this->m_scrollWheel_Left_Middle--; //******
			this->m_scrollWheel_OLD = this->m_scrollWheel;
			this->m_scrollWheel--; 
			this->m_scrollWheel_Left--;
			this->m_scrollWheel_Middle--;
		}
		else if ( (button == MOUSE_CONSTANTS::WHEEL_DOWN_LEFT_RIGHT) && (state == GLUT_UP) )
		{
			this->m_scrollWheel_Left_Right--; //******
			this->m_scrollWheel_OLD = this->m_scrollWheel;
			this->m_scrollWheel--; 
			this->m_scrollWheel_Left--;
			this->m_scrollWheel_Right--;
		}
		else if ( (button == MOUSE_CONSTANTS::WHEEL_DOWN_MID_RIGHT) && (state == GLUT_UP) )
		{
			this->m_scrollWheel_Middle_Right--; //******
			this->m_scrollWheel_OLD = this->m_scrollWheel;
			this->m_scrollWheel--; 
			this->m_scrollWheel_Right--;
			this->m_scrollWheel_Middle--;
		}
		else if ( (button == MOUSE_CONSTANTS::WHEEL_DOWN_LEFT_MID_RIGHT) && (state == GLUT_UP) )
		{
			this->m_scrollWheel_Left_Middle_Right--; //******
			this->m_scrollWheel_OLD = this->m_scrollWheel;
			this->m_scrollWheel--; 
			this->m_scrollWheel_Left--;
			this->m_scrollWheel_Right--;
			this->m_scrollWheel_Middle--;
		}
	} // if ( this->bKeepWheelButtonCombinationsSeparate )
}

int CMouseState::getScrollWheelDelta(void)
{
	return ( this->m_scrollWheel - this->m_scrollWheel_OLD );
}

bool CMouseState::isLeftButtonDown(void)
{
	return this->m_bLeftButtonDown;
}

bool CMouseState::isMiddleButtonDown(void)
{
	return this->m_bMiddleButtonDown;
}

bool CMouseState::isRightButtonDown(void)
{
	return this->m_bRightButtonDown;
}


std::string CMouseState::DEBUG_GetMouseStateString(void)
{
	std::stringstream ss;
	ss << "x,y=" << this->m_X << "," << this->m_Y << " "
		<< "Buttons: ";
	if ( this->m_bLeftButtonDown )	ss << "DOWN "; else ss << "UP ";
	if ( this->m_bMiddleButtonDown ) ss << "DOWN "; else ss << "UP ";
	if ( this->m_bRightButtonDown ) ss << "DOWN "; else ss << "UP ";
	ss << "SW:" << this->m_scrollWheel;
	return ss.str();
}

std::string CMouseState::DEBUG_GetMouseStateStringAll(void)
{
	std::stringstream ss;
	ss << "x,y=" << this->m_X << "," << this->m_Y << " "
		<< "Buttons: ";
	if ( this->m_bLeftButtonDown )	ss << "DOWN "; else ss << "UP ";
	if ( this->m_bMiddleButtonDown ) ss << "DOWN "; else ss << "UP ";
	if ( this->m_bRightButtonDown ) ss << "DOWN "; else ss << "UP ";
	ss 	<< "SW:" << this->m_scrollWheel << " " << std::endl
		<< "SW+L:" << this->m_scrollWheel_Left << " "
		<< "SW+M:" << this->m_scrollWheel_Middle << " "
		<< "SW+R:" << this->m_scrollWheel_Right << " "
		<< "SW+L+M:" << this->m_scrollWheel_Left_Middle << " "
		<< "SW+L+R:" << this->m_scrollWheel_Left_Right << " "
		<< "SW+M+R:" << this->m_scrollWheel_Middle_Right << " "
		<< "SW+L+R+M:" << this->m_scrollWheel_Left_Middle_Right;
	return ss.str();

}

int CMouseState::getX(void)
{
	return this->m_X;
}

int CMouseState::getY(void)
{
	return this->m_Y;
}

int CMouseState::getOldX(void)
{
	return this->m_old_X;
}

int CMouseState::getOldY(void)
{
	return this->m_old_Y;
}

int CMouseState::getDeltaX(void)
{
	return (this->m_X - this->m_old_X);
}

int CMouseState::getDeltaY(void)
{
	return (this->m_Y - this->m_old_Y);
}

int CMouseState::getScrollWheel(void)
{
	return this->m_scrollWheel;
}

int CMouseState::getScrollWheel_Left(void)
{
	return this->m_scrollWheel_Left;
}

int CMouseState::getScrollWheel_Middle(void)
{
	return this->m_scrollWheel_Middle;
}

int CMouseState::getScrollWheel_Right(void)
{
	return this->m_scrollWheel_Right;
}

int CMouseState::getScrollWheel_Left_Middle(void)
{
	return this->m_scrollWheel_Left_Middle;
}

int CMouseState::getScrollWheel_Left_Right(void)
{
	return this->m_scrollWheel_Left_Right;
}

int CMouseState::getScrollWheel_Middle_Right(void)
{
	return this->m_scrollWheel_Middle_Right;
}

int CMouseState::getScrollWheel_Left_Middle_Right(void)
{
	return this->m_scrollWheel_Left_Middle_Right;
}



