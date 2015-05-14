/********************************************************/
/* AABB-triangle overlap test code                      */
/* by Tomas Akenine-Möller                              */
/* Function: int triBoxOverlap(float boxcenter[3],      */
/*          float boxhalfsize[3],float triverts[3][3]); */
/* History:                                             */
/*   2001-03-05: released the code in its first version */
/*   2001-06-18: changed the order of the tests, faster */
/*                                                      */
/* Acknowledgement: Many thanks to Pierre Terdiman for  */
/* suggestions and discussions on how to optimize code. */
/* Thanks to David Hunt for finding a ">="-bug!         */
/********************************************************/

#include <math.h>
#include <stdio.h>
#include "global.h"
#include <iostream>		// Added
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#define X 0
#define Y 1
#define Z 2

#define CROSS(dest, v1, v2) \
	dest[0] = v1[1] * v2[2] - v1[2] * v2[1]; \
	dest[1] = v1[2] * v2[0] - v1[0] * v2[2]; \
	dest[2] = v1[0] * v2[1] - v1[1] * v2[0];

#define DOT(v1, v2) (v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2])

#define SUB(dest, v1, v2) \
	dest[0] = v1[0] - v2[0]; \
	dest[1] = v1[1] - v2[1]; \
	dest[2] = v1[2] - v2[2];

#define FINDMINMAX(x0, x1, x2, min, max) \
	min = max = x0; \
if (x1 < min) min = x1; \
if (x1 > max) max = x1; \
if (x2 < min) min = x2; \
if (x2 > max) max = x2;


int genIndex(int x, int y, int z)
{
	return z +					// 2
		y * 100 +			// 500
		x * 100 * 100 +	// 30000
		1000000000;				//used incase any of the multiplications work out to be 0
}


bool TestAABBAABB(CAABB* a, CAABB* b)
{
	float t;
	if ((t = a->minXYZ.x - b->minXYZ.x) > b->cubeSize || -t > a->cubeSize) return false;
	if ((t = a->minXYZ.y - b->minXYZ.y) > b->cubeSize || -t > a->cubeSize) return false;
	if ((t = a->minXYZ.z - b->minXYZ.z) > b->cubeSize || -t > a->cubeSize) return false;
	return true;
}


int planeBoxOverlap(float normal[3], float vert[3], float maxbox[3]) // -NJMP-
{
	int q;
	float vmin[3], vmax[3], v;

	for (q = X; q <= Z; q++)
	{
		v = vert[q]; // -NJMP-

		if (normal[q] > 0.0f)
		{
			vmin[q] = -maxbox[q] - v; // -NJMP-
			vmax[q] = maxbox[q] - v; // -NJMP-
		}
		else
		{
			vmin[q] = maxbox[q] - v; // -NJMP-
			vmax[q] = -maxbox[q] - v; // -NJMP-
		}
	}

	if (DOT(normal, vmin) > 0.0f) return 0; // -NJMP-
	if (DOT(normal, vmax) >= 0.0f) return 1; // -NJMP-

	return 0;
}


/*======================== X-tests ========================*/
#define AXISTEST_X01(a, b, fa, fb) \
	p0 = a * v0[Y] - b * v0[Z]; \
	p2 = a * v2[Y] - b * v2[Z]; \
if (p0 < p2) { min = p0; max = p2; } else { min = p2; max = p0; } \
	rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z]; \
if (min > rad || max < -rad) return 0;

#define AXISTEST_X2(a, b, fa, fb) \
	p0 = a * v0[Y] - b * v0[Z]; \
	p1 = a * v1[Y] - b * v1[Z]; \
if (p0 < p1) { min = p0; max = p1; } else { min = p1; max = p0; } \
	rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z]; \
if (min > rad || max < -rad) return 0;


/*======================== Y-tests ========================*/
#define AXISTEST_Y02(a, b, fa, fb) \
	p0 = -a * v0[X] + b * v0[Z]; \
	p2 = -a * v2[X] + b * v2[Z]; \
if (p0 < p2) { min = p0; max = p2; } else { min = p2; max = p0; } \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z]; \
if (min > rad || max < -rad) return 0;

#define AXISTEST_Y1(a, b, fa, fb) \
	p0 = -a * v0[X] + b * v0[Z]; \
	p1 = -a * v1[X] + b * v1[Z]; \
if (p0 < p1) { min = p0; max = p1; } else { min = p1; max = p0; } \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z]; \
if (min > rad || max < -rad) return 0;


/*======================== Z-tests ========================*/
#define AXISTEST_Z12(a, b, fa, fb) \
	p1 = a * v1[X] - b * v1[Y]; \
	p2 = a * v2[X] - b * v2[Y]; \
if (p2 < p1) { min = p2; max = p1; } else { min = p1; max = p2; } \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y]; \
if (min > rad || max < -rad) return 0;

#define AXISTEST_Z0(a, b, fa, fb) \
	p0 = a * v0[X] - b * v0[Y]; \
	p1 = a * v1[X] - b * v1[Y]; \
if (p0 < p1) { min = p0; max = p1; } else { min = p1; max = p0; } \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y]; \
if (min > rad || max < -rad) return 0;


int TrangleAABBCollision(CAABB* pAABB, CTriangle* triangle)
{
	float triverts[3][3];
	float boxhalfsize[3];
	float boxcenter[3];

	triverts[0][X] = triangle->P1.x;
	triverts[0][Y] = triangle->P1.y;
	triverts[0][Z] = triangle->P1.z;

	triverts[1][X] = triangle->P2.x;
	triverts[1][Y] = triangle->P2.y;
	triverts[1][Z] = triangle->P2.z;

	triverts[2][X] = triangle->P3.x;
	triverts[2][Y] = triangle->P3.y;
	triverts[2][Z] = triangle->P3.z;

	boxcenter[X] = pAABB->minXYZ.x + pAABB->cubeSize * 0.5f;
	boxcenter[Y] = pAABB->minXYZ.y + pAABB->cubeSize * 0.5f;
	boxcenter[Z] = pAABB->minXYZ.z + pAABB->cubeSize * 0.5f;

	for (int index = 0; index < 3; index++)
	{
		boxhalfsize[index] = pAABB->cubeSize * 0.5f;
	}
	/*    use separating axis theorem to test overlap between triangle and box */
	/*    need to test for overlap in these directions: */
	/*    1) the {x,y,z}-directions (actually, since we use the AABB of the triangle */
	/*       we do not even need to test these) */
	/*    2) normal of the triangle */
	/*    3) crossproduct(edge from tri, {x,y,z}-directin) */
	/*       this gives 3x3=9 more tests */
	float v0[3], v1[3], v2[3];
	//float axis[3];
	float min, max, p0, p1, p2, rad, fex, fey, fez; // -NJMP- "d" local variable removed
	float normal[3], e0[3], e1[3], e2[3];

	/* This is the fastest branch on Sun */
	/* move everything so that the boxcenter is in (0,0,0) */
	SUB(v0, triverts[0], boxcenter);
	SUB(v1, triverts[1], boxcenter);
	SUB(v2, triverts[2], boxcenter);

	/* compute triangle edges */
	SUB(e0, v1, v0); /* tri edge 0 */
	SUB(e1, v2, v1); /* tri edge 1 */
	SUB(e2, v0, v2); /* tri edge 2 */

	/* Bullet 3:  */
	/*  test the 9 tests first (this was faster) */
	fex = fabsf(e0[X]);
	fey = fabsf(e0[Y]);
	fez = fabsf(e0[Z]);

	AXISTEST_X01(e0[Z], e0[Y], fez, fey);
	AXISTEST_Y02(e0[Z], e0[X], fez, fex);
	AXISTEST_Z12(e0[Y], e0[X], fey, fex);

	fex = fabsf(e1[X]);
	fey = fabsf(e1[Y]);
	fez = fabsf(e1[Z]);

	AXISTEST_X01(e1[Z], e1[Y], fez, fey);
	AXISTEST_Y02(e1[Z], e1[X], fez, fex);
	AXISTEST_Z0(e1[Y], e1[X], fey, fex);

	fex = fabsf(e2[X]);
	fey = fabsf(e2[Y]);
	fez = fabsf(e2[Z]);

	AXISTEST_X2(e2[Z], e2[Y], fez, fey);
	AXISTEST_Y1(e2[Z], e2[X], fez, fex);
	AXISTEST_Z12(e2[Y], e2[X], fey, fex);

	/* Bullet 1: */
	/*  first test overlap in the {x,y,z}-directions */
	/*  find min, max of the triangle each direction, and test for overlap in */
	/*  that direction -- this is equivalent to testing a minimal AABB around */
	/*  the triangle against the AABB */

	/* test in X-direction */
	FINDMINMAX(v0[X], v1[X], v2[X], min, max);
	if (min > boxhalfsize[X] || max < -boxhalfsize[X]) return 0;

	/* test in Y-direction */
	FINDMINMAX(v0[Y], v1[Y], v2[Y], min, max);
	if (min > boxhalfsize[Y] || max < -boxhalfsize[Y]) return 0;

	/* test in Z-direction */
	FINDMINMAX(v0[Z], v1[Z], v2[Z], min, max);
	if (min > boxhalfsize[Z] || max < -boxhalfsize[Z]) return 0;

	/* Bullet 2: */
	/*  test if the box intersects the plane of the triangle */
	/*  compute plane equation of triangle: normal*x+d=0 */
	CROSS(normal, e0, e1);

	// -NJMP- (line removed here)

	if (!planeBoxOverlap(normal, v0, boxhalfsize)) return 0; // -NJMP-

	return 1; /* box and triangle overlaps */
}


void DrawCube(CAABB* pAABB)
{
	CGameObject* pGameObject = new CGameObject();
	//CPhysicalProp physicalProps;
	CMeshDesc meshDesc;
	pGameObject->m_physicalProperties.position = pAABB->minXYZ;		// *******
	if (pAABB->bHasTriangleInIt == true)
		meshDesc.debugColour = CVector4f(1.0f, 0.0f, 0.0f, 1.0f);
	else
		meshDesc.debugColour = CVector4f(0.0f, 1.0f, 0.0f, 1.0f);
	meshDesc.modelName = "Cube.ply";
	meshDesc.scale = pAABB->cubeSize;
	meshDesc.bIsWireframe = true;
	meshDesc.bUseDebugColour = true;

	//pGameObject->setPhysicalProperties(physicalProps);
	pGameObject->addMeshDescription(meshDesc);
	::g_vecPhysDebugObjects.push_back(pGameObject);
}

void DrawSphere(CAABB* pChildAABB)
{
	for (unsigned int index = 0; index < pChildAABB->vec_pTriangles.size(); index++)
	{
		CGameObject* pGameObject = new CGameObject();
		pGameObject->m_physicalProperties.position = pChildAABB->vec_pTriangles[index]->P1;
		CMeshDesc meshDesc;
		meshDesc.modelName = "sphere_UV_xyz.ply";
		meshDesc.scale = 0.1f;
		meshDesc.debugColour = CVector4f(0.0f, 1.0f, 0.0f, 1.0f);
		meshDesc.bUseDebugColour = true;
		pGameObject->addMeshDescription(meshDesc);
		::g_vecPhysDebugObjects.push_back(pGameObject);
	}
	for (unsigned int index = 0; index < pChildAABB->vec_pTriangles.size(); index++)
	{
		CGameObject* pGameObject = new CGameObject();
		pGameObject->m_physicalProperties.position = pChildAABB->vec_pTriangles[index]->P2;
		CMeshDesc meshDesc;
		meshDesc.modelName = "sphere_UV_xyz.ply";
		meshDesc.scale = 0.1f;
		meshDesc.debugColour = CVector4f(0.0f, 1.0f, 0.0f, 1.0f);
		meshDesc.bUseDebugColour = true;
		pGameObject->addMeshDescription(meshDesc);
		::g_vecPhysDebugObjects.push_back(pGameObject);
	}
	for (unsigned int index = 0; index < pChildAABB->vec_pTriangles.size(); index++)
	{
		CGameObject* pGameObject = new CGameObject();
		pGameObject->m_physicalProperties.position = pChildAABB->vec_pTriangles[index]->P3;
		CMeshDesc meshDesc;
		meshDesc.modelName = "sphere_UV_xyz.ply";
		meshDesc.scale = 0.1f;
		meshDesc.debugColour = CVector4f(0.0f, 1.0f, 0.0f, 1.0f);
		meshDesc.bUseDebugColour = true;
		pGameObject->addMeshDescription(meshDesc);
		::g_vecPhysDebugObjects.push_back(pGameObject);
	}
}

CVector3f ClosestPtPointTriangle(CVector3f p, CVector3f a, CVector3f b, CVector3f c, bool &bIsInsideFaceRegion)
{
	bIsInsideFaceRegion = false;

	// Check if P in vertex region outside A
	CVector3f ab = b - a;
	CVector3f ac = c - a;
	CVector3f ap = p - a;
	float d1 = CVector3f::DotProduct(ab, ap);
	float d2 = CVector3f::DotProduct(ac, ap);
	if ((d1 <= 0.0f) && (d2 <= 0.0f))			// barycentric coordinates (1,0,0)
	{
		return a;
	}

	// Check if P in vertex region outside B
	CVector3f bp = p - b;
	float d3 = CVector3f::DotProduct(ab, bp);
	float d4 = CVector3f::DotProduct(ac, bp);
	if ((d3 >= 0.0f) && (d4 <= d3))				// barycentric coordinates (0,1,0)
	{
		return b;
	}

	// Check if P in edge region of AB, if so return projection of P onto AB
	float vc = d1*d4 - d3*d2;
	if ((vc <= 0.0f) && (d1 >= 0.0f) && (d3 <= 0.0f))
	{
		float v = d1 / (d1 - d3);
		//return a + v * ab;		// barycentric coordinates (1-v,v,0)
		ab *= v;
		a += ab;
		return a;
	}

	// Check if P in vertex region outside C
	CVector3f cp = p - c;
	float d5 = CVector3f::DotProduct(ab, cp);
	float d6 = CVector3f::DotProduct(ac, cp);
	if ((d6 >= 0.0f) && (d5 <= d6))
	{
		//std::cout << "return c is called" << std::endl;
		return c;		// barycentric coordinates (0,0,1)
	}

	// Check if P in edge region of AC, if so return projection of P onto AC
	float vb = d5*d2 - d1*d6;
	if ((vb <= 0.0f) && (d2 >= 0.0f) && (d6 <= 0.0f))
	{
		float w = d2 / (d2 - d6);
		//return a + w * ac;	// barycentric coordinates (1-w, 0, w)
		ac *= w;
		a += ac;
		return  a;
	}

	// Check if P in edge region of BC, if so return projection of P onto BC
	float va = d3*d6 - d5*d4;
	if ((va <= 0.0f) && ((d4 - d3) >= 0.0f) && ((d5 - d6) >= 0.0f))
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));

		//return b + w * (c - b);		// barycentric coordinates (0,1-w,w)
		CVector3f tempCBW = c - b;
		tempCBW *= w;
		b += tempCBW;
		return b;
	}

	// P inside face region. 
	bIsInsideFaceRegion = true;

	// Compute Q through its barycentri coordinates (u,v,w)
	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;

	//return a + ab * v + ac * w;		// = u*a + v*b + w*c, u = va * denom = 1.0f - v - w
	ab *= v;
	ac *= w;
	a += ab;
	a += ac;
	return a;

}

void NormalizeVectorYOLO(float &x, float &y, float &z)
{
	// Convert to double so there is no loss...
	double x2 = static_cast<double>(x)* static_cast<double>(x);
	double y2 = static_cast<double>(y)* static_cast<double>(y);
	double z2 = static_cast<double>(z)* static_cast<double>(z);
	double length = x2 + y2 + z2;
	if (length == 0)
	{
		x = y = z = 0.0f;
	}
	else
	{	// Multiply by reciprical as multiplies are faster than divides (usually)
		double reciprical = 1 / sqrt(length);
		x = static_cast<float>(static_cast<double>(x)* reciprical);
		y = static_cast<float>(static_cast<double>(y)* reciprical);
		z = static_cast<float>(static_cast<double>(z)* reciprical);
	}
}

float getDistanceBetweenPoints_YOLO2(CVector3f point1, CVector3f point2)
{
	float deltaXsquared = (point2.x - point1.x) * (point2.x - point1.x);  // pow()
	float deltaYsquared = (point2.y - point1.y) * (point2.y - point1.y);
	float deltaZsquared = (point2.z - point1.z) * (point2.z - point1.z);

	// There's a number of square root functions. 
	// You many have to include "math.h" or something else. 
	float distance = sqrt(deltaXsquared + deltaYsquared + deltaZsquared);

	return distance;
}

void CollisionDetection(unsigned int player_index, unsigned int player_Cindex, CAABB* pPlayerAABB, unsigned int objectID)
{
	std::map< unsigned int, CAABB* >::iterator itAABB;
	itAABB = g_mapCAABB.find(player_index);


	if (itAABB != g_mapCAABB.end())
	{
		// Oh crap! Blame Data!
		// first() is the "unsigned int"
		// second() is the actual thing that stored there.
		//Parent Cube
		CAABB* pParentAABB = itAABB->second;
		if (TestAABBAABB(pPlayerAABB, pParentAABB))  //player is inside parent cube
		{
			if (drawHitbox)
				DrawCube(pParentAABB);
		}

		//Child Cube
		std::map< unsigned int, CAABB* >::iterator child_itAABB;
		child_itAABB = itAABB->second->map_pchildAABB.find(player_Cindex);
		if (child_itAABB != itAABB->second->map_pchildAABB.end()) //inside the smaller cubes
		{
			CAABB* pChildAABB = child_itAABB->second;
			if (TestAABBAABB(pPlayerAABB, pChildAABB))  //player is inside child cube
			{
				if (drawHitbox)
					DrawCube(pChildAABB);

				if (pChildAABB->bHasTriangleInIt == true)
				{
					if (drawHitbox)
						DrawSphere(pChildAABB);
					std::vector <CTriangle *> vec_contactTriangles;
					std::vector <CVector3f> vec_contactShipHitBoxes;

					for (unsigned int index = 0; index < pChildAABB->vec_pTriangles.size(); index++)
					{
						if (TrangleAABBCollision(pPlayerAABB, pChildAABB->vec_pTriangles[index]) == 1)
						{
							vec_contactTriangles.push_back(pChildAABB->vec_pTriangles[index]);
						}
					}

					if (vec_contactTriangles.size() > 0)	//the AABB is touching at least one triangle so we use collision detection to reposition the sphere
					{
						//InsideThePlayerAABB()
						if (drawHitbox)
						{
							CGameObject* pGameObject = new CGameObject();
							pGameObject->m_physicalProperties.position = pPlayerAABB->minXYZ;		// *******
							CMeshDesc meshDesc;
							meshDesc.modelName = "Cube.ply";
							meshDesc.scale = pPlayerAABB->cubeSize;
							meshDesc.debugColour = CVector4f(1.0f, 0.0f, 0.0f, 1.0f);
							meshDesc.bIsWireframe = true;
							pGameObject->addMeshDescription(meshDesc);
							::g_vecPhysDebugObjects.push_back(pGameObject);
						}

						CVector3f avgNormal;
						CVector3f avgPlayerPosition, playerPos;
						CVector3f playerVelocity, newVelocity;
						CPhysicalProp physicalProp;
						g_pFactoryMediator->getPhysicalPropertiesByID(objectID, physicalProp);
						playerPos = physicalProp.position;

						//g_pFactoryMediator->GetObjectVelocityByID(::g_Player_ID, playerVelocity);
						float numberOfNormals = 0.0f;
						for (int index = 0; index != vec_contactTriangles.size(); index++)
						{
							CVector3f a(vec_contactTriangles[index]->P1.x, vec_contactTriangles[index]->P1.y, vec_contactTriangles[index]->P1.z);
							CVector3f b(vec_contactTriangles[index]->P2.x, vec_contactTriangles[index]->P2.y, vec_contactTriangles[index]->P2.z);
							CVector3f c(vec_contactTriangles[index]->P3.x, vec_contactTriangles[index]->P3.y, vec_contactTriangles[index]->P3.z);
							//if the sphere is inside the triangles face aka not on the edge

							for (int jndex = 0; jndex != g_vecShipHitBoxes.size(); jndex++) //finds which of the six points is the closest
							{
								bool bIsInsideFace = false;
								CVector3f closestPoint = ClosestPtPointTriangle(g_vecShipHitBoxes[jndex]->m_physicalProperties.position, a, b, c, bIsInsideFace);
								float distance = getDistanceBetweenPoints_YOLO2(g_vecShipHitBoxes[jndex]->m_physicalProperties.position, closestPoint);
								if (distance < 0.2f)
								{
									if (drawHitbox)
									{
										CGameObject* pGameObject = new CGameObject();
										pGameObject->m_physicalProperties.position = g_vecShipHitBoxes[jndex]->m_physicalProperties.position;
										CMeshDesc meshDesc;
										meshDesc.modelName = "sphere_UV_xyz.ply";
										meshDesc.scale = 0.2f;
										meshDesc.debugColour = CVector4f(1.0f, 1.0f, 0.0f, 1.0f);
										pGameObject->addMeshDescription(meshDesc);
										::g_vecPhysDebugObjects.push_back(pGameObject);
									}
									playerPos.x = (vec_contactTriangles[index]->normal.x * 0.1f) + closestPoint.x;
									playerPos.y = (vec_contactTriangles[index]->normal.y * 0.1f) + closestPoint.y;
									playerPos.z = (vec_contactTriangles[index]->normal.z * 0.1f) + closestPoint.z;
									avgPlayerPosition += playerPos;
									//vec_contactShipHitBoxes.push_back(closestPoint);
									avgNormal.x += vec_contactTriangles[index]->normal.x;
									avgNormal.y += vec_contactTriangles[index]->normal.y;
									avgNormal.z += vec_contactTriangles[index]->normal.z;
									numberOfNormals++;
								}
							}
						}
						if (numberOfNormals != 0.0f)
						{
							CPhysicalProp tempProp;
							g_pFactoryMediator->getPhysicalPropertiesByID(objectID, tempProp);
							//tempProp.velocity = CVector3f(0.0f, 0.0f, 0.0f);
							//tempProp.rotationalSpeed = glm::fquat(glm::vec3(0.0f, 0.0f, 0.0f));
							//g_pFactoryMediator->setPhysicalPropertiesByID(::g_Player_ID, tempProp);
							//g_pFactoryMediator->UpdateObjectPositionByID(::g_Player_ID, avgPlayerPosition);

							avgNormal.x = avgNormal.x / numberOfNormals;
							avgNormal.y = avgNormal.y / numberOfNormals;
							avgNormal.z = avgNormal.z / numberOfNormals;

							newVelocity.x = avgNormal.x * playerVelocity.x;
							newVelocity.y = avgNormal.y * playerVelocity.y;
							newVelocity.z = avgNormal.z * playerVelocity.z;
							tempProp.velocity = newVelocity;

							avgPlayerPosition.x = avgPlayerPosition.x / numberOfNormals;
							avgPlayerPosition.y = avgPlayerPosition.y / numberOfNormals;
							avgPlayerPosition.z = avgPlayerPosition.z / numberOfNormals;
							tempProp.position = avgPlayerPosition;
							/*if (newVelocity.x > 0.01)
							tempProp.position.x *= newVelocity.x;
							if (newVelocity.y > 0.01)
							tempProp.position.y *= newVelocity.y;
							if (newVelocity.z > 0.01)
							tempProp.position.z *= newVelocity.z;*/
							g_pFactoryMediator->setPhysicalPropertiesByID(objectID, tempProp);
							//g_pFactoryMediator->UpdateObjectVelocityByID(::g_Player_ID, newVelocity);
						}
						//if (vec_contactShipHitBoxes.size() > 0)	//at least one point is touching a triangle, change
						//{
						//	for (int index = 0; index != vec_contactTriangles.size(); index++)
						//	{
						//		for (int jndex = 0; jndex != vec_contactShipHitBoxes.size(); jndex++) //finds which of the six points is the closest
						//		{

						//		}
						//	}

						//}
						//float radius = 1.0f;
						//CVector3f avgPlayerPosition;
						//for (int index = 0; index != vec_contactTriangles.size(); index++)
						//{
						//	CVector3f playerPos;
						//	CVector3f a(vec_contactTriangles[index]->P1.x, vec_contactTriangles[index]->P1.y, vec_contactTriangles[index]->P1.z);
						//	CVector3f b(vec_contactTriangles[index]->P2.x, vec_contactTriangles[index]->P2.y, vec_contactTriangles[index]->P2.z);
						//	CVector3f c(vec_contactTriangles[index]->P3.x, vec_contactTriangles[index]->P3.y, vec_contactTriangles[index]->P3.z);
						//	bool bIsInsideFace = false;  //if the sphere is inside the triangles face aka not on the edge
						//	CVector3f closestPoint = ClosestPtPointTriangle(playerPos, a, b, c, bIsInsideFace);
						//	
						//	if (bIsInsideFace) //if the sphere is over the triangle's face
						//	{    //(triangle normal * radius) + closestpoint
						//		playerPos.x = (vec_contactTriangles[index]->normal.x * radius) + closestPoint.x;
						//		playerPos.y = (vec_contactTriangles[index]->normal.y * radius) + closestPoint.y;
						//		playerPos.z = (vec_contactTriangles[index]->normal.z * radius) + closestPoint.z;

						//	}
						//	else //calculate where on the edge of the triangle the sphere will be 
						//	{
						//		//Normalize(ballposition - closestpoint) * radius
						//		playerPos = playerPos - closestPoint;
						//		NormalizeVectorYOLO(playerPos.x, playerPos.y, playerPos.z);
						//		playerPos *= radius;
						//		playerPos += closestPoint;
						//	}
						//	avgPlayerPosition += playerPos; //takes the average from all the triangles touching the sphere
						//}
						////Calculates average of the triangles to determine new sphere location
						//avgPlayerPosition.x = avgPlayerPosition.x / (float)vec_contactTriangles.size();
						//avgPlayerPosition.y = avgPlayerPosition.y / (float)vec_contactTriangles.size();
						//avgPlayerPosition.z = avgPlayerPosition.z / (float)vec_contactTriangles.size();
						//g_pFactoryMediator->UpdateObjectPositionByID(g_Player_ID, avgPlayerPosition);
						//}

						vec_contactTriangles.clear();
					}
					else
					{
						//InsideThePlayerAABB()
						if (drawHitbox)
						{
							CGameObject* pGameObject = new CGameObject();
							pGameObject->m_physicalProperties.position = pPlayerAABB->minXYZ;		// *******
							CMeshDesc meshDesc;
							meshDesc.modelName = "Cube.ply";
							meshDesc.debugColour = CVector4f(0.0f, 1.0f, 0.0f, 1.0f);
							meshDesc.scale = pPlayerAABB->cubeSize;
							meshDesc.bIsWireframe = true;
							meshDesc.bUseDebugColour = true;
							pGameObject->addMeshDescription(meshDesc);
							::g_vecPhysDebugObjects.push_back(pGameObject);
						}
					}
				}
				else
				{
					//InsideThePlayerAABB()
					if (drawHitbox)
					{
						CGameObject* pGameObject = new CGameObject();
						pGameObject->m_physicalProperties.position = pPlayerAABB->minXYZ;		// *******
						CMeshDesc meshDesc;
						meshDesc.modelName = "Cube.ply";
						meshDesc.debugColour = CVector4f(0.0f, 1.0f, 0.0f, 1.0f);
						meshDesc.scale = pPlayerAABB->cubeSize;
						meshDesc.bIsWireframe = true;
						meshDesc.bUseDebugColour = true;
						pGameObject->addMeshDescription(meshDesc);
						::g_vecPhysDebugObjects.push_back(pGameObject);
					}
				}

			}
		}
	}
	return;
}

void CreateShipHitbox(CVector3f extent, CVector3f playerPos, unsigned int objectID)
{
	float sphereScale = 0.01f;
	CGameObject* pShipHitBoxGO = new CGameObject();
	pShipHitBoxGO->m_physicalProperties.position = extent;
	CMeshDesc meshDesc;
	meshDesc.modelName = "sphere_UV_xyz.ply";
	meshDesc.scale = sphereScale;
	meshDesc.debugColour = CVector4f(1.0f, 0.0f, 0.0f, 1.0f);
	meshDesc.bIsWireframe = true;
	pShipHitBoxGO->addMeshDescription(meshDesc);

	CPhysicalProp physicalProp;
	g_pFactoryMediator->getPhysicalPropertiesByID(objectID, physicalProp);
	//g_pFactoryMediator->GetObjectRotationByID(g_Player_ID, rotation);
	glm::mat4 matOrientation = glm::toMat4(physicalProp.orientation);
	glm::vec4 velThisFrame(pShipHitBoxGO->m_physicalProperties.position.x,
		pShipHitBoxGO->m_physicalProperties.position.y,
		pShipHitBoxGO->m_physicalProperties.position.z, 1.0f);
	// Calculate adjustment based on orientation (just like in the shader)
	glm::vec4 velAdjust = matOrientation * velThisFrame;
	// Update the linear velocity based on this acceleration step (this frame)
	pShipHitBoxGO->m_physicalProperties.position = CVector3f(playerPos.x + velAdjust.x, playerPos.y + velAdjust.y, playerPos.z + velAdjust.z);

	//pShipHitBoxGO->setPhysicalProperties(physicalProp);
	if (drawHitbox)
		::g_vecPhysDebugObjects.push_back(pShipHitBoxGO);
	::g_vecShipHitBoxes.push_back(pShipHitBoxGO);
}

void TriangleAABBCollision(float deltaTime)
{
	unsigned int objectID;
	if (::g_gameState.currentGameMode == CGameState::EDITING_OBJECTS)
		objectID = vecModelsInfo[g_gameState.selectedObjectID].ID;
	else
		objectID = ::g_Player_ID;
	std::vector<CMeshDesc> vecMeshDescs;
	g_pFactoryMediator->getRenderingPropertiesByID(objectID, vecMeshDescs);


	CPlyInfo shipModelInfo;
	//g_pModelLoader->GetRenderingInfoByModelFileName("FV_X1_Fighter.ply", shipModelInfo);
	g_pModelLoader->GetRenderingInfoByModelFileName(vecMeshDescs[0].modelName, shipModelInfo);
	// Place cubes where the player object is...
	CVector3f playerPos;
	CPhysicalProp physicalProp;
	g_pFactoryMediator->getPhysicalPropertiesByID(objectID, physicalProp);
	playerPos = physicalProp.position;
	//::g_pFactoryMediator->GetPositionByID(::g_Player_ID, playerPos);

	::g_vecShipHitBoxes.clear();
	CreateShipHitbox(shipModelInfo.minExtentX, playerPos, objectID);
	CreateShipHitbox(shipModelInfo.maxExtentX, playerPos, objectID);
	CreateShipHitbox(shipModelInfo.minExtentY, playerPos, objectID);
	CreateShipHitbox(shipModelInfo.maxExtentY, playerPos, objectID);
	CreateShipHitbox(shipModelInfo.minExtentZ, playerPos, objectID);
	CreateShipHitbox(shipModelInfo.maxExtentZ, playerPos, objectID);

	CAABB* pPlayerAABB = new CAABB();
	pPlayerAABB->cubeSize = shipModelInfo.extent;
	pPlayerAABB->minXYZ.x = (playerPos.x - (pPlayerAABB->cubeSize / 2.0f)) * pPlayerAABB->cubeSize;
	pPlayerAABB->minXYZ.y = (playerPos.y - (pPlayerAABB->cubeSize / 2.0f)) * pPlayerAABB->cubeSize;
	pPlayerAABB->minXYZ.z = (playerPos.z - (pPlayerAABB->cubeSize / 2.0f)) * pPlayerAABB->cubeSize;
	unsigned int playerIndex = pPlayerAABB->genIndex();
	// Add a "debug cube" where the player is...

	//std::cout << "PlayerPos x=" << playerPos.x << " y=" << playerPos.y << " z=" << playerPos.z << std::endl;


	//Looking at the ship from behind this is the lower corner towards the viewer
	/* Visual aid
	*   Max-----C
	*  / |	  /|
	* /  E---/-D               ^
	*F------G /    ship facing |
	*|  	|/
	*H------Min
	*/
	//Min corner
	int indexIndent = 10;
	float min_xPosC = ((int)pPlayerAABB->minXYZ.x - (int)pPlayerAABB->minXYZ.x % indexIndent) * 0.1f;
	float min_yPosC = ((int)pPlayerAABB->minXYZ.y - (int)pPlayerAABB->minXYZ.y % indexIndent) * 0.1f;
	float min_zPosC = ((int)pPlayerAABB->minXYZ.z - (int)pPlayerAABB->minXYZ.z % indexIndent) * 0.1f;
	float min_xPos = min_xPosC * 0.1f;
	float min_yPos = min_yPosC * 0.1f;
	float min_zPos = min_zPosC * 0.1f;

	unsigned int min_player_index = genIndex((int)min_xPos * 10, (int)min_yPos * 10, (int)min_zPos * 10);
	unsigned int min_player_Cindex = genIndex((int)min_xPosC, (int)min_yPosC, (int)min_zPosC);

	CollisionDetection(min_player_index, min_player_Cindex, pPlayerAABB, objectID);

	//Max Corner
	float max_xPosC = (((int)pPlayerAABB->minXYZ.x + (int)pPlayerAABB->cubeSize) - ((int)pPlayerAABB->minXYZ.x + (int)pPlayerAABB->cubeSize) % indexIndent) * 0.1f;
	float max_yPosC = (((int)pPlayerAABB->minXYZ.y + (int)pPlayerAABB->cubeSize) - ((int)pPlayerAABB->minXYZ.y + (int)pPlayerAABB->cubeSize) % indexIndent) * 0.1f;
	float max_zPosC = (((int)pPlayerAABB->minXYZ.z + (int)pPlayerAABB->cubeSize) - ((int)pPlayerAABB->minXYZ.z + (int)pPlayerAABB->cubeSize) % indexIndent) * 0.1f;
	float max_xPos = max_xPosC * 0.1f;
	float max_yPos = max_yPosC * 0.1f;
	float max_zPos = max_zPosC * 0.1f;

	unsigned int max_player_index = genIndex((int)max_xPos * 10, (int)max_yPos * 10, (int)max_zPos * 10);
	unsigned int max_player_Cindex = genIndex((int)max_xPosC, (int)max_yPosC, (int)max_zPosC);

	//If these two corners are in the same bounding box then no need to search further
	if (min_player_Cindex != max_player_Cindex)
	{
		CollisionDetection(max_player_index, max_player_Cindex, pPlayerAABB, objectID);
		unsigned int otherCorners;
		//only x is max
		otherCorners = genIndex((int)max_xPosC, (int)min_yPosC, (int)min_zPosC);
		if (otherCorners != min_player_Cindex && otherCorners != max_player_Cindex)
			CollisionDetection(genIndex((int)max_xPos * 10, (int)min_yPos * 10, (int)min_zPos * 10), otherCorners, pPlayerAABB, objectID);
		//only y is max
		otherCorners = genIndex((int)min_xPosC, (int)max_yPosC, (int)min_zPosC);
		if (otherCorners != min_player_Cindex && otherCorners != max_player_Cindex)
			CollisionDetection(genIndex((int)min_xPos * 10, (int)max_yPos * 10, (int)min_zPos * 10), otherCorners, pPlayerAABB, objectID);
		//only z is max
		otherCorners = genIndex((int)min_xPosC, (int)min_yPosC, (int)max_zPosC);
		if (otherCorners != min_player_Cindex && otherCorners != max_player_Cindex)
			CollisionDetection(genIndex((int)min_xPos * 10, (int)min_yPos * 10, (int)max_zPos * 10), otherCorners, pPlayerAABB, objectID);
		// x and y are max
		otherCorners = genIndex((int)max_xPosC, (int)max_yPosC, (int)min_zPosC);
		if (otherCorners != min_player_Cindex && otherCorners != max_player_Cindex)
			CollisionDetection(genIndex((int)max_xPos * 10, (int)max_yPos * 10, (int)min_zPos * 10), otherCorners, pPlayerAABB, objectID);
		// x and z are max
		otherCorners = genIndex((int)max_xPosC, (int)min_yPosC, (int)max_zPosC);
		if (otherCorners != min_player_Cindex && otherCorners != max_player_Cindex)
			CollisionDetection(genIndex((int)max_xPos * 10, (int)min_yPos * 10, (int)max_zPos * 10), otherCorners, pPlayerAABB, objectID);
		// y and z are max
		otherCorners = genIndex((int)min_xPosC, (int)max_yPosC, (int)max_zPosC);
		if (otherCorners != min_player_Cindex && otherCorners != max_player_Cindex)
			CollisionDetection(genIndex((int)min_xPos * 10, (int)max_yPos * 10, (int)max_zPos * 10), otherCorners, pPlayerAABB, objectID);
	}
	g_vecShipHitBoxes.clear();
}


void GenerateAABBWorld()
{
	float numberOfCubes = 10.0f;
	for (float xPos = 0.0f; xPos < numberOfCubes; xPos++)
	{
		for (float yPos = 0.0f; yPos < numberOfCubes; yPos++)
		{
			for (float zPos = 0.0f; zPos < numberOfCubes; zPos++)
			{
				CAABB* pTempAABB = new CAABB();
				pTempAABB->cubeSize = 100.0f;
				pTempAABB->minXYZ.x = xPos * pTempAABB->cubeSize;
				pTempAABB->minXYZ.y = yPos * pTempAABB->cubeSize;
				pTempAABB->minXYZ.z = zPos * pTempAABB->cubeSize;
				pTempAABB->bHasTriangleInIt = false;
				//unsigned int theIndex = pTempAABB->genIndex();
				//unsigned int theIndex = genIndex((int)pTempAABB->minXYZ.x, (int)pTempAABB->minXYZ.y, (int)pTempAABB->minXYZ.z);
				unsigned int theIndex = genIndex((int)xPos * 10, (int)yPos * 10, (int)zPos * 10);
				pTempAABB->m_index = theIndex;

				for (float xPosC = 0.0f; xPosC < numberOfCubes; xPosC++)
				{
					for (float yPosC = 0.0f; yPosC < numberOfCubes; yPosC++)
					{
						for (float zPosC = 0.0f; zPosC < numberOfCubes; zPosC++)
						{
							CAABB* pChildAABB = new CAABB();
							pChildAABB->cubeSize = 10.0f;
							pChildAABB->minXYZ.x = xPosC * pChildAABB->cubeSize + pTempAABB->minXYZ.x;
							pChildAABB->minXYZ.y = yPosC * pChildAABB->cubeSize + pTempAABB->minXYZ.y;
							pChildAABB->minXYZ.z = zPosC * pChildAABB->cubeSize + pTempAABB->minXYZ.z;
							pChildAABB->bHasTriangleInIt = false;
							//unsigned int childIndex = pChildAABB->genIndex();
							unsigned int childIndex = genIndex((int)xPosC + (int)xPos * 10, (int)yPosC + (int)yPos * 10, (int)zPosC + (int)zPos * 10);
							pChildAABB->m_index = childIndex;

							pTempAABB->map_pchildAABB[childIndex] = pChildAABB;
							//std::cout << childIndex << " x=" << xPosC << " y=" << yPosC << " z=" << zPosC << std::endl;
						}
					}
				}


				g_mapCAABB[theIndex] = pTempAABB;
				//std::cout << theIndex << " x=" << xPos << " y=" << yPos << " z=" << zPos << std::endl;
			}
		}
	}

	CPlyFile5nt* pEnvironment = ::g_pModelLoader->getOriginalModelInfo("NubianComplex2.ply");
	for (int index = 0; index < pEnvironment->GetNumberOfElements(); index++)
	{
		//Point ClosestPtPointTriangle( Point p, Point a, Point b, Point c, bool &bIsInsideFaceRegion );
		PLYELEMENT tempTriangle = pEnvironment->getElement_at(index);
		// This returns the index values, not the vertices...

		PLYVERTEX tempA = pEnvironment->getVertex_at(tempTriangle.vertex_index_1);
		PLYVERTEX tempB = pEnvironment->getVertex_at(tempTriangle.vertex_index_2);
		PLYVERTEX tempC = pEnvironment->getVertex_at(tempTriangle.vertex_index_3);

		int indexIndent = 10;

		/*int xPos = ((int)tempA.xyz.x - (int)tempA.xyz.x % indexIndent);
		int yPos = ((int)tempA.xyz.y - (int)tempA.xyz.y % indexIndent);
		int zPos = ((int)tempA.xyz.z - (int)tempA.xyz.z % indexIndent);*/
		float xPosC = ((int)tempA.xyz.x - (int)tempA.xyz.x % indexIndent) * 0.1f;
		float yPosC = ((int)tempA.xyz.y - (int)tempA.xyz.y % indexIndent) * 0.1f;
		float zPosC = ((int)tempA.xyz.z - (int)tempA.xyz.z % indexIndent) * 0.1f;
		float xPos = xPosC * 0.1f;
		float yPos = yPosC * 0.1f;
		float zPos = zPosC * 0.1f;

		unsigned int triA_index = genIndex((int)xPos * 10, (int)yPos * 10, (int)zPos * 10);
		unsigned int triA_Cindex = genIndex((int)xPosC, (int)yPosC, (int)zPosC);
		//std::cout << triA_index << " x=" << xPos << " y=" << yPos << " z=" << zPos << std::endl;
		//std::cout << triA_Cindex << "C x=" << xPosC << " y=" << yPosC << " z=" << zPosC << std::endl;


		xPosC = ((int)tempB.xyz.x - (int)tempB.xyz.x % indexIndent) * 0.1f;
		yPosC = ((int)tempB.xyz.y - (int)tempB.xyz.y % indexIndent) * 0.1f;
		zPosC = ((int)tempB.xyz.z - (int)tempB.xyz.z % indexIndent) * 0.1f;
		xPos = xPosC * 0.1f;
		yPos = yPosC * 0.1f;
		zPos = zPosC * 0.1f;

		unsigned int triB_index = genIndex((int)xPos * 10, (int)yPos * 10, (int)zPos * 10);
		unsigned int triB_Cindex = genIndex((int)xPosC, (int)yPosC, (int)zPosC);
		//unsigned int triB_index = zPos + yPos * indexIndent + xPos * indexIndent * indexIndent + 1000000000;
		//std::cout << triB_index << " x=" << xPos << " y=" << yPos << " z=" << zPos << std::endl;
		//std::cout << triB_Cindex << "C x=" << xPosC << " y=" << yPosC << " z=" << zPosC << std::endl;

		xPosC = ((int)tempC.xyz.x - (int)tempC.xyz.x % indexIndent) * 0.1f;
		yPosC = ((int)tempC.xyz.y - (int)tempC.xyz.y % indexIndent) * 0.1f;
		zPosC = ((int)tempC.xyz.z - (int)tempC.xyz.z % indexIndent) * 0.1f;
		xPos = xPosC * 0.1f;
		yPos = yPosC * 0.1f;
		zPos = zPosC * 0.1f;

		unsigned int triC_index = genIndex((int)xPos * 10, (int)yPos * 10, (int)zPos * 10);
		unsigned int triC_Cindex = genIndex((int)xPosC, (int)yPosC, (int)zPosC);
		//unsigned int triC_index = zPos + yPos * indexIndent + xPos * indexIndent * indexIndent + 1000000000;
		//std::cout << triC_index << " x=" << xPos << " y=" << yPos << " z=" << zPos << std::endl;
		//std::cout << triC_Cindex << "C x=" << xPosC << " y=" << yPosC << " z=" << zPosC << std::endl;

		//std::cout << index << " a=" << triA_index << " b=" << triB_index << " c=" << triA_index << std::endl;

		//creates triangle temp to store inside
		CTriangle* tempTri = new CTriangle();
		tempTri->V1 = tempTriangle.vertex_index_1;
		tempTri->V2 = tempTriangle.vertex_index_2;
		tempTri->V3 = tempTriangle.vertex_index_3;
		tempTri->P1 = tempA.xyz;
		tempTri->P2 = tempB.xyz;
		tempTri->P3 = tempC.xyz;
		tempTri->normal = tempTriangle.normal;

		//g_mapCAABB[triA_index]->vec_pTriangles.push_back(tempTri);
		g_mapCAABB[triA_index]->map_pchildAABB[triA_Cindex]->vec_pTriangles.push_back(tempTri);
		if (g_mapCAABB[triA_index]->map_pchildAABB[triA_Cindex]->bHasTriangleInIt == false)
		{
			g_mapCAABB[triA_index]->map_pchildAABB[triA_Cindex]->bHasTriangleInIt = true;
			if (g_mapCAABB[triA_index]->bHasTriangleInIt == false)
			{
				g_mapCAABB[triA_index]->bHasTriangleInIt = true;
			}
		}
		//check if the other triangle points are within the same Bounding box
		if (triB_Cindex != triA_Cindex)
		{
			g_mapCAABB[triB_index]->map_pchildAABB[triB_Cindex]->vec_pTriangles.push_back(tempTri);
			if (triB_index != triA_index)
			{
				//g_mapCAABB[triB_index]->vec_pTriangles.push_back(tempTri);
				if (g_mapCAABB[triB_index]->map_pchildAABB[triB_Cindex]->bHasTriangleInIt == false)
				{
					g_mapCAABB[triB_index]->map_pchildAABB[triB_Cindex]->bHasTriangleInIt = true;
					if (g_mapCAABB[triB_index]->bHasTriangleInIt == false)
						g_mapCAABB[triB_index]->bHasTriangleInIt = true;
				}
			}
		}
		if (triC_Cindex != triA_Cindex && triC_Cindex != triB_Cindex)
		{
			//g_mapCAABB[triC_index]->vec_pTriangles.push_back(tempTri);
			g_mapCAABB[triC_index]->map_pchildAABB[triC_Cindex]->vec_pTriangles.push_back(tempTri);
			if (g_mapCAABB[triC_index]->map_pchildAABB[triC_Cindex]->bHasTriangleInIt == false)
			{
				g_mapCAABB[triC_index]->map_pchildAABB[triC_Cindex]->bHasTriangleInIt = true;
				if (g_mapCAABB[triC_index]->bHasTriangleInIt == false)
				{
					g_mapCAABB[triC_index]->bHasTriangleInIt = true;
					//std::cout << index << " a=" << triA_index << " b=" << triB_index << " c=" << triC_index << std::endl;
				}
			}
		}
	}


}