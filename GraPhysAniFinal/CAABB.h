#ifndef _CAABB_HG_
#define _CAABB_HG_

#include <vector>
#include <map>
#include "CTriangle.h"


class CAABB
{
public:
	CAABB() {};
	CVector3f minXYZ;		// 3.0, 5.0, 2.0 
	float cubeSize;		// Assume these are square

	int genIndex()
	{
		this->m_index = (int)minXYZ.z +					// 2
			            (int)minXYZ.y * 100 +			// 500
						(int)minXYZ.x * 100 * 100 +	// 30000
						1000000000;				//used incase any of the multiplications work out to be 0
		// 30000 + 500 + 2
		// 1003005002;
		return this->m_index;
	}

	bool bHasTriangleInIt;
	std::map< unsigned int, CAABB* > map_pchildAABB;		// "Branch" nodes
	std::vector< CTriangle* > vec_pTriangles;	// "Leaf" nodes

	int m_index;
};

#endif