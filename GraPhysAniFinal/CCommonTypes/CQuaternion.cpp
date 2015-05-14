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
#include "CQuaternion.h"
#include <math.h>

static const float PI = 3.14159265358979323846f;

CQuaternion::CQuaternion()
{
	this->m_x = this->m_y = this->m_z = 0.0f;
	this->m_w = 1.0f;
}

CQuaternion::~CQuaternion()
{

}

void CQuaternion::CreateFromAxisAngle(float x, float y, float z, float degrees)
{
	// First we want to convert the degrees to radians 
	// since the angle is assumed to be in radians
	float angle = float((degrees / 180.0f) * PI);

	// Here we calculate the sin( theta / 2) once for optimization
	float result = (float)sin( angle / 2.0f );
		
	// Calcualte the w value by cos( theta / 2 )
	m_w = (float)cos( angle / 2.0f );

	// Calculate the x, y and z of the quaternion
	m_x = float(x * result);
	m_y = float(y * result);
	m_z = float(z * result);
}

void CQuaternion::CreateFromMultipleAngles( float xAngleDegrees, float yAngleDegrees, float zAngleDegrees )
{
	this->CreateFromAxisAngle( 1.0f, 0.0f, 0.0f, xAngleDegrees );
	CQuaternion rot;	rot.CreateFromAxisAngle( 0.0f, 1.0f, 0.0f, yAngleDegrees );
	(*this) *= rot;
	rot;	rot.CreateFromAxisAngle( 0.0f, 0.0f, 1.0f, zAngleDegrees );
	(*this) *= rot;
}


void CQuaternion::CreateOpenGLMatrix(float *pMatrix)
{
	// Make sure the matrix has allocated memory to store the rotation data
	if(!pMatrix) return;

	// First row
	pMatrix[ 0] = 1.0f - 2.0f * ( m_y * m_y + m_z * m_z ); 
	pMatrix[ 1] = 2.0f * (m_x * m_y + m_z * m_w);
	pMatrix[ 2] = 2.0f * (m_x * m_z - m_y * m_w);
	pMatrix[ 3] = 0.0f;  

	// Second row
	pMatrix[ 4] = 2.0f * ( m_x * m_y - m_z * m_w );  
	pMatrix[ 5] = 1.0f - 2.0f * ( m_x * m_x + m_z * m_z ); 
	pMatrix[ 6] = 2.0f * (m_z * m_y + m_x * m_w );  
	pMatrix[ 7] = 0.0f;  

	// Third row
	pMatrix[ 8] = 2.0f * ( m_x * m_z + m_y * m_w );
	pMatrix[ 9] = 2.0f * ( m_y * m_z - m_x * m_w );
	pMatrix[10] = 1.0f - 2.0f * ( m_x * m_x + m_y * m_y );  
	pMatrix[11] = 0.0f;  

	// Fourth row
	pMatrix[12] = 0;  
	pMatrix[13] = 0;  
	pMatrix[14] = 0;  
	pMatrix[15] = 1.0f;

	// Now pMatrix[] is a 4x4 homogeneous matrix that can be applied to an OpenGL Matrix
}

CQuaternion CQuaternion::operator*(CQuaternion q)
{
	CQuaternion r;

	r.m_w = (this->m_w * q.m_w) - (this->m_x * q.m_x) - (this->m_y * q.m_y) - (this->m_z * q.m_z);
	r.m_x = (this->m_w * q.m_x) + (this->m_x * q.m_w) + (this->m_y * q.m_z) - (this->m_z * q.m_y);
	r.m_y = (this->m_w * q.m_y) + (this->m_y * q.m_w) + (this->m_z * q.m_x) - (this->m_x * q.m_z);
	r.m_z = (this->m_w * q.m_z) + (this->m_z * q.m_w) + (this->m_x * q.m_y) - (this->m_y * q.m_x);
	
	return(r);
}

	
//CVector3f& CVector3f::operator*=(float scalar)
CQuaternion& CQuaternion::operator*=(CQuaternion q)
{
	CQuaternion r = *this;

	*this = r * q;

	return *this;
}


CQuaternion& CQuaternion::operator=(const CQuaternion &q)
{
	// Check for self-assignment *IMPORTANT*
	if ( this == &q )	return *this;
	// Else...copy values
	this->m_x = q.m_x; this->m_y = q.m_y; this->m_z = q.m_z;
	this->m_w = q.m_w;
	return *this;
}
