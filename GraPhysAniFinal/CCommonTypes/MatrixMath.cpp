#include "MatrixMath.h"
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
//

#include <sstream>
#include <math.h>
const double PI		= 3.14159265f;;
const double PI_DIV_180	= 3.14159265f / 180.f;

CMat4x4f::CMat4x4f() 
{
	this->m[0][0] = this->m[0][1] = this->m[0][2] = this->m[0][3] = 0.0f;
	this->m[1][0] = this->m[1][1] = this->m[1][2] = this->m[1][3] = 0.0f;
	this->m[2][0] = this->m[2][1] = this->m[2][2] = this->m[2][3] = 0.0f;
	this->m[3][0] = this->m[3][1] = this->m[3][2] = this->m[3][3] = 0.0f;
}

CMat4x4f::CMat4x4f( float c0r0, float c0r1, float c0r2, float c0r3, 
                    float c1r0, float c1r1, float c1r2, float c1r3,
                    float c2r0, float c2r1, float c2r2, float c2r3,
                    float c3r0, float c3r1, float c3r2, float c3r3 )
{
	this->m[0][0] = c0r0; this->m[0][1] = c0r1;  
	this->m[0][2] = c0r2; this->m[0][3] = c0r3;

	this->m[1][0] = c1r0; this->m[1][1] = c1r1;  
	this->m[1][2] = c1r2; this->m[1][3] = c1r3;

	this->m[2][0] = c2r0; this->m[2][1] = c2r1;  
	this->m[2][2] = c2r2; this->m[2][3] = c2r3;

	this->m[3][0] = c3r0; this->m[3][1] = c3r1;  
	this->m[3][2] = c3r2; this->m[3][3] = c3r3;
}

CMat4x4f::CMat4x4f( const float OpenGLMat16[] )
{
	// m[col][row]
	this->m[0][0] = OpenGLMat16[0];
	this->m[0][1] = OpenGLMat16[1];
	this->m[0][2] = OpenGLMat16[2];
	this->m[0][3] = OpenGLMat16[3];

	this->m[1][0] = OpenGLMat16[4];
	this->m[1][1] = OpenGLMat16[5];
	this->m[1][2] = OpenGLMat16[6];
	this->m[1][3] = OpenGLMat16[7];

	this->m[2][0] = OpenGLMat16[8];
	this->m[2][1] = OpenGLMat16[9];
	this->m[2][2] = OpenGLMat16[10];
	this->m[2][3] = OpenGLMat16[11];

	this->m[3][0] = OpenGLMat16[12];
	this->m[3][1] = OpenGLMat16[13];
	this->m[3][2] = OpenGLMat16[14];
	this->m[3][3] = OpenGLMat16[15];}



CMat4x4f::CMat4x4f( const CMat4x4f &rhs )		// Copy constructor
{
	for ( int col = 0; col != 4; col++ )
	{
		for ( int row = 0; row != 4; row++ )
		{
			this->m[col][row] = rhs.m[col][row];
		}
	}
}

CMat4x4f& CMat4x4f::operator=( const CMat4x4f &rhs )
{
	// Check for self-assignment *IMPORTANT*
	if ( this == &rhs )	return *this;
	// Else...copy values
	for ( int col = 0; col != 4; col++ )
	{
		for ( int row = 0; row != 4; row++ )
		{
			this->m[col][row] = rhs.m[col][row];
		}
	}
	return *this;
}

CMat4x4f& CMat4x4f::operator+( const CMat4x4f &rhs )
{
	return CMat4x4f( *this ) += rhs;
}

CMat4x4f& CMat4x4f::operator-( const CMat4x4f &rhs )
{
	return CMat4x4f( *this ) -= rhs;
}

CMat4x4f& CMat4x4f::operator+=( const CMat4x4f &rhs )
{
	for ( int col = 0; col != 4; col++ )
	{
		for ( int row = 0; row != 4; row++ )
		{
			this->m[col][row] += rhs.m[col][row];
		}
	}
	return *this;
}

CMat4x4f& CMat4x4f::operator-=( const CMat4x4f &rhs )
{
	for ( int col = 0; col != 4; col++ )
	{
		for ( int row = 0; row != 4; row++ )
		{
			this->m[col][row] -= rhs.m[col][row];
		}
	}
	return *this;
}

CMat4x4f& CMat4x4f::operator*=( float scalar )		// Times a scalar
{
	for ( int col = 0; col != 4; col++ )
	{
		for ( int row = 0; row != 4; row++ )
		{
			this->m[col][row] *= scalar;
		}
	}
	return *this;
}

// http://www.bluebit.gr/matrix-calculator/matrix_multiplication.aspx
// http://www.gamedev.net/community/forums/topic.asp?topic_id=379901
// This is the money maker operator, the one that takes the most time...
CMat4x4f& CMat4x4f::operator*=( const CMat4x4f &rhs )	
{
	// Make a copy...
	CMat4x4f temp = (*this);		

//   (c[0]  c[4]	 c[8]	c[12] )	    (c[0]  c[4]	 c[8]	c[12] )
//   |c[1]  c[5]	 c[9]	c[13] |	    |c[1]  c[5]	 c[9]	c[13] |
//   |c[2]  c[6]	 c[10]	c[14] |	 x  |c[2]  c[6]	 c[10]	c[14] |
//   (c[3]  c[7]	 c[11]	c[15] )	    (c[3]  c[7]	 c[11]	c[15] )

#ifdef COLUMN_MAJOR_ORDER_MULTIPLY_OPEN_GL

		this->m[0][0] = temp.m[0][0]*rhs.m[0][0] + temp.m[1][0]*rhs.m[0][1] + temp.m[2][0]*rhs.m[0][2] + temp.m[3][0]*rhs.m[0][3];	  
		this->m[0][1] = temp.m[0][1]*rhs.m[0][0] + temp.m[1][1]*rhs.m[0][1] + temp.m[2][1]*rhs.m[0][2] + temp.m[3][1]*rhs.m[0][3];	  
		this->m[0][2] = temp.m[0][2]*rhs.m[0][0] + temp.m[1][2]*rhs.m[0][1] + temp.m[2][2]*rhs.m[0][2] + temp.m[3][2]*rhs.m[0][3];	  
	#ifndef FASTER_MATRIX_MULTIPLY_IGNORE_LAST_ROW
		this->m[0][3] = temp.m[0][3]*rhs.m[0][0] + temp.m[1][3]*rhs.m[0][1] + temp.m[2][3]*rhs.m[0][2] + temp.m[3][3]*rhs.m[0][3];	  
	#else
		this->m[0][3] = 0.0f;	  
	#endif

		this->m[1][0] = temp.m[0][0]*rhs.m[1][0] + temp.m[1][0]*rhs.m[1][1] + temp.m[2][0]*rhs.m[1][2] + temp.m[3][0]*rhs.m[1][3];   
		this->m[1][1] = temp.m[0][1]*rhs.m[1][0] + temp.m[1][1]*rhs.m[1][1] + temp.m[2][1]*rhs.m[1][2] + temp.m[3][1]*rhs.m[1][3];   
		this->m[1][2] = temp.m[0][2]*rhs.m[1][0] + temp.m[1][2]*rhs.m[1][1] + temp.m[2][2]*rhs.m[1][2] + temp.m[3][2]*rhs.m[1][3];   
	#ifndef FASTER_MATRIX_MULTIPLY_IGNORE_LAST_ROW
		this->m[1][3] = temp.m[0][3]*rhs.m[1][0] + temp.m[1][3]*rhs.m[1][1] + temp.m[2][3]*rhs.m[1][2] + temp.m[3][3]*rhs.m[1][3];   
	#else
		this->m[1][3] = 0.0f;   
	#endif

		this->m[2][0] = temp.m[0][0]*rhs.m[2][0] + temp.m[1][0]*rhs.m[2][1] + temp.m[2][0]*rhs.m[2][2] + temp.m[3][0]*rhs.m[2][3];   
		this->m[2][1] = temp.m[0][1]*rhs.m[2][0] + temp.m[1][1]*rhs.m[2][1] + temp.m[2][1]*rhs.m[2][2] + temp.m[3][1]*rhs.m[2][3];  
		this->m[2][2] = temp.m[0][2]*rhs.m[2][0] + temp.m[1][2]*rhs.m[2][1] + temp.m[2][2]*rhs.m[2][2] + temp.m[3][2]*rhs.m[2][3];   
	#ifndef FASTER_MATRIX_MULTIPLY_IGNORE_LAST_ROW
		this->m[2][3] = temp.m[0][3]*rhs.m[2][0] + temp.m[1][3]*rhs.m[2][1] + temp.m[2][3]*rhs.m[2][2] + temp.m[3][3]*rhs.m[2][3];   
	#else
		this->m[2][3] = 0.0f;   
	#endif

		this->m[3][0] = temp.m[0][0]*rhs.m[3][0] + temp.m[1][0]*rhs.m[3][1] + temp.m[2][0]*rhs.m[3][2] + temp.m[3][0]*rhs.m[3][3];
		this->m[3][1] = temp.m[0][1]*rhs.m[3][0] + temp.m[1][1]*rhs.m[3][1] + temp.m[2][1]*rhs.m[3][2] + temp.m[3][1]*rhs.m[3][3];
		this->m[3][2] = temp.m[0][2]*rhs.m[3][0] + temp.m[1][2]*rhs.m[3][1] + temp.m[2][2]*rhs.m[3][2] + temp.m[3][2]*rhs.m[3][3];
	#ifndef FASTER_MATRIX_MULTIPLY_IGNORE_LAST_ROW
		this->m[3][3] = temp.m[0][3]*rhs.m[3][0] + temp.m[1][3]*rhs.m[3][1] + temp.m[2][3]*rhs.m[3][2] + temp.m[3][3]*rhs.m[3][3];
	#else
		this->m[3][3] = 1.0f;
	#endif

#else
	// This multiplication is really in row major order (vs column major order for OpenGL)
	// This one is ROW major, which is the way DirectX does it...
    this->m[0][0] = temp.m[0][0] * rhs.m[0][0] + temp.m[0][1] * rhs.m[1][0] + temp.m[0][2] * rhs.m[2][0] + temp.m[0][3] * rhs.m[3][0];
    this->m[0][1] = temp.m[0][0] * rhs.m[0][1] + temp.m[0][1] * rhs.m[1][1] + temp.m[0][2] * rhs.m[2][1] + temp.m[0][3] * rhs.m[3][1];
    this->m[0][2] = temp.m[0][0] * rhs.m[0][2] + temp.m[0][1] * rhs.m[1][2] + temp.m[0][2] * rhs.m[2][2] + temp.m[0][3] * rhs.m[3][2];
    this->m[0][3] = temp.m[0][0] * rhs.m[0][3] + temp.m[0][1] * rhs.m[1][3] + temp.m[0][2] * rhs.m[2][3] + temp.m[0][3] * rhs.m[3][3];

	this->m[1][0] = temp.m[1][0] * rhs.m[0][0] + temp.m[1][1] * rhs.m[1][0] + temp.m[1][2] * rhs.m[2][0] + temp.m[1][3] * rhs.m[3][0];
    this->m[1][1] = temp.m[1][0] * rhs.m[0][1] + temp.m[1][1] * rhs.m[1][1] + temp.m[1][2] * rhs.m[2][1] + temp.m[1][3] * rhs.m[3][1];
    this->m[1][2] = temp.m[1][0] * rhs.m[0][2] + temp.m[1][1] * rhs.m[1][2] + temp.m[1][2] * rhs.m[2][2] + temp.m[1][3] * rhs.m[3][2];
    this->m[1][3] = temp.m[1][0] * rhs.m[0][3] + temp.m[1][1] * rhs.m[1][3] + temp.m[1][2] * rhs.m[2][3] + temp.m[1][3] * rhs.m[3][3];

	this->m[2][0] = temp.m[2][0] * rhs.m[0][0] + temp.m[2][1] * rhs.m[1][0] + temp.m[2][2] * rhs.m[2][0] + temp.m[2][3] * rhs.m[3][0];
    this->m[2][1] = temp.m[2][0] * rhs.m[0][1] + temp.m[2][1] * rhs.m[1][1] + temp.m[2][2] * rhs.m[2][1] + temp.m[2][3] * rhs.m[3][1];
    this->m[2][2] = temp.m[2][0] * rhs.m[0][2] + temp.m[2][1] * rhs.m[1][2] + temp.m[2][2] * rhs.m[2][2] + temp.m[2][3] * rhs.m[3][2];
    this->m[2][3] = temp.m[2][0] * rhs.m[0][3] + temp.m[2][1] * rhs.m[1][3] + temp.m[2][2] * rhs.m[2][3] + temp.m[2][3] * rhs.m[3][3];

	this->m[3][0] = temp.m[3][0] * rhs.m[0][0] + temp.m[3][1] * rhs.m[1][0] + temp.m[3][2] * rhs.m[2][0] + temp.m[3][3] * rhs.m[3][0];
    this->m[3][1] = temp.m[3][0] * rhs.m[0][1] + temp.m[3][1] * rhs.m[1][1] + temp.m[3][2] * rhs.m[2][1] + temp.m[3][3] * rhs.m[3][1];
    this->m[3][2] = temp.m[3][0] * rhs.m[0][2] + temp.m[3][1] * rhs.m[1][2] + temp.m[3][2] * rhs.m[2][2] + temp.m[3][3] * rhs.m[3][2];
    this->m[3][3] = temp.m[3][0] * rhs.m[0][3] + temp.m[3][1] * rhs.m[1][3] + temp.m[3][2] * rhs.m[2][3] + temp.m[3][3] * rhs.m[3][3];
#endif

	return *this;
}

CMat4x4f CMat4x4f::operator*( const CMat4x4f &mat ) const
{
	return CMat4x4f(*this) *= mat;
}


void CMat4x4f::OpenGL_SetMat16( float OpenGLMat16[] ) // Sets matrix from OpenGL format for glLoadMatrixf() and glMultMatrixf()
{
	// m[col][row]
	this->m[0][0] = OpenGLMat16[0];
	this->m[0][1] = OpenGLMat16[1];
	this->m[0][2] = OpenGLMat16[2];
	this->m[0][3] = OpenGLMat16[3];

	this->m[1][0] = OpenGLMat16[4];
	this->m[1][1] = OpenGLMat16[5];
	this->m[1][2] = OpenGLMat16[6];
	this->m[1][3] = OpenGLMat16[7];

	this->m[2][0] = OpenGLMat16[8];
	this->m[2][1] = OpenGLMat16[9];
	this->m[2][2] = OpenGLMat16[10];
	this->m[2][3] = OpenGLMat16[11];

	this->m[3][0] = OpenGLMat16[12];
	this->m[3][1] = OpenGLMat16[13];
	this->m[3][2] = OpenGLMat16[14];
	this->m[3][3] = OpenGLMat16[15];
}

float* CMat4x4f::OpenGL_GetMat16( float OpenGLMat16[] )	// Returns in OpenGL format for glLoadMatrixf() and glMultMatrixf()
{
	// m[col][row]
	OpenGLMat16[0] = this->m[0][0];
	OpenGLMat16[1] = this->m[0][1];
	OpenGLMat16[2] = this->m[0][2];
	OpenGLMat16[3] = this->m[0][3];

	OpenGLMat16[4] = this->m[1][0];
	OpenGLMat16[5] = this->m[1][1];
	OpenGLMat16[6] = this->m[1][2];
	OpenGLMat16[7] = this->m[1][3];

	OpenGLMat16[8] = this->m[2][0];
	OpenGLMat16[9] = this->m[2][1];
	OpenGLMat16[10] = this->m[2][2];
	OpenGLMat16[11] = this->m[2][3];

	OpenGLMat16[12] = this->m[3][0];
	OpenGLMat16[13] = this->m[3][1];
	OpenGLMat16[14] = this->m[3][2];
	OpenGLMat16[15] = this->m[3][3];
	//
	return OpenGLMat16;	// Return pointer to this array
}

// Some links:
// http://www.cs.sfu.ca/~haoz/teaching/htmlman/loadidentity.html
void CMat4x4f::OpenGL_LoadIdentity( void )			// like glLoadIdentity()
{
	// ( 1   0	 0   0	)
	// |                |
	// | 0   1	 0   0	|
	// |                |
	// | 0   0	 1   0	|
	// |                |
	// ( 0   0	 0   1	)

	this->m[0][0] = 1.0f;
	this->m[0][1] = 0.0f;
	this->m[0][2] = 0.0f;
	this->m[0][3] = 0.0f;

	this->m[1][0] = 0.0f;
	this->m[1][1] = 1.0f;
	this->m[1][2] = 0.0f;
	this->m[1][3] = 0.0f;

	this->m[2][0] = 0.0f;
	this->m[2][1] = 0.0f;
	this->m[2][2] = 1.0f;
	this->m[2][3] = 0.0f;

	this->m[3][0] = 0.0f;
	this->m[3][1] = 0.0f;
	this->m[3][2] = 0.0f;
	this->m[3][3] = 1.0f;

	return;
}

// Some links:
// http://www.opengl.org/sdk/docs/man/xhtml/glRotate.xml
// http://www.cs.sfu.ca/~haoz/teaching/htmlman/rotate.html
void CMat4x4f::OpenGL_Rotatef( float angle, float x, float y, float z )	// like glRotatef()
{

	// ( xx(1-c)+c    0   0  0  )
	// |                                          |
	// | yx(1-c)+zs   0   0  0  |
	// |                                          |
	// | xz(1-c)-ys   0   0  0  |
	// |                                          |
	// (	 0        0   0  1  )

	float c = static_cast<float>( cos( static_cast<double>( angle ) * PI_DIV_180 ) );
	float s = static_cast<float>( sin( static_cast<double>( angle ) * PI_DIV_180 ) );

	CMat4x4f rotMatrix;
	rotMatrix.m[0][0] = x*x*(1-c)+c;		// xx(1-c)+c
	rotMatrix.m[0][1] = y*x*(1-c)+z*s;		// yx(1-c)+zs
	rotMatrix.m[0][2] = x*z*(1-c)-y*s;		// xz(1-c)-ys
	rotMatrix.m[0][3] = 0.0f;				// 0

	rotMatrix.m[1][0] = x*y*(1-c)-z*s;		// xy(1-c)-zs
	rotMatrix.m[1][1] = y*y*(1-c)+c;		// yy(1-c)+c
	rotMatrix.m[1][2] = y*z*(1-c)+x*s;		// yz(1-c)+xs
	rotMatrix.m[1][3] = 0.0f;				// 0

	rotMatrix.m[2][0] = x*z*(1-c)+y*s;		// xz(1-c)+ys
	rotMatrix.m[2][1] = y*z*(1-c)-x*s;		// yz(1-c)-xs
	rotMatrix.m[2][2] = z*z*(1-c)+c;		// zz(1-c)+c
	rotMatrix.m[2][3] = 0.0f;

	rotMatrix.m[3][0] = 0.0f;
	rotMatrix.m[3][1] = 0.0f;
	rotMatrix.m[3][2] = 0.0f;
	rotMatrix.m[3][3] = 1.0f;

	// Now apply it to the matrix...
	(*this) *= rotMatrix;
	//(*this) = rotMatrix * (*this);
}

// http://www.cs.sfu.ca/~haoz/teaching/htmlman/translate.html
void CMat4x4f::OpenGL_Translatef( float x, float y, float z )			// like glTranslatef()
{
	 // ( 1  0  0  x )
	 // |            |
	 // | 0  1  0  y |
	 // |            |
	 // | 0  0  1  z |
	 // |            |
	 // ( 0  0  0  1 )
	CMat4x4f transMatrix;
	transMatrix.m[0][0] = 1.0f;
	transMatrix.m[0][1] = 0.0f;
	transMatrix.m[0][2] = 0.0f;
	transMatrix.m[0][3] = 0.0f;

	transMatrix.m[1][0] = 0.0f;
	transMatrix.m[1][1] = 1.0f;
	transMatrix.m[1][2] = 0.0f;
	transMatrix.m[1][3] = 0.0f;

	transMatrix.m[2][0] = 0.0f;
	transMatrix.m[2][1] = 0.0f;
	transMatrix.m[2][2] = 1.0f;
	transMatrix.m[2][3] = 0.0f;

	transMatrix.m[3][0] = x;
	transMatrix.m[3][1] = y;
	transMatrix.m[3][2] = z;
	transMatrix.m[3][3] = 1.0f;

	// Apply it to the matrix...
	(*this) *= transMatrix;
	//(*this) = transMatrix * (*this);
}

// http://www.cs.sfu.ca/~haoz/teaching/htmlman/scale.html
void CMat4x4f::OpenGL_Scalef( float x, float y, float z )				// like glScalef()
{
	// ( x   0   0   0 )
	// |               |
	// | 0   y   0   0 |
	// |               |
	// | 0   0   z   0 |
	// |               |
	// ( 0   0   0   1 )
	
	CMat4x4f scaleMatrix;
	scaleMatrix.m[0][0] = x;
	scaleMatrix.m[0][1] = 0.0f;
	scaleMatrix.m[0][2] = 0.0f;
	scaleMatrix.m[0][3] = 0.0f;

	scaleMatrix.m[1][0] = 0.0f;
	scaleMatrix.m[1][1] = y;
	scaleMatrix.m[1][2] = 0.0f;
	scaleMatrix.m[1][3] = 0.0f;

	scaleMatrix.m[2][0] = 0.0f;
	scaleMatrix.m[2][1] = 0.0f;
	scaleMatrix.m[2][2] = z;
	scaleMatrix.m[2][3] = 0.0f;

	scaleMatrix.m[3][0] = 0.0f;
	scaleMatrix.m[3][1] = 0.0f;
	scaleMatrix.m[3][2] = 0.0f;
	scaleMatrix.m[3][3] = 1.0f;

	// Apply it to the matrix...
	(*this) *= scaleMatrix;
}

bool CMat4x4f::operator==( const CMat4x4f &mat ) const
{
	for ( int col = 0; col != 4; col++ )
	{
		for ( int row = 0; row != 4; row++ )
		{
			if ( this->m[col][row] != mat.m[col][row] )
			{
				return false;
			}
		}
	}
	return true;
}

bool CMat4x4f::operator!=( const CMat4x4f &mat ) const
{
	return ( !( (*this) == mat) );
}

#include <iomanip>

std::string CMat4x4f::DEBUG_GetMatrixString(void)
{
	std::stringstream ss;
	//ss << std::setw(15) << std::setprecision(2) << std::right << std::noskipws;
	for ( int row = 0; row != 4; row++ )
	{
		for ( int col = 0; col != 4; col++ )
		{
			ss << "[" << col << "][" << row << "]=" << this->m[col][row] << " ";
		}
		ss << std::endl;
	}
	return ss.str();
}


// This is called recursively to find the determinant...
// Taken mainly from here: http://thecodecracker.com/c-programming/inverse-of-a-matrix-in-c/
// (Wonderful, straight-forward coding. Love it.)
float CMat4x4f::m_Determinant( float a[][ 4 ], float order )
{
    float s = 1, det = 0, b[ 4 ][ 4 ];
    int i, j;
 
    if ( order == 1 )
    {
        return ( a[ 0 ][ 0 ] );
    }
    else
    {
        det = 0;

        for ( int c = 0; c != order; c++ )
        {
            int m = 0;
            int n = 0;

            for ( i = 0; i != order; i++ )
            {
                for ( j = 0; j != order; j++ )
                {
                    b[ i ][ j ] = 0;

                    if ( (i != 0) && (j != c) )
                    {
                        b[ m ][ n ] = a[ i ][ j ];

                        if ( n < ( order - 2.0 ) )
						{
                            n++;
						}
                        else
                        {
                            n = 0;
                            m++;
                        }
                    }
                }
            }

            det = det + s * ( a[ 0 ][ c ] * this->m_Determinant( b, order - 1 ) );
            s = -1 * s;
        }
    }
 
    return ( det );
}




void CMat4x4f::Transpose( void )
{
	const unsigned int ORDER = 4;
	// Copy it...
	float matCopy[ORDER][ORDER] = {0.0f};
	for ( int col = 0; col != ORDER; col++ )
	{
		for ( int row = 0; row != ORDER; row++ )
		{
			matCopy[col][row] = this->m[col][row];
		}
	}
	// Transpose...
    for ( int col = 0; col != ORDER; col++ )
    {
        for ( int row = 0; row != ORDER; row++ )
        {
            this->m[ col ][ row ] = matCopy[ row ][ col ];
        }
    }
}


// Only works for affine and square matrices...(returns true if worked)
//bool CMat4x4f::m_Inverse( float num[][4], float fac[][4], float order )
// Taken mainly from here: http://thecodecracker.com/c-programming/inverse-of-a-matrix-in-c/
// (Wonderful, straight-forward coding. Love it.)
bool CMat4x4f::Inverse( void )			
{
	const int MATRIX_ORDER = 4;

	//float matcopy[ MATRIX_ORDER ][ MATRIX_ORDER ] = {0.0f};

	//for ( int col = 0; col != MATRIX_ORDER; col++ )
	//{
	//	for ( int row = 0; row != MATRIX_ORDER; row++ )
	//	{
	//		matcopy[col][row] = this->m[col][row];
	//	}
	//}

    float d = this->m_Determinant( this->m, static_cast<float>( MATRIX_ORDER ) );
    if ( d == 0 )
	{	// MATRIX IS NOT INVERSIBLE
		return false;
	}	

	// Find the inverse...
	//int i, j;
 
	// Transpose...
	CMat4x4f cofactorTransposeMatrix( *this );
	cofactorTransposeMatrix.Cofactor();
	cofactorTransposeMatrix.Transpose();

    //for ( i = 0; i < MATRIX_ORDER; i++ )
    //{
    //    for ( j = 0;j < MATRIX_ORDER;j++ )
    //    {
    //        trans[ i ][ j ] = cofactorMatrix.m[ j ][ i ];
    //    }
    //}

	//float inverse[MATRIX_ORDER][MATRIX_ORDER] = {0.0f};
 //
    for ( int col = 0; col < MATRIX_ORDER; col++ )
    {
        for ( int row = 0; row < MATRIX_ORDER; row++ )
        {
            this->m[ col ][ row ] = cofactorTransposeMatrix.m[ col ][ row ] / d;
        }
    }

	return true;
}

float CMat4x4f::Determinant( void )
{
	float det = this->m_Determinant( this->m, 4 );
	return det;
}

// THIS HASN'T BEEN TESTED AS MUCH AS I WOULD LIKE....
// ...COULD WORK. THEN AGAIN, PERHAPS NOT...
// Taken mainly from here: http://thecodecracker.com/c-programming/inverse-of-a-matrix-in-c/
// (Wonderful, straight-forward coding. Love it.)
void CMat4x4f::Cofactor( void )
{
	const int MATRIX_ORDER = 4;
    float b[ MATRIX_ORDER ][ MATRIX_ORDER ], fac[ MATRIX_ORDER ][ MATRIX_ORDER ];
    int p, q, m, n, i, j;
 
    for ( q = 0; q != MATRIX_ORDER; q++ )
    {
        for ( p = 0; p != MATRIX_ORDER; p++ )
        {
            m = 0;
            n = 0;

            for ( i = 0; i != MATRIX_ORDER; i++ )
            {
                for ( j = 0; j != MATRIX_ORDER; j++ )
                {
                    b[ i ][ j ] = 0;

                    if ( i != q && j != p )
                    {
                        /*b[ m ][ n ] = num[ i ][ j ];*/
						b[ m ][ n ] = this->m[ i ][ j ];

                        if ( n < ( MATRIX_ORDER - 2 ) )
						{
                            n++;
						}
                        else
                        {
                            n = 0;
                            m++;
                        }
                    }
                }
            }

            fac[ q ][ p ] = pow( -1.0f, q + p ) * this->m_Determinant( b, MATRIX_ORDER - 1 );
        }
    }//for ( q = 0; q < order; q++ )
	// Update the internal matrix
	for ( int col = 0; col != 4; col++ )
	{
		for ( int row = 0; row != 4; row++ )
		{
			this->m[col][row] = fac[col][row];
		}
	}
}





















// Taken mainly from here: http://thecodecracker.com/c-programming/inverse-of-a-matrix-in-c/
// (Wonderful, straight-forward coding. Love it.)
//void CMat4x4f::m_Cofactor( float num[][ 4 ], float order )
//{
//    float b[ 4 ][ 4 ], fac[ 4 ][ 4 ];
//    int p, q, m, n, i, j;
// 
//    for ( q = 0; q < order; q++ )
//    {
//        for ( p = 0; p < order; p++ )
//        {
//            m = 0;
//            n = 0;
//
//            for ( i = 0; i < order; i++ )
//            {
//                for ( j = 0; j < order; j++ )
//                {
//                    b[ i ][ j ] = 0;
//
//                    if ( i != q && j != p )
//                    {
//                        b[ m ][ n ] = num[ i ][ j ];
//
//                        if ( n < ( order - 2 ) )
//						{
//                            n++;
//						}
//                        else
//                        {
//                            n = 0;
//                            m++;
//                        }
//                    }
//                }
//            }
//
//            fac[ q ][ p ] = pow( -1.0f, q + p ) * this->m_Determinant( b, order - 1 );
//        }
//    }
// 
//    //this->m_Transpose( num, fac, order );
//}

// Taken mainly from here: http://thecodecracker.com/c-programming/inverse-of-a-matrix-in-c/
// (Wonderful, straight-forward coding. Love it.)
//void CMat4x4f::m_Transpose( float num[][ 4 ], float fac[][ 4 ], float order )
//{
//    int i, j;
//    float trans[ 4 ][ 4 ];	/* , dinverse[ 4 ][ 4 ],*/ 
// 
//	// Transpose...
//    for ( i = 0; i < order; i++ )
//    {
//        for ( j = 0;j < order;j++ )
//        {
//            trans[ i ][ j ] = fac[ j ][ i ];
//        }
//    }
// 
//    //d = Determinant( num, order );
//    //inverse[ i ][ j ] = 0;
// 
//    //for ( i = 0; i < order; i++ )
//    //{
//    //    for ( j = 0; j < order; j++ )
//    //    {
//    //        inverse[ i ][ j ] = trans[ i ][ j ] / d;
//    //    }
//    //}
// //
// //   printf( "\nTHE INVERSE OF THE MATRIX:\n" );
//	//// Inverse...
// //   for ( i = 0; i < order; i++ )
// //   {
// //       for ( j = 0; j < order; j++ )
// //       {
//	//		std::cout << "\t" << inverse[ i ][ j ];
// //       }
//
//	//	std::cout << std::endl;
// //   }
//}

// Taken mainly from here: http://thecodecracker.com/c-programming/inverse-of-a-matrix-in-c/
// (Wonderful, straight-forward coding. Love it.)
//bool CMat4x4f::m_Inverse( float num[][4], float CofactorMatrix[][4], float order )
//{
//    float d = this->m_Determinant( num, order );
//    if ( d == 0 )
//	{	// MATRIX IS NOT INVERSIBLE
//		return false;
//	}	
//	// Find the inverse...
//
//	int i, j;
//    float trans[ 4 ][ 4 ], inverse[ 4 ][ 4 ];	/*, d*/
// 
//	// Transpose...
//    for ( i = 0; i < order; i++ )
//    {
//        for ( j = 0;j < order;j++ )
//        {
//            trans[ i ][ j ] = CofactorMatrix[ j ][ i ];
//        }
//    }
//
//    d = this->m_Determinant( num, order );
//    inverse[ i ][ j ] = 0;
// 
//    for ( i = 0; i < order; i++ )
//    {
//        for ( j = 0; j < order; j++ )
//        {
//            inverse[ i ][ j ] = trans[ i ][ j ] / d;
//        }
//    }
//
//	return true;
//}
