
#ifndef _CQuaternion_HG_
#define _CQuaternion_HG_
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
// Code inspired by: http://nehe.gamedev.net/data/lessons/lesson.asp?lesson=Quaternion_Camera_Class
//

class CQuaternion  
{
public:
	CQuaternion();
	virtual ~CQuaternion();
	void CreateOpenGLMatrix(float *pMatrix);
	void CreateFromAxisAngle(float x, float y, float z, float degrees);
	void CreateFromMultipleAngles( float xAngleDegrees, float yAngleDegrees, float zAngleDegrees );

	CQuaternion& operator=(const CQuaternion &q);
	CQuaternion operator*(CQuaternion q);
	CQuaternion& operator*=(CQuaternion q);

private:
	float m_w;
	float m_z;
	float m_y;
	float m_x;
};

#endif
