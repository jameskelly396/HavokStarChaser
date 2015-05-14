#ifndef _CMouseState_HG_
#define _CMouseState_HG_

// Written by Michael Feeney, Fanshawe College, 2010
// mfeeney@fanshawec.on.ca
// It may be distributed under the terms of the General Public License:
// http://www.fsf.org/licenses/gpl.html
// Use this code at your own risk. It is indented only as a learning aid.
//
#include <string>

// Here's the breakdown of my buttons...
//
//	Left button (alone) 	= 0		GLUT_LEFT_BUTTON
//	Middle button (alone) 	= 1		GLUT_MIDDLE_BUTTON
//	Right button (alone)	= 2		GLUT_RIGHT_BUTTON
namespace MOUSE_CONSTANTS
{
	const int WHEEL_UP			= 3;
	const int WHEEL_DOWN		= 4;

	const int WHEEL_UP_LEFT			= 5;
	const int WHEEL_UP_MID			= 35;
	const int WHEEL_UP_RIGHT		= 7;
	const int WHEEL_UP_LEFT_MID		= 37;
	const int WHEEL_UP_LEFT_RIGHT	= 9;
	const int WHEEL_UP_MID_RIGHT	= 39;
	const int WHEEL_UP_LEFT_MID_RIGHT	= 41;

	const int WHEEL_DOWN_LEFT		= 6;
	const int WHEEL_DOWN_MID		= 36;
	const int WHEEL_DOWN_RIGHT		= 8;
	const int WHEEL_DOWN_LEFT_MID	= 38;
	const int WHEEL_DOWN_LEFT_RIGHT	= 10;
	const int WHEEL_DOWN_MID_RIGHT	= 40;
	const int WHEEL_DOWN_LEFT_MID_RIGHT	= 42;
}

class CMouseState
{
public:
	CMouseState();
	void UpdateStateFromGLUT( int button, int state, int x, int y );
	void UpdateStateFromGLUT( int x, int y );
	void ResetScrollWheelValues(void);
	int getX(void);		int getY(void);
	int getOldX(void);	int getOldY(void);
	bool isLeftButtonDown(void);
	bool isMiddleButtonDown(void);
	bool isRightButtonDown(void);
	int getDeltaX(void); int getDeltaY(void);
	// When this flag is true, the exact combinations of wheel + button
	//	will update the corresponding counters. 
	//	For instance, if you scroll up with Left + Right buttons, 
	//	ONLY the scrollWheel_Left_Right is changed.
	//
	// When this flag is false (the default), the various counters are 
	//	updated together. 
	//	For instance, if you scroll up with the Left + Right buttons,
	//	scrollWheel_Left, scrollWheel_Right, AND scrollWheel_Left_Right 
	//	are all updated together. 
	bool bKeepWheelButtonCombinationsSeparate;
	int getScrollWheel(void);
	int getScrollWheelDelta(void);

	int getScrollWheel_Left(void);
	int getScrollWheel_Middle(void);
	int getScrollWheel_Right(void);
	int getScrollWheel_Left_Middle(void);
	int getScrollWheel_Left_Right(void);
	int getScrollWheel_Middle_Right(void);
	int getScrollWheel_Left_Middle_Right(void);
	//
	std::string DEBUG_GetMouseStateString(void);
	std::string DEBUG_GetMouseStateStringAll(void);
private:
	int m_X;
	int m_Y;
	bool m_bLeftButtonDown;
	bool m_bMiddleButtonDown;
	bool m_bRightButtonDown;
	int m_old_X;
	int m_old_Y;
	int m_scrollWheel;
	int m_scrollWheel_OLD;
	int m_scrollWheel_Left;					// Scroll wheel + left button
	int m_scrollWheel_Middle;				// Scroll wheel + middle button
	int m_scrollWheel_Right;				// Scroll wheel + right button
	int m_scrollWheel_Left_Middle;			// Scroll wheel + left & middle buttons
	int m_scrollWheel_Left_Right;			// Scroll wheel + left & right buttons
	int m_scrollWheel_Middle_Right;			// Scroll wheel + middle & right buttons
	int m_scrollWheel_Left_Middle_Right;	// Scroll wheel + left & middle & right buttons
};

#endif
