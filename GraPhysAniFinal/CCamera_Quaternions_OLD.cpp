/* Written by Michael Feeney, Fanshawe College, (c) 2008 to 2011            */
/* mfeeney@fanshawec.ca                                                     */
/*                                                                          */
/* For the 'Game Development - Advanced Programming' curriculum             */
/* www.fanshawec.ca/en/GDP1                                                 */
/*                                                                          */
/* It may be distributed under the terms of the General Public License:     */
/* http://www.fsf.org/licenses/gpl.html                                     */
/*                                                                          */
/* Use this code at your own risk. It is indented only as a learning aid.   */
/*                                                                          */
#include "CCamera.h"
#include "MatrixMath.h"

CCamera::CCamera()
{
	this->m_speed = 0.1f;
}

CCamera::CCamera( const CVector3f &xyz )
{
	this->m_xyz = xyz;
}

//CCamera( const CVector3f &xyz, const CVector3f &yawPitchRoll );	// Pass three angles...
//CCamera( const CVector3f &xyz, const CQuaternion &yawPitchRoll );

CCamera::~CCamera()
{
}

// Updates camera location based on view
void CCamera::Move( float distance )	
{
	float yawPitchRollMatArray[16] = {0.0f};
	// Get OpenGL Model View matrix representation of quaternion 
	m_yaw_pitch_roll.CreateOpenGLMatrix( yawPitchRollMatArray );

	CVector3f direction( -yawPitchRollMatArray[8], 
		                 -yawPitchRollMatArray[9], 
					     yawPitchRollMatArray[10] );

	direction *= distance;

	this->m_xyz += direction;
}


void CCamera::YawPitchRoll( const CQuaternion &angle )
{
	this->m_yaw_pitch_roll *= angle;
}

void CCamera::Pitch( float angle )
{
	CQuaternion rot;
	rot.CreateFromAxisAngle( 1.0f, 0.0f, 0.0f, angle );
	this->m_yaw_pitch_roll *= rot;
}


void CCamera::Yaw( float angle )
{
	CQuaternion rot;
	rot.CreateFromAxisAngle( 0.0f, 1.0f, 0.0f, angle );
	this->m_yaw_pitch_roll *= rot;
}

void CCamera::Roll( float angle )
{
	CQuaternion rot;
	rot.CreateFromAxisAngle( 0.0f, 0.0f, 1.0f, angle );
	this->m_yaw_pitch_roll *= rot;
}
//
void CCamera::LoadOpenGLRotationMatrix(float *pMatrix)
{
	// Get the rotational matrix 
	this->m_yaw_pitch_roll.CreateOpenGLMatrix( pMatrix );

	CMat4x4f matRotate;
	matRotate.OpenGL_SetMat16( pMatrix );


	// Now determine the translation matrix for this camera...
	CMat4x4f matTrans;		
	matTrans.OpenGL_LoadIdentity();
	matTrans.OpenGL_Translatef( this->m_xyz.x, this->m_xyz.y, this->m_xyz.z );

	//// Combine the two...
	CMat4x4f result;		
	result = matRotate * matTrans;

	result.OpenGL_GetMat16( pMatrix );
}

//
//// Getters and Setters
//CVector3f getXYZ(void);
//void setXYZ( const CVector3f &newXYZ );
void CCamera::setXYZ( float x, float y, float z )
{
	this->m_xyz.x = x;	this->m_xyz.y = y;	this->m_xyz.z = z;
}

//CVector3f getYawPitchRoll(void);
//void setYawPitchRoll( const CVector3f &newYawPitchRoll );
//CQuaternion getYawPitchRollQuaternion( void );
//void setYawPitchRoll( const CQuaternion &newYawPitchRoll );
//
float CCamera::getX( void )
{
	return this->m_xyz.x;
}

float CCamera::getY( void )
{
	return this->m_xyz.y;
}

float CCamera::getZ( void )
{
	return this->m_xyz.z;
}

void CCamera::setX( float newX )
{
	this->m_xyz.x = newX;
}

void CCamera::setY( float newY )
{
	this->m_xyz.y = newY;
}

void CCamera::setZ( float newZ )
{
	this->m_xyz.z = newZ;
}

void CCamera::setSpeed( float newSpeed )
{
	this->m_speed = newSpeed;
}

float CCamera::getSpeed( void )
{
	return this->m_speed;
}


//float getYaw( void );			float getPitch( void );				float getRoll( void );
//void setYaw( float newYaw );	void setPitch( float newPitch);		void setRoll( float newRoll );
