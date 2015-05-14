#ifndef _CTriangle_HG_
#define _CTriangle_HG_

#include "CCommonTypes\CVector3f.h"

class CTriangle
{
public:
	CTriangle();
	int V1;		// float x,y,z
	int V2;		
	int V3;		
	CVector3f P1;
	CVector3f P2;
	CVector3f P3;
	CVector3f normal;
};
#endif;
