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
#ifndef _CCamera_HG_
#define _CCamera_HG_

#include "CVector3f.h"
#include "CQuaternion.h"

class CCamera
{
public:
	CCamera();
	CCamera( const CVector3f &xyz );				
	CCamera( const CVector3f &xyz, const CVector3f &yawPitchRoll );	// Pass three angles...
	CCamera( const CVector3f &xyz, const CQuaternion &yawPitchRoll );
	virtual ~CCamera();

	void Move( float distance );	// Updates camera location based on view

	void Pitch( float angle );		
	void Yaw( float angle );		
	void Roll( float angle );		
	void YawPitchRoll( const CQuaternion &angle );

	void LoadOpenGLRotationMatrix(float *pMatrix);

	// Getters and Setters
	CVector3f getXYZ(void);
	void setXYZ( const CVector3f &newXYZ );
	void setXYZ( float x, float y, float z );
	CVector3f getYawPitchRoll(void);
	void setYawPitchRoll( const CVector3f &newYawPitchRoll );
	CQuaternion getYawPitchRollQuaternion( void );
	void setYawPitchRoll( const CQuaternion &newYawPitchRoll );
	
	float getX( void );			float getY( void );			float getZ( void );
	void setX( float newX );	void setY( float newY );	void setZ( float newZ );
	float getYaw( void );			float getPitch( void );				float getRoll( void );
	void setYaw( float newYaw );	void setPitch( float newPitch);		void setRoll( float newRoll );

	void setSpeed( float newSpeed );	float getSpeed( void );
private:
	CVector3f m_xyz;
	CQuaternion m_yaw_pitch_roll;
	float m_speed;
};

#endif
