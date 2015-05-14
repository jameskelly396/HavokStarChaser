#ifndef _CVector4f_HG_
#define _CVector4f_HG_

class CVector4f
{
public:
	CVector4f() : x(0.0f), y(0.0f), z(0.0f), a(1.0f) {};
	CVector4f(float x, float y, float z, float a) : x(x), y(y), z(z), a(a) {};
	~CVector4f() {};
	float x,y,z,a;
};

#endif
