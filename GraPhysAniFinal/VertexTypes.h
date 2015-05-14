#ifndef _VertexTypes_HG_
#define _VertexTypes_HG_

// These are some of the vertex types me might want. 
// Note that this sort of thing, from the original code, 
//	is from "old school" C definition. 
// I don't remember the history behind it, but you needed
//	to define a "type definition" (typedef) of the struct
//	from another struct; you couldn't define it directly. 
// 
//	typedef struct Vertex
//	{
//	  float Position[4];
//	  float Color[4];
//	} Vertex;


//struct CVertex_fXYZ_fRGB
//{
//	CVertex_fXYZ_fRGB();	// Yes, structs can have constructors. It's true.
//	float Position[4];
//	float Color[4];
//};

// CAN'T DO THIS SORT OF (SENSIBLE) THING UNTIL OpenGL 4.3...
//struct CVertex_fXYZ_fRGBA_fNxNyNzW
//{
//	CVertex_fXYZ_fRGBA_fNxNyNzW();	
//	float Position[4];
//	float Color[4];
//	float Normal[4];
//};

struct sVert_PC
{
	sVert_PC();
	float Position[4];
	float Color[4];
};

struct sVert_PCN
{
	sVert_PCN();
	float Position[4];
	float Color[4];
	float Normal[4];

	void SetPosition(float x, float y, float z, float w = 1.0f);
	void SetColor(float r, float g, float b, float a = 1.0f);
	void SetNormal(float x, float y, float z, float w = 1.0f);
};

struct CVertex_fXYZW
{
	CVertex_fXYZW() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}; 
	float x, y, z, w;
};


#endif
