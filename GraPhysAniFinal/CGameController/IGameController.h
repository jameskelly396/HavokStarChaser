#ifndef _IGameController_HG_
#define _IGameController_HG_

// General controller class for game controllers
#include <vector>
#include <string>

class IGameController
{
public:
	virtual ~IGameController() {};
	
	virtual int GetPlayerNumber(void) = 0;
	virtual bool bIsConnected(void) = 0;

	enum enumDeadZoneCalculation
	{
		NO_DEAD_ZONE,
		AXIAL_DEAD_ZONE,
		RADIAL_DEAD_ZONE,
		SCALED_RADIAL_DEAD_ZONE		// Default
	};
	virtual IGameController::enumDeadZoneCalculation getDeadZoneCalculation(void) = 0;
	virtual void setDeadZoneCalculation( IGameController::enumDeadZoneCalculation newDeadZoneCalculation) = 0;
	virtual std::string getDeadZoneCalculationString(void) = 0;
	virtual void setDeadZone(float newDeadZone) = 0;
	virtual float getDeadZone(void) = 0;

	// Values from 0 to 65,535 (XBox controller)
	virtual void Vibrate( unsigned int intLeftVal, unsigned int intRightVal ) = 0;
	// Values scaled from 0.0 to 1.0f
	virtual void Vibrate( float fLeftValNormalized, float fRightValNormalized ) = 0;

	struct CVibStep
	{
	public:
		enum enumMotor { LEFT, RIGHT };
		enumMotor motor;
		float durationSeconds;
		float fSpeed;
		CVibStep( enumMotor motor_, float fSpeed_, float duration_ ) : 
			motor( motor_), fSpeed( fSpeed_ ), durationSeconds( duration_ ) {};
	private:
		CVibStep() {};
	};
	// Update has to be called each frame for this to work
	virtual void AddVibrationSequence( const CVibStep &VibStep ) = 0;
	virtual void AddVibrationSequence( CVibStep::enumMotor motor_, float speed_, float duration_ ) = 0;
	virtual void AddVibrationSequence( const std::vector<CVibStep> &vecVibSequence ) = 0;
	virtual void ClearVibrationSequence(void) = 0;
	// This is used for the vibration sequence
	virtual bool Update( float deltaSeconds ) = 0;


	virtual bool bIsAButtonDown(void) = 0;	// Button 1
	virtual bool bIsBButtonDown(void) = 0;	// Button 2
	virtual bool bIsXButtonDown(void) = 0;	// Button 3
	virtual bool bIsYButtonDown(void) = 0;	// Button 4
	virtual bool bIsLeftButtonDown(void) = 0;	// Button 5
	virtual bool bIsRightButtonDown(void) = 0;	// Button 6
	virtual bool bIsBackButtonDown(void) = 0;	// Button 7
	virtual bool bIsStartButtonDown(void) = 0;	// Button 8
	virtual bool bIsLeftJoystickButtonDown(void) = 0;	// Button 9
	virtual bool bIsRightJoystickButtonDown(void) = 0;	// Button 10
	virtual float getLeftJoystickXAxis(void) = 0;
	virtual float getLeftJoystickYAxis(void) = 0;
	virtual float getLeftJoystickMagnitudeFromCentre(void) = 0;
	virtual float getRightJoystickXAxis(void) = 0;
	virtual float getRightJoystickYAxis(void) = 0;
	virtual float getRightJoystickMagnitudeFromCentre(void) = 0;
	virtual float getLeftTrigger(void) = 0;
	virtual float getRightTrigger(void) = 0;
	virtual bool bIsLeftTriggerDown(void) = 0;	// Depends on threshold
	virtual bool bIsRightTriggerDown(void) = 0;	// Depends on threshold
	virtual bool bIsGamePadLeft(void) = 0;
	virtual bool bIsGamePadRight(void) = 0;
	virtual bool bIsGamePadUp(void) = 0;
	virtual bool bIsGamePadDown(void) = 0;
};

#endif
