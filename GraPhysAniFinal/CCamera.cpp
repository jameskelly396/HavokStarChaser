#include "CCamera.h"

#include <iostream>

#include <math.h>		// For sqrt()

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

	//static const float m_defAccCurve[ACC_CURVE_ARRAY_SIZE] = 
	//{
	//	0, 0.00024672, 0.000986636, 0.002219018, 0.003942649, 0.00615583, 0.008856375, 
	//	0.012041619, 0.015708419, 0.019853157, 0.024471742, 0.029559616, 0.035111757, 
	//	0.041122687, 0.047586474, 0.054496738, 0.06184666, 0.069628986, 0.077836037, 
	//	0.086459713, 0.095491503, 0.104922494, 0.114743379, 0.124944465, 0.135515686, 
	//	0.146446609, 0.157726447, 0.169344067, 0.181288005, 0.193546473, 0.206107374, 
	//	0.218958311, 0.232086603, 0.245479292, 0.259123163, 0.27300475, 0.287110354, 
	//	0.301426055, 0.315937724, 0.33063104, 0.345491503, 0.360504447, 0.375655056, 
	//	0.390928379, 0.406309343, 0.421782767, 0.437333383, 0.452945843, 0.46860474, 
	//	0.48429462, 0.5, 0.51570538, 0.53139526, 0.547054157, 0.562666617, 0.578217233, 
	//	0.593690657, 0.609071621, 0.624344944, 0.639495553, 0.654508497, 0.66936896, 
	//	0.684062276, 0.698573945, 0.712889646, 0.72699525, 0.740876837, 0.754520708, 
	//	0.767913397, 0.781041689, 0.793892626, 0.806453527, 0.818711995, 0.830655933, 
	//	0.842273553, 0.853553391, 0.864484314, 0.875055535, 0.885256621, 0.895077506, 
	//	0.904508497, 0.913540287, 0.922163963, 0.930371014, 0.93815334, 0.945503262, 
	//	0.952413526, 0.958877313, 0.964888243, 0.970440384, 0.975528258, 0.980146843, 
	//	0.984291581, 0.987958381, 0.991143625, 0.99384417, 0.996057351, 0.997780982, 
	//	0.999013364, 0.99975328, 1 };

CCamera::CCamera()
{
	this->m_pTheMediator = 0;		// If mediator == 0, don't use it

	this->deltaMove = 0.0f;
	this->cameraMove = 0.0f;
	this->deltaAngleYaw = 0.0f;
	this->cameraAngleYaw = 0.0f;
	this->deltaAnglePitch = 0.0f;
	this->cameraAnglePitch = 0.0f;

	this->m_LEFPLookupMode = CCamera::LERP;

	this->m_Init();

	return;
}

void CCamera::m_Init(void)
{
	this->cameraMode = CCamera::INDEPENDENT_FREE_LOOK;


	// replaces bFollowToHeight and minHeight
	glm::fquat followTargetToCameraAngle;		
	this->object_to_follow_ID = 0;
	this->minFollowDistance = 0.0f;
	this->maxSpeedDistance = 10.0f;
	this->maxFollowSpeed = 0.0f;
	this->bFollowPointBasedOnTargetOrientation = false;

	this->bUpVectorBasedOnTargetOrientation = false;

	this->genBasicAccelCurve();

	return;
}

CCamera::~CCamera()
{
	return;
}

void CCamera::setMode_IndependentFreeLook(void)
{
	this->cameraMode = CCamera::INDEPENDENT_FREE_LOOK;
	return;
}

void CCamera::setMode_FollowAtDistance(void)
{
	this->cameraMode = CCamera::FOLLOW_TARGET_AT_DISTANCE_CHASE_PLANE;
	return;
}

void CCamera::setMode_FollowAtDistance( unsigned int ID_to_Follow )
{
	this->cameraMode = CCamera::FOLLOW_TARGET_AT_DISTANCE_CHASE_PLANE;
	this->object_to_follow_ID = ID_to_Follow;
	return;
}

void CCamera::setMode_LockToTarget(void)
{
	this->cameraMode = CCamera::LOCK_TO_TARGET;
	return;
}

void CCamera::setMode_LockToTarget( unsigned int ID_to_Follow )
{
	this->cameraMode = CCamera::LOCK_TO_TARGET;
	this->object_to_follow_ID = ID_to_Follow;
	return;
}

void CCamera::setFollowMaxSpeed(float newMaxFollowSpeed)
{
	this->maxFollowSpeed = newMaxFollowSpeed;
	return;
}

void CCamera::setFollowMinDistance(float newMinDistance)
{
	this->minFollowDistance = newMinDistance;
	return;
}

void CCamera::setFollowMaxSpeedDistance(float newMaxSpeedDistance)
{
	this->maxSpeedDistance = newMaxSpeedDistance;
	return;
}

void CCamera::setFollowDistances(float newMinDistance, float newMaxSpeedDistance)
{
	this->minFollowDistance = newMinDistance;
	this->maxSpeedDistance = newMaxSpeedDistance;
	return;
}

void CCamera::setFollowTargetToCameraAngle( float xAngle, float yAngle, float zAngle )
{
	this->followTargetToCameraAngle = glm::fquat( glm::vec3( xAngle, yAngle, zAngle ) );
	return;
}

void CCamera::setFollowTargetToCameraAngle( glm::fquat newAngle )
{
	this->followTargetToCameraAngle = newAngle;
	return;
}

// Returns false if it can't find the object
bool CCamera::setFollowTarget( unsigned int ID_to_Follow )
{
	CPhysicalProp targetProps;
	if ( ! this->m_pTheMediator->getPhysicalPropertiesByID( ID_to_Follow, targetProps ) )
	{	// No target with that ID, yo!
		this->cameraMode = CCamera::INDEPENDENT_FREE_LOOK;
		return false;
	}
	this->object_to_follow_ID = ID_to_Follow;
	return true;
}


void CCamera::SetMediator( IMediator* pTheMediator )
{
	this->m_pTheMediator = pTheMediator;
	return;
}

void CCamera::calcTargetFromOrientation(void)
{
	//*********************************************************************
	glm::vec4 relativeTargetPosition = glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f );

	// Calculate adjustment based on orientation (just like in the shader)
	glm::mat4 matOrientation = glm::toMat4( this->orientation );

	glm::vec4 newTargetPos = matOrientation * relativeTargetPosition; 
	//*********************************************************************

	// Copy the glm vec4 into the CVector3f ('cause I used that)
	this->target = CVector3f( newTargetPos.x + this->eye.x, 
		                      newTargetPos.y + this->eye.y, 
							  newTargetPos.z + this->eye.z );
	return;
}

// -ve is Left, +ve is Right
void CCamera::MoveLeftRight( float amount )
{
	glm::vec4 posAdjust = glm::vec4( amount, 0.0f, 0.0f, 1.0f );

	// Calculate adjustment based on orientation (just like in the shader)
	glm::mat4 matOrientation = glm::toMat4( this->orientation );

	posAdjust = matOrientation * posAdjust; 

	this->eye += CVector3f( posAdjust.x, posAdjust.y, posAdjust.z );
	return;
}

// +ve is Forward, -ve is Backward
void CCamera::MoveForwardBackward( float amount )
{
	glm::vec4 posAdjust = glm::vec4( 0.0f, 0.0f, amount, 1.0f );

	// Calculate adjustment based on orientation (just like in the shader)
	glm::mat4 matOrientation = glm::toMat4( this->orientation );

	posAdjust = matOrientation * posAdjust; 

	this->eye += CVector3f( posAdjust.x, posAdjust.y, posAdjust.z );
	return;
}


// +ve is Up, -ve is Down
void CCamera::MoveUpDown( float amount )
{
	glm::vec4 posAdjust = glm::vec4( 0.0f, amount, 0.0f, 1.0f );

	// Calculate adjustment based on orientation (just like in the shader)
	glm::mat4 matOrientation = glm::toMat4( this->orientation );

	posAdjust = matOrientation * posAdjust; 

	this->eye += CVector3f( posAdjust.x, posAdjust.y, posAdjust.z );
	return;
}


void CCamera::Update(float deltaSeconds)
{
	switch ( this->cameraMode )
	{
	case CCamera::INDEPENDENT_FREE_LOOK:
		this->calcTargetFromOrientation();

		break;
	case CCamera::LOCK_TO_TARGET:
		{
			// Get location of the target you are following
			CPhysicalProp targetProps;
			if ( ! this->m_pTheMediator->getPhysicalPropertiesByID( this->object_to_follow_ID, targetProps ) )
			{	// No target with that ID, yo!
				this->cameraMode = CCamera::INDEPENDENT_FREE_LOOK;
			}

			// Make the camera eye the same as the object
			this->eye = targetProps.position;
			// TODO: This isn't finished, yet
		}

		break;
	case CCamera::FOLLOW_TARGET_AT_DISTANCE_CHASE_PLANE:
		{
			CPhysicalProp targetProps;
			if ( ! this->m_pTheMediator->getPhysicalPropertiesByID( this->object_to_follow_ID, targetProps ) )
			{	// No target with that ID, yo!
				this->cameraMode = CCamera::INDEPENDENT_FREE_LOOK;
				return;
			}


			this->target = targetProps.position;

			//targetProps.position = targetPosition;

			//std::cout << "targetProps = " << targetProps.position.x << ", " << targetProps.position.y << "' " << targetProps.position.z << std::endl;

			CVector3f changeThisStep;

			changeThisStep.x = this->m_calcSpeedStepSpeedSingleAxis( targetProps.position.x, this->eye.x, deltaSeconds );
			changeThisStep.y = this->m_calcSpeedStepSpeedSingleAxis( targetProps.position.y, this->eye.y, deltaSeconds );
			changeThisStep.z = this->m_calcSpeedStepSpeedSingleAxis( targetProps.position.z, this->eye.z, deltaSeconds );

			//	// Move camera (may be not moving if changeThisStep = 0.0f)
			if ( this->eye.x < targetProps.position.x )	{ 
				this->eye.x += changeThisStep.x; 
			}
			else if ( this->eye.x > targetProps.position.x )	{ 
				this->eye.x -= changeThisStep.x; 
			}

			if ( this->eye.y < targetProps.position.y )	{ 
				this->eye.y += changeThisStep.y; 
			}
			else if ( this->eye.y > targetProps.position.y )	{ 
				this->eye.y -= changeThisStep.y;
			}

			if ( this->eye.z < targetProps.position.z )	{ 
				this->eye.z += changeThisStep.z; 
			}
			else if ( this->eye.z > targetProps.position.z )	{ 
				this->eye.z -= changeThisStep.z; 
			}
		}

		break;
	}// switch ( this->cameraMode )

	return;
}


void CCamera::AdjustYaw( float changeInDegrees )
{
	glm::fquat yawAdjust = glm::fquat( glm::vec3( 0.0f, changeInDegrees, 0.0f ) );
	this->orientation *= yawAdjust;
	return;
}

// Rotation around x-axis (object relative) - airplane "nose down" or "nose up" 
void CCamera::AdjustPitch( float changeInDegrees )
{
	glm::fquat pitchAdjust = glm::fquat( glm::vec3( changeInDegrees, 0.0f, 0.0f ) );
	this->orientation *= pitchAdjust;
	return;
}


// Rotation around z-axis (object relative) - airplane "barrel roll" from "side to side"
void CCamera::AdjustRoll( float changeInDegrees )
{
	glm::fquat rollAdjust = glm::fquat( glm::vec3( 0.0f, 0.0f, changeInDegrees ) );
	this->orientation *= rollAdjust;
	return;
}


float CCamera::m_calcSpeedStepSpeedSingleAxis( float target, float eye, float deltaTime )
{
	float cameraSpeed = 0.0f;
	
	// Calcualte the gradual speed 
	// 5.0 - 2.0 ===> 3.0f 
	float deltaDistanceRange = this->maxSpeedDistance - this->minFollowDistance;
	float distanceToTarget = abs( eye - target );

	if ( distanceToTarget <= this->minFollowDistance )
	{
		cameraSpeed = 0.0f;		// Not really needed as it's set above
	}
	else if ( distanceToTarget > this->maxSpeedDistance )
	{	
		// Go at full speed
		cameraSpeed = this->maxFollowSpeed * deltaTime;
	}
	else
	{
		float distanceTozeroSpeedBoundary = distanceToTarget - this->minFollowDistance;

		// We want to get a number between 0.0 to 1.0:
		// 0.0 is when the camera is AT the minFollowDistance
		// 1.0 is when the camera is AT the maxSpeedDistance
		float decelRangeRatio = distanceTozeroSpeedBoundary / deltaDistanceRange;

		// Look up the "adjustment" of the speed we want, based on this scaled value
		// This number is how much we will adjust the camera speed.
		float speedAdjustRatio = this->getLERPValue( decelRangeRatio );
		
		// Calculate the maximum speed of camera... 
		cameraSpeed = speedAdjustRatio * this->maxFollowSpeed * deltaTime;
	}

	return cameraSpeed;
}

CCamera::enumCameraStates CCamera::getCameraState(void)
{
	return this->cameraMode;
}

std::string CCamera::getCameraStateString(void)
{
	std::string cameraModeString = "";
	switch ( this->cameraMode )
	{
	case CCamera::INDEPENDENT_FREE_LOOK:
		cameraModeString = "INDEPENDENT_FREE_LOOK";
		break;
	case CCamera::FOLLOW_TARGET_AT_DISTANCE_CHASE_PLANE:
		cameraModeString = "FOLLOW_TARGET_AT_DISTANCE_CHASE_PLANE";
		break;
	case CCamera::LOCK_TO_TARGET:
		cameraModeString = "LOCK_TO_TARGET";
		break;
	}
	return cameraModeString;
}

	//static const float m_defAccCurve[ACC_CURVE_ARRAY_SIZE] = 
	//{
	//	0, 0.00024672, 0.000986636, 0.002219018, 0.003942649, 0.00615583, 0.008856375, 
	//	0.012041619, 0.015708419, 0.019853157, 0.024471742, 0.029559616, 0.035111757, 
	//	0.041122687, 0.047586474, 0.054496738, 0.06184666, 0.069628986, 0.077836037, 
	//	0.086459713, 0.095491503, 0.104922494, 0.114743379, 0.124944465, 0.135515686, 
	//	0.146446609, 0.157726447, 0.169344067, 0.181288005, 0.193546473, 0.206107374, 
	//	0.218958311, 0.232086603, 0.245479292, 0.259123163, 0.27300475, 0.287110354, 
	//	0.301426055, 0.315937724, 0.33063104, 0.345491503, 0.360504447, 0.375655056, 
	//	0.390928379, 0.406309343, 0.421782767, 0.437333383, 0.452945843, 0.46860474, 
	//	0.48429462, 0.5, 0.51570538, 0.53139526, 0.547054157, 0.562666617, 0.578217233, 
	//	0.593690657, 0.609071621, 0.624344944, 0.639495553, 0.654508497, 0.66936896, 
	//	0.684062276, 0.698573945, 0.712889646, 0.72699525, 0.740876837, 0.754520708, 
	//	0.767913397, 0.781041689, 0.793892626, 0.806453527, 0.818711995, 0.830655933, 
	//	0.842273553, 0.853553391, 0.864484314, 0.875055535, 0.885256621, 0.895077506, 
	//	0.904508497, 0.913540287, 0.922163963, 0.930371014, 0.93815334, 0.945503262, 
	//	0.952413526, 0.958877313, 0.964888243, 0.970440384, 0.975528258, 0.980146843, 
	//	0.984291581, 0.987958381, 0.991143625, 0.99384417, 0.996057351, 0.997780982, 
	//	0.999013364, 0.99975328, 1 };
void CCamera::genBasicAccelCurve(void)
{
	// Calcualtes the values of the accel curve... 
	// (the ones Feeney wasted 10 minutes of your life typing in...)
	// NOTE: the index will go from 0 to 100 (from 0.0 to 1.0f)
	for ( int index = 0; index != CCamera::ACC_CURVE_ARRAY_SIZE; index++ )
	{
		// From Excel: =SIN( value *PI() - PI()/2)*0.5 + 0.5
		// Value ranges from 0.0 to 1.0
		float value = static_cast<float>( index / 100.0f );

		static const float PI = 3.1415927f;	// This may be defined somewhere else...

		this->m_accCurve[index] = sin( value * PI - PI/2.0f ) * 0.5f + 0.5f;
		
	}
	return;
}

float CCamera::getLERPValue( float lookup )
{
	// Determine lookup value from table.
	// Note that our table is 0 to ACC_CURVE_ARRAY_SIZE, 
	//	but we are passing in 0.0 to 1.0f. 
	
	// NOTE: We want to make sure that we either:
	// * only pass values from 0.0 to 1.0
	// * adjust the values so that they are in a range of 0.0 to 1.0;
	// TODO: add code that will do this
	
	// Clamp value from 0.0 to 1.0
	if ( lookup < 0.0 )	{ lookup = 0.0f; }
	if ( lookup > 1.0f ) { lookup = 1.0f; }

	float returnValue = 0.0f;
	switch ( this->m_LEFPLookupMode )
	{
	case CCamera::ROUND_DOWN:
		{
			// Pick the "closest" value (i.e. round down)
			int index = static_cast<int>( lookup * 100 );
			returnValue = this->m_accCurve[index];
		}
		break;
	case CCamera::PICK_CLOSEST:
		{
			// Assume it's rounding down, first
			// floor(x + 0.5f) is 'round nearest'
			int index = static_cast<int>( floor( lookup * 100 + 0.5f)  );
			returnValue = this->m_accCurve[index];
		}
		break;
	case CCamera::LERP:
		{
			float lowLookupVal = floor( lookup * 100.0f );
			float highLookupVal = ceil( lookup * 100.0f );

			float delta = highLookupVal - lowLookupVal;		// The difference between the two lookup points
			float LERPLocal = (lookup * 100.0f) - lowLookupVal;	// The amount from the low value

			float LERPamount = LERPLocal / delta;	// The percentage "along" the in between values

			int lowIndex = static_cast<int>( floor(lookup * 100 ) );
			int highIndex = static_cast<int>( ceil(lookup * 100 ) );

			float lowVal = this->m_accCurve[lowIndex];
			float highVal = this->m_accCurve[highIndex];

			float deltaValue = highVal - lowVal;	// Difference between the lookup values

			returnValue = lowVal + ( LERPamount * deltaValue );
		}
		break;
	}

	return returnValue;	
}

