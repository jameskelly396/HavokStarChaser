#include "Physics.h"
#include <math.h>		// for abs()

int x = 2;

// Compute barycentric coordinates (u, v, w) for 
// point p with respect to triangle (a, b, c)
void Physics::Barycentric_A(Point a, Point b, Point c, Point p, float &u, float &v, float &w)
{
    CVector3f v0(b.x - a.x, b.y - a.y, b.z - a.z);	// Vector v0 = b - a
	CVector3f v1(c.x - a.x, c.y - a.y, c.z - a.z);  // Vector v1 = c - a
	CVector3f v2(p.x - a.x, p.y - a.y, p.z - a.z);	// Vector v2 = p - a;
    float d00 = CVector3f::DotProduct( v0, v0 );	// Dot(v0, v0);
    float d01 = CVector3f::DotProduct( v0, v1 );	// Dot(v0, v1);
    float d11 = CVector3f::DotProduct( v1, v1 );	// Dot(v1, v1);
    float d20 = CVector3f::DotProduct( v2, v0 );	// Dot(v2, v0);
    float d21 = CVector3f::DotProduct( v2, v1 );	// Dot(v2, v1);
    float denom = d00 * d11 - d01 * d01;
    v = (d11 * d20 - d01 * d21) / denom;
    w = (d00 * d21 - d01 * d20) / denom;
    u = 1.0f - v - w;
} 

//
//--------------------------------------------------------------------------------
//
//u = SignedArea(PBC)/SignedArea(ABC),
//v = SignedArea(PCA)/SignedArea(ABC), and
//w = SignedArea(PAB)/SignedArea(ABC) = 1 - u - v
//
//--------------------------------------------------------------------------------
//
//SignedArea(PBC) = Dot(Cross(B-P, C-P), Normalize(Cross(B-A, C-A))). 
//
//--------------------------------------------------------------------------------
//
inline float Physics::TriArea2D(float x1, float y1, float x2, float y2, float x3, float y3)
{
    return (x1-x2)*(y2-y3) - (x2-x3)*(y1-y2);
}

// Compute barycentric coordinates (u, v, w) for 
// point p with respect to triangle (a, b, c)
void Physics::Barycentric_B(Point a, Point b, Point c, Point p, float &u, float &v, float &w)
{
    // Unnormalized triangle normal
    CVector3f m = CVector3f::CrossProduct( CVector3f(b.x, b.y, b.z) - CVector3f(a.x, a.y, a.z), 
		                                   CVector3f(c.x, c.y, c.z) - CVector3f(a.x, a.y, a.z) );	// Vector m = Cross(b - a, c - a);
    // Nominators and one-over-denominator for u and v ratios
    float nu, nv, ood;
    // Absolute components for determining projection plane
    float x = abs(m.x), y = abs(m.y), z = abs(m.z);

    // Compute areas in plane of largest projection
    if (x >= y && x >= z) {
        // x is largest, project to the yz plane
        nu = TriArea2D(p.y, p.z, b.y, b.z, c.y, c.z); // Area of PBC in yz plane
        nv = TriArea2D(p.y, p.z, c.y, c.z, a.y, a.z); // Area of PCA in yz plane
        ood = 1.0f / m.x;                             // 1/(2*area of ABC in yz plane)
    } else if (y >= x && y >= z) {
        // y is largest, project to the xz plane
        nu = TriArea2D(p.x, p.z, b.x, b.z, c.x, c.z);
        nv = TriArea2D(p.x, p.z, c.x, c.z, a.x, a.z);
        ood = 1.0f / -m.y;
    } else {
        // z is largest, project to the xy plane
        nu = TriArea2D(p.x, p.y, b.x, b.y, c.x, c.y);
        nv = TriArea2D(p.x, p.y, c.x, c.y, a.x, a.y);
        ood = 1.0f / m.z;
    }
    u = nu * ood;
    v = nv * ood;
    w = 1.0f - u - v;
}

// Test if point p is contained in triangle (a, b, c)
int Physics::TestPointTriangle(Point p, Point a, Point b, Point c)
{
    float u, v, w;
    Barycentric_B(a, b, c, p, u, v, w);
    return v >= 0.0f && w >= 0.0f && (v + w) <= 1.0f;
}


Physics::Plane::Plane( Point a, Point b, Point c )
{
	this->ComputePlane( a, b, c );
	return;
}


// Given three noncollinear points (ordered ccw), compute the plane equation
void Physics::Plane::ComputePlane(Point a, Point b, Point c)
{
    //Plane p;
    //p.n = Normalize(Cross(b - a, c - a));
    //p.d = Dot(p.n, a);
    //return p;
	this->n.Normalize( CVector3f::CrossProduct( CVector3f(b.x, b.y, b.z) - CVector3f(a.x, a.y, a.z), 
		                                        CVector3f(c.x, c.y, c.z) - CVector3f(a.z, a.y, a.z) ) );
	this->d = CVector3f::DotProduct( this->n, CVector3f( a.x, a.y, a.z ) );

	return;
}


// Test if quadrilateral (a, b, c, d) is convex
int Physics::IsConvexQuad(Point a, Point b, Point c, Point d)
{
    // Quad is nonconvex if Dot(Cross(bd, ba), Cross(bd, bc)) >= 0
    CVector3f bda = CVector3f::CrossProduct( CVector3f(d.x, d.y, d.z) - CVector3f(b.x, b.y, b.z),	// Vector bda = Cross(d - b, a - b);
		                                     CVector3f(a.x, a.y, a.z) - CVector3f(b.x, b.y, b.z) ); 
    CVector3f bdc = CVector3f::CrossProduct( CVector3f(d.x, d.y, d.z) - CVector3f(b.x, b.y, b.z), 
											 CVector3f(c.x, c.y, c.z) - CVector3f(b.x, b.y, b.z) );	// Vector bdc = Cross(d - b, c - b);

    if ( CVector3f::DotProduct(bda, bdc) >= 0.0f )	//   if (Dot(bda, bdc) >= 0.0f) 
	{
		return 0;
	}

    // Quad is now convex iff Dot(Cross(ac, ad), Cross(ac, ab)) < 0
    
	CVector3f acd = CVector3f::CrossProduct( CVector3f(c.x, c.y, c.z) - CVector3f(a.x, a.y, a.z),	// Vector acd = Cross(c - a, d - a);
		                                     CVector3f(d.x, d.y, d.z) - CVector3f(a.x, a.y, a.z) );

	CVector3f acb = CVector3f::CrossProduct( CVector3f(c.x, c.y, c.z) - CVector3f(a.x, a.y, a.z),	// Vector acb = Cross(c - a, b - a);
		                                     CVector3f(b.x, b.y, b.z) - CVector3f(a.x, a.y, a.z) );

    return CVector3f::DotProduct(acd, acb) < 0.0f;		//   Dot(acd, acb) < 0.0f;
}

//// Return index i of point p[i] farthest from the edge ab, to the left of the edge
//int Physics::PointFarthestFromEdge(Point2D a, Point2D b, Point2D p[], int n)
//{
//    // Create edge vector and vector (counterclockwise) perpendicular to it
//    Vector2D e = b – a, eperp = Vector2D(-e.y, e.x);
//
//    // Track index, ‘distance’ and ‘rightmostness’ of currently best point
//    int bestIndex = -1;
//    float maxVal = -FLT_MAX, rightMostVal = -FLT_MAX;
//
//    // Test all points to find the one farthest from edge ab on the left side
//    for (int i = 1; i < n; i++) {
//        float d = Dot2D(p[i] – a, eperp); // d is proportional to distance along eperp
//        float r = Dot2D(p[i] – a, e);     // r is proportional to distance along e
//        if (d > maxVal || (d == maxVal && r > rightMostVal)) {
//            bestIndex = i;
//            maxVal = d;
//            rightMostVal = r;
//        }
//    }
//    return bestIndex;
//}

int Physics::TestSphereSphere(Sphere a, Sphere b)
{
    // Calculate squared distance between centers

	CVector3f d = CVector3f( a.c.x, a.c.y, a.c.y ) - CVector3f( b.c.x, b.c.y, b.c.z );	// Vector d = a.c – b.c;
	float dist2 = CVector3f::DotProduct( d, d );		// float dist2 = Dot(d, d);

    // Spheres intersect if squared distance is less than squared sum of radii
    float radiusSum = a.r + b.r;
    return dist2 <= radiusSum * radiusSum;
}

// Compute indices to the two most separated points of the (up to) six points
// defining the AABB encompassing the point set. Return these as min and max.
void Physics::MostSeparatedPointsOnAABB(int &min, int &max, Point pt[], int numPts)
{
    // First find most extreme points along principal axes
    int minx = 0, maxx = 0, miny = 0, maxy = 0, minz = 0, maxz = 0;
    for (int i = 1; i < numPts; i++) {
        if (pt[i].x < pt[minx].x) minx = i;
        if (pt[i].x > pt[maxx].x) maxx = i;
        if (pt[i].y < pt[miny].y) miny = i;
        if (pt[i].y > pt[maxy].y) maxy = i;
        if (pt[i].z < pt[minz].z) minz = i;
        if (pt[i].z > pt[maxz].z) maxz = i;
    }

    // Compute the squared distances for the three pairs of points
    //float dist2x = Dot(pt[maxx] – pt[minx], pt[maxx] – pt[minx]);
	float dist2x = CVector3f::DotProduct( CVector3f( pt[maxx].x, pt[maxx].y, pt[maxx].z ) - CVector3f( pt[minx].x, pt[minx].y, pt[minx].z ), 
		                                  CVector3f( pt[maxx].x, pt[maxx].y, pt[maxx].z ) - CVector3f( pt[minx].x, pt[minx].y, pt[minx].z ) );

    //float dist2y = Dot(pt[maxy] – pt[miny], pt[maxy] – pt[miny]);
	float dist2y = CVector3f::DotProduct( CVector3f( pt[maxy].x, pt[maxy].y, pt[maxy].z ) - CVector3f( pt[miny].x, pt[miny].y, pt[miny].z ), 
		                                  CVector3f( pt[maxy].x, pt[maxy].y, pt[maxy].z ) - CVector3f( pt[miny].x, pt[miny].y, pt[miny].z ) );

	//float dist2z = Dot(pt[maxz] – pt[minz], pt[maxz] – pt[minz]);
	float dist2z = CVector3f::DotProduct( CVector3f( pt[maxz].x, pt[maxz].y, pt[maxz].z ) - CVector3f( pt[minz].x, pt[minz].y, pt[minz].z ), 
		                                  CVector3f( pt[maxz].x, pt[maxz].y, pt[maxz].z ) - CVector3f( pt[minz].x, pt[minz].y, pt[minz].z ) );

    // Pick the pair (min,max) of points most distant
    min = minx;
    max = maxx;
    if (dist2y > dist2x && dist2y > dist2z) {
        max = maxy;
        min = miny;
    }
    if (dist2z > dist2x && dist2z > dist2y) {
        max = maxz;
        min = minz;
    }
}

void Physics::SphereFromDistantPoints(Sphere &s, Point pt[], int numPts)
{
    // Find the most separated point pair defining the encompassing AABB
    int min, max;
    MostSeparatedPointsOnAABB(min, max, pt, numPts);

    // Set up sphere to just encompass these two points
    s.c = (pt[min] + pt[max]);			// s.c = (pt[min] + pt[max]) * 0.5f;
	s.c *= 0.5f; 

    s.r = CVector3f::DotProduct(pt[max] - s.c, pt[max] - s.c);	// Dot(pt[max] - s.c, pt[max] - s.c);
    s.r = sqrt(s.r);
}

Physics::Point Physics::ClosestPtPointTriangle( Point p, Point a, Point b, Point c )
{
	bool bIsInsideFaceRegion = false;
	return ClosestPtPointTriangle( p, a, b, c, bIsInsideFaceRegion );
}

Physics::Point Physics::ClosestPtPointTriangle( Point p, Point a, Point b, Point c, bool &bIsInsideFaceRegion )
{
	bIsInsideFaceRegion = false;

	// Check if P in vertex region outside A
	CVector3f ab = b - a;
	CVector3f ac = c - a;
	CVector3f ap = p - a;
	float d1 = CVector3f::DotProduct( ab, ap );
	float d2 = CVector3f::DotProduct( ac, ap );
	if ( (d1 <= 0.0f) && (d2 <= 0.0f) )			// barycentric coordinates (1,0,0)
	{
		return a;
	}

	// Check if P in vertex region outside B
	CVector3f bp = p - b;
	float d3 = CVector3f::DotProduct( ab, bp );
	float d4 = CVector3f::DotProduct( ac, bp );
	if ( (d3 >= 0.0f) && (d4 <= d3) )				// barycentric coordinates (0,1,0)
	{
		return b;
	}

	// Check if P in edge region of AB, if so return projection of P onto AB
	float vc = d1*d4 - d3*d2;
	if ( (vc <= 0.0f) && (d1 >= 0.0f) && (d3 <= 0.0f) )
	{
		float v = d1 / (d1 - d3);
		//return a + v * ab;		// barycentric coordinates (1-v,v,0)
		ab *= v;
		a += ac;
		return a;
	}

	// Check if P in vertex region outside C
	CVector3f cp = p - c;
	float d5 = CVector3f::DotProduct( ab, cp );
	float d6 = CVector3f::DotProduct( ac, cp );
	if ( (d6 >= 0.0f) && (d5 <= d6) )
	{
		return c;		// barycentric coordinates (0,0,1)
	}

	// Check if P in edge region of AC, if so return projection of P onto AC
	float vb = d5*d2 - d1*d6;
	if ( (vb <= 0.0f) && (d2 >= 0.0f) && (d6 <= 0.0f) )
	{
		float w = d2 / ( d2 - d6 );
		//return a + w * ac;	// barycentric coordinates (1-w, 0, w)
		ac *= w;
		a += ac;
		return a;
	}

	// Check if P in edge region of BC, if so return projection of P onto BC
	float va = d3*d6 - d5*d4;
	if ( (va <= 0.0f) && ((d4-d3) >= 0.0f) && ((d5-d6) >= 0.0f) )
	{
		float w = (d4-d3) / ( (d4-d3) + (d5-d6) );

		//return b + w * (c - b);		// barycentric coordinates (0,1-w,w)
		CVector3f tempCBW = c - b;
		tempCBW *= w;
		b + tempCBW;
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

}// Physics::Point Physics::ClosestPtPointTriangle( Point p, Point a, Point b, Point c )



