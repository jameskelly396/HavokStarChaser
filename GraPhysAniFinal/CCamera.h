#ifndef _CCamera_HG_
#define _CCamera_HG_

#include "CCommonTypes/CVector3f.h"		// Basically a vec3

#include "IMediator.h"		// Allow camera to find game objects

#include <glm/gtc/quaternion.hpp>	// I'd suggest that you use this one

// This was set up so the camera could have a number of modes. 
// (Keep in mind that you could combine the settings here in unusual ways, 
//  and get, well, strange effects perhaps. Use at your own risk, I guess.)
//
// 1. Independent, free look (INDEPENDENT_FREE_LOOK):
//       This is where you can "fly" the camera around the scene, like a plane.
//       INDEPENDENT_FREE_LOOK mode is all you need. 
//       The "up" vector is set to face the same way all the time.
// 
// 2. Follow target at a distance (FOLLOW_TARGET_AT_DISTANCE_CHASE_PLANE)
//       This is sort of a "chase plane" sort of mode. You set how far 
//       behind the target you want. There are two ranges, one where the camera 
//       will stop, and one (further out) where the camera will go full speed.
//       In between these ranges, the camera will gradulally (smoothly) slow down. 
//       If bFollowPointBasedOnTargetOrientation = false, then the camera will 
//       stop when its in range (no matter where it is around the target - behind, 
//       below, whatever). If bFollowPointBasedOnTargetOrientation = true, then 
//       the "stop" point will be relative the orientation of the object. For instance,
//       if you want to follow the target "above and behind" the object, you would set 
//       the followTargetToCameraAngle to some value relative to the object. 
//       If bUpVectorBasedOnTargetOrientation = false (default), the cameras "up" 
//       won't be changed. But if true, then the camera will rotate with the object.
// 
// 3. LOCK_TO_TARGET
//       This is for first and third person cameras. Location and orientation 
//       changes with target (matches). The "Move" methods don't work when this 
//       is active (move left, right, etc.), but the Adjust (for the pitch) 
//       will change the relative direction - i.e. a pilot looking around
//       a cockpit, for instance. 
//       The minFollowDistance controls how "close" the camera gets. If it's
//       zero (0), then the camera is clamped to the target. If it's non-zero,
//       it "orbits" the target (like the camera Ratchet and Clank).

class CCamera 
{
public:
	CCamera();
	~CCamera();

	enum enumCameraStates
	{
		INDEPENDENT_FREE_LOOK,
		FOLLOW_TARGET_AT_DISTANCE_CHASE_PLANE,
		LOCK_TO_TARGET
	};
	enumCameraStates cameraMode;

	enumCameraStates getCameraState(void);
	std::string getCameraStateString(void);

	void setMode_IndependentFreeLook(void);

	void setMode_FollowAtDistance(void);
	void setMode_FollowAtDistance( unsigned int ID_to_Follow );

	void setMode_LockToTarget(void);
	void setMode_LockToTarget( unsigned int ID_to_Follow );

	void setFollowMaxSpeed(float newMaxFollowSpeed);
	void setFollowMinDistance(float newMinDistance);
	void setFollowMaxSpeedDistance(float newMaxSpeedDistance);
	void setFollowDistances(float newMinDistance, float newMaxSpeedDistance);
	void setFollowTargetToCameraAngle( float xAngle, float yAngle, float zAngle );
	void setFollowTargetToCameraAngle( glm::fquat newAngle );

	// Returns false if it can't find the object
	bool setFollowTarget( unsigned int ID_to_Follow );

	void SetMediator( IMediator* pTheMediator );

	CVector3f eye;
	CVector3f target;
	CVector3f up;

	void calcTargetFromOrientation(void);

	// This is only updated if we are manually controlling the camera, 
	//	like if the camera is placed at the same location as the object
	//  (like "in the cockpit" for example)
	glm::fquat orientation;	


	// Called in the "idle" or "simulation step"
	void Update(float deltaSeconds);

	// replaces bFollowToHeight and minHeight
	glm::fquat followTargetToCameraAngle;		

	unsigned int object_to_follow_ID;
	float minFollowDistance;
	float maxSpeedDistance;
	float maxFollowSpeed;
	//float minHeight;
	// If true, this will rotate relative to the target, 
	//	for example, you could always follow "behind" or "beside" the target.
	//  The followTargetToCameraAngle is then used in combination with this.
	// If false, only the followTargetToCameraAngle is used
	// (i.e. the orientation of the target is ignored)
	bool bFollowPointBasedOnTargetOrientation;		

	//used for moving the camera when not locked on player
	float deltaMove;
	float cameraMove;
	float deltaAngleYaw;
	float cameraAngleYaw;
	float deltaAnglePitch;
	float cameraAnglePitch;

	// Like the follow point, this changes the way the direct movement works. 
	// If false, the movement is based on its own orientation
	// If true, the movement is basd on the targets orientation
	bool bUpVectorBasedOnTargetOrientation;


	// Rotation around y-axis (object relative) - "spin" like a child's toy "top"
	void AdjustYaw( float changeInDegrees );
	// Rotation around x-axis (object relative) - airplane "nose down" or "nose up" 
	void AdjustPitch( float changeInDegrees );
	// Rotation around z-axis (object relative) - airplane "barrel roll" from "side to side"
	void AdjustRoll( float changeInDegrees );

	// -ve is Left, +ve is Right
	void MoveLeftRight( float amount );
	// +ve is Forward, -ve is Backward
	void MoveForwardBackward( float amount );
	// +ve is Up, -ve is Down
	void MoveUpDown( float amount );

	void genBasicAccelCurve(void);
	// Does the look up in the table of values. Also performs LERP
	float getLERPValue( float lookup );

	enum enumLEFPMode
	{
		ROUND_DOWN = 1000,		// So it doesn't compare with previous enum
		PICK_CLOSEST,
		LERP
	};
	enumLEFPMode m_LEFPLookupMode;

private:
	void m_Init(void);		// called by constructor

	IMediator* m_pTheMediator;

	static const int ACC_CURVE_ARRAY_SIZE = 101;
	float m_accCurve[ACC_CURVE_ARRAY_SIZE];

	float m_calcSpeedStepSpeedSingleAxis( float target, float eye, float deltaTime );
};

#endif