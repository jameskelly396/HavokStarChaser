#ifndef _MatrixMath_HG_
#define _MatrixMath_HG_
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
#include <string>

// If this is defined, the last row of the matrix is ignored
// (because affine transforms have the row 0,0,0,1)
#define FASTER_MATRIX_MULTIPLY_IGNORE_LAST_ROW

// If this is defined, then the multiply is column order
// (instead of row order as in DirectX)
// NOTE: The row order has NOT been tested...
#define COLUMN_MAJOR_ORDER_MULTIPLY_OPEN_GL


// OpenGL uses 1D array theData[16], column major order
// DirectX stores matrix data in row major order


// From http://www.cs.sfu.ca/~haoz/teaching/htmlman/multmatrix.html
// about glMultMatrix_()
//
//  EXAMPLES
//      If the current matrix	is C, and the coordinates to be
//      transformed are, v=(v[0],v[1],v[2],v[3]).  Then the current
//      transformation is C x	v, or
//
//
//      (c[0]  c[4]	 c[8]	c[12] )	    ( v[0] )
//      |                         |     |      |
//      |c[1]  c[5]	 c[9]	c[13] |	    | v[1] |
//      |c[2]  c[6]	 c[10]	c[14] |	 x  | v[2] |
//      |                         |     |      |
//      (c[3]  c[7]	 c[11]	c[15] )	    ( v[3] )
//
//      Calling glMultMatrix with an argument	of
//      m=m[0],m[1],...,m[15] replaces the current transformation
//      with (C x M) x v, or
//
//      ( c[0]  c[4]  c[8]   c[12] )	 ( m[0]	 m[4]  m[8]   m[12] )     ( v[0] )
//      |                          |     |                          |     |      |
//      | c[1]  c[5]  c[9]   c[13] |	 | m[1]	 m[5]  m[9]   m[13] |	  | v[1] |
//      | c[2]  c[6]  c[10]  c[14] |  x  | m[2]	 m[6]  m[10]  m[14] |  x  | v[2] |
//      |                          |     |                          |     |      |
//      ( c[3]  c[7]  c[11]  c[15] )     ( m[3]  m[7]  m[11]  m[15] )     ( v[3] )
//
//      Where 'x' denotes matrix multiplication, and v is represented as a 4 x 1 matrix.
//
class CMat4x4f				
{
public:
	CMat4x4f();
	CMat4x4f( float c0r0, float c0r1, float c0r2, float c0r3, 
              float c1r0, float c1r1, float c1r2, float c1r3,
              float c2r0, float c2r1, float c2r2, float c2r3,
              float c3r0, float c3r1, float c3r2, float c3r3 );
	CMat4x4f( const float OpenGLmat16[] );		// Sets matrix from OpenGL format (same code as SetGLmat16() )
	CMat4x4f( const CMat4x4f &mat );		// Copy constructor
	//
	CMat4x4f& operator=( const CMat4x4f &mat );
	CMat4x4f& operator+( const CMat4x4f &mat );
	CMat4x4f& operator-( const CMat4x4f &mat );
	CMat4x4f operator*( const CMat4x4f &m ) const;
	CMat4x4f& operator+=( const CMat4x4f &mat );
	CMat4x4f& operator-=( const CMat4x4f &mat );
	// 
	bool operator==( const CMat4x4f &mat ) const;
	bool operator!=( const CMat4x4f &mat ) const;
	//
	CMat4x4f& operator*=( float scalar );		// Times a scalar
	CMat4x4f& operator*=( const CMat4x4f &mat );	// 

	void Transpose( void );			
	bool Inverse( void );			// Only works for affine and square matrices...(returns true if worked)
	float Determinant( void );
	void Cofactor( void );


	 //  ___                ___ _      ___ _         __  __ 
	 // / _ \ _ __ ___ _ _ / __| |    / __| |_ _  _ / _|/ _|
	 //| (_) | '_ | -_) ' \ (_ | |__  \__ \  _| || |  _|  _|
	 // \___/| .__|___|_||_\___|____| |___/\__|\_,_|_| |_|  
	 //      |_|                                            
	void OpenGL_SetMat16( float OpenGLMat16[] ); // Sets matrix from OpenGL format for glLoadMatrixf() and glMultMatrixf()
	// You can either load by reference or accept return
	//	Note: Return is simply pointer to passed array (to avoid possible stack fault 
	float* OpenGL_GetMat16( float OpenGLMat16[] );	// Returns in OpenGL format for glLoadMatrixf() and glMultMatrixf()
	//
	// These do the same sort of thing as the OpenGL calls
	void OpenGL_LoadIdentity( void );								// like glLoadIdentity()
	void OpenGL_Rotatef( float angle, float x, float y, float z );	// like glRotatef()
	void OpenGL_Translatef( float x, float y, float z );			// like glTranslatef()
	void OpenGL_Scalef( float x, float y, float z );				// like glScalef()

	std::string DEBUG_GetMatrixString(void);
	//
	float m[4][4];		
private:
	float m_Determinant( float a[][ 4 ], float order );

};





#endif
