#ifndef _CXBoxController_HG_
#define _CXBoxController_HG_

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <XInput.h>		// for the actual controller

// Modified from: http://www.codeproject.com/Articles/26949/Xbox-360-Controller-Input-in-C-with-XInput

#include "IGameController.h"

#include <list>

// XInput Versions:
// http://msdn.microsoft.com/en-us/library/windows/desktop/hh405051(v=vs.85).aspx
// http://docs.nvidia.com/gameworks/content/gameworkslibrary/graphicssamples/d3d_samples/d3d_setup.htm

////#pragma comment(lib, "XInput.lib")
//#ifdef _DEBUG
//#pragma comment(lib, "XInput.lib")
//#else
//#pragma comment(lib, "XINPUT9_1_0.LIB")
//#endif

// The darn includes and dlls are different between the various versions of Windows... Grrrr.
// See: https://katyscode.wordpress.com/2013/08/30/xinput-tutorial-part-1-adding-gamepad-support-to-your-windows-game/
#ifndef _DEBUG
#pragma comment(lib, "XINPUT9_1_0.LIB")
#endif

class CXBoxController : public IGameController
{
public:
	CXBoxController( int playerNumber );
	~CXBoxController();



	virtual IGameController::enumDeadZoneCalculation getDeadZoneCalculation(void);
	virtual void setDeadZoneCalculation( IGameController::enumDeadZoneCalculation newDeadZoneCalculation);
	virtual std::string getDeadZoneCalculationString(void);
	virtual void setDeadZone(float newDeadZone);
	virtual float getDeadZone(void);

	XINPUT_STATE GetState(void);
	virtual bool bIsConnected(void);
	// Values from 0 to 65,535 (XBox controller)
	virtual void Vibrate( unsigned int intLeftVal, unsigned int intRightVal );
	// Values scaled from 0.0 to 1.0f
	virtual void Vibrate( float fLeftValNormalized, float fRightValNormalized );
	// Update has to be called for this to work
	virtual void AddVibrationSequence( const CVibStep &VibStep );
	virtual void AddVibrationSequence( CVibStep::enumMotor motor_, float speed_, float duration_ );
	virtual void AddVibrationSequence( const std::vector<CVibStep> &vecVibSequence );
	virtual void ClearVibrationSequence(void);
	// This is used for the vibration sequence
	virtual bool Update( float deltaSeconds );

	virtual int GetPlayerNumber(void);
	virtual bool bIsAButtonDown(void);	// Button 1
	virtual bool bIsBButtonDown(void);	// Button 2
	virtual bool bIsXButtonDown(void);	// Button 3
	virtual bool bIsYButtonDown(void);	// Button 4
	virtual bool bIsLeftButtonDown(void);	// Button 5
	virtual bool bIsRightButtonDown(void);	// Button 6
	virtual bool bIsBackButtonDown(void);	// Button 7
	virtual bool bIsStartButtonDown(void);	// Button 8
	virtual bool bIsLeftJoystickButtonDown(void);	// Button 9
	virtual bool bIsRightJoystickButtonDown(void);	// Button 10
	virtual float getLeftJoystickXAxis(void);
	virtual float getLeftJoystickYAxis(void);
	virtual float getLeftJoystickMagnitudeFromCentre(void);
	virtual float getRightJoystickXAxis(void);
	virtual float getRightJoystickYAxis(void);
	virtual float getRightJoystickMagnitudeFromCentre(void);
	virtual float getLeftTrigger(void);
	virtual float getRightTrigger(void);
	virtual bool bIsLeftTriggerDown(void);	// Depends on threshold
	virtual bool bIsRightTriggerDown(void);	// Depends on threshold
	virtual bool bIsGamePadLeft(void);
	virtual bool bIsGamePadRight(void);
	virtual bool bIsGamePadUp(void);
	virtual bool bIsGamePadDown(void);


private:
	CXBoxController() {};	// Can't call

	XINPUT_STATE m_State;
	int m_playerNumber;

	// These are the massaged values for the joysticks.
	void m_UpdateLeftJoystickValues(void);
	void m_UpdateRightJoystickValues(void);
	float m_LeftJoyX;
	float m_LeftJoyY;
	float m_LeftJoyMagnitude;
	float m_RightJoyX;
	float m_RightJoyY;
	float m_RightJoyMagnitude;

	enumDeadZoneCalculation m_deadZoneCalculation;
	void m_adjustForDeadZone(void);
	float m_deadZone;
	static const float DEAD_ZONE_DEFAULT;// = 0.2f;

	// http://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.reference.xinput_vibration(v=vs.85).aspx
	static const unsigned int MINVIBRATEMOTORSPEED = 0;
	static const unsigned int MAXVIBRATEMOTORSPEED = 65535;

	std::list<CVibStep> m_VibSequenceLeft;
	std::list<CVibStep> m_VibSequenceRight;
	float m_elapsedSeconds;

#ifdef _DEBUG
	// These are to get the process addresses so we don't have to link to the libraries any more
	// Gets the process addresses
	static bool Init(void);	 
	// Will be true once one controller is created.
	static bool bGetProcCompleted;
	static bool bCantResolveProcAddresses_SoAllIsLost;		// If this is false, then the controller won't work

	//      DWORD XInputGetState              ( _In_ DWORD dwUserIndex, _Out_ XINPUT_STATE *pState );
	typedef DWORD (WINAPI *XBoxInputGetState) ( DWORD dwUserIndex, XINPUT_STATE *pState );
	//      DWORD XInputSetState              ( _In_ DWORD dwUserIndex, _Inout_ XINPUT_VIBRATION *pVibration );
	typedef DWORD (WINAPI *XBoxInputSetState) ( DWORD dwUserIndex, XINPUT_VIBRATION *pVibration );

	static XBoxInputGetState pXboxInputGetState;
	static XBoxInputSetState pXboxInputSetState;
#endif

};

#endif