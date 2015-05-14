#ifndef _Physics_HG_
#define _Physics_HG_

#include "../CCommonTypes/CVector3f.h" 


// These are mainly taken from the AMAZING Christer Ericson book "Real-Time Collision Detection"
namespace Physics
{
	typedef CVector3f Point;
	//struct Point
	//{
	//	Point() : x(0.0f), y(0.0f), z(0.0f) {};
	//	float x,y,z;
	//};

	struct Point2D 
	{
		Point2D() : x(0.0f), y(0.0f) {};
		float x,y;
	};

	struct Plane 
	{
		CVector3f n;  // Plane normal. Points x on the plane satisfy Dot(n,x) = d
		float d;   // d = dot(n,p) for a given point p on the plane
		Plane( Point a, Point b, Point c );
		void ComputePlane(Point a, Point b, Point c);
	};

	// Region R = { (x, y, z) | (x-c.x)^2 + (y-c.y)^2 + (z-c.z)^2 <= r^2 }
	struct Sphere 
	{
		Point c; // Sphere center
		float r; // Sphere radius
	};


	// Compute barycentric coordinates (u, v, w) for 
	// point p with respect to triangle (a, b, c)
	void Barycentric_A(Point a, Point b, Point c, Point p, float &u, float &v, float &w);
	// "B" uses triangle area function (so is faster??)
	void Barycentric_B(Point a, Point b, Point c, Point p, float &u, float &v, float &w);

	inline float TriArea2D(float x1, float y1, float x2, float y2, float x3, float y3);

	// Test if point p is contained in triangle (a, b, c)
	int TestPointTriangle(Point p, Point a, Point b, Point c);


	// Given three noncollinear points (ordered ccw), compute the plane equation
	Plane ComputePlane(Point a, Point b, Point c);

	int IsConvexQuad(Point a, Point b, Point c, Point d);

	// Return index i of point p[i] farthest from the edge ab, to the left of the edge
	int PointFarthestFromEdge(Point2D a, Point2D b, Point2D p[], int n);


	int TestSphereSphere(Sphere a, Sphere b);

	// Compute indices to the two most separated points of the (up to) six points
	// defining the AABB encompassing the point set. Return these as min and max.
	void MostSeparatedPointsOnAABB(int &min, int &max, Point pt[], int numPts);

	void SphereFromDistantPoints(Sphere &s, Point pt[], int numPts);

	// Closest point to triangle
	Point ClosestPtPointTriangle( Point p, Point a, Point b, Point c );
	Point ClosestPtPointTriangle( Point p, Point a, Point b, Point c, bool &bIsInsideFaceRegion );
};

#endif

