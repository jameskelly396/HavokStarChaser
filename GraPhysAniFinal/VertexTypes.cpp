#include "VertexTypes.h"

sVert_PC::sVert_PC()
{
	Position[0] = Position[1] = Position[2] = Position[3] = 0.0f;
	Color[0] = Color[1] = Color[2] = 0.0f;  Color[3] = 1.0f;
}

sVert_PCN::sVert_PCN()
{
	Position[0] = Position[1] = Position[2] = Position[3] = 0.0f;
	Color[0] = Color[1] = Color[2] = 0.0f;  Color[3] = 1.0f;
	Normal[0] = Normal[1] = Normal[2] = 0.0f;  Normal[3] = 1.0f;
}

void sVert_PCN::SetPosition(float x, float y, float z, float w /*= 1.0f*/)
{
	Position[0] = x;
	Position[1] = y;
	Position[2] = z;
	Position[3] = w;
}

void sVert_PCN::SetColor(float r, float g, float b, float a /*= 1.0f*/)
{
	Color[0] = r;
	Color[1] = g;
	Color[2] = b;
	Color[3] = a;
}

void sVert_PCN::SetNormal(float x, float y, float z, float w /*= 1.0f*/)
{
	Normal[0] = x;
	Normal[1] = y;
	Normal[2] = z;
	Normal[3] = w;
}


//CVertex_fXYZ_fRGB::CVertex_fXYZ_fRGB()
//{
//	Position[0] = Position[1] = Position[2] = Position[3] = 0.0f;
//	Color[0] = Color[1] = Color[2] = 0.0f;
//	Color[3] = 1.0f;	// Alpha = 1.0f;
//
//	return;
//}

//CVertex_fXYZ_fRGBA_fNxNyNzW::CVertex_fXYZ_fRGBA_fNxNyNzW()
//{
//	Position[0] = Position[1] = Position[2] = Position[3] = 0.0f;
//	Color[0] = Color[1] = Color[2] = 0.0f;
//	Color[3] = 1.0f;	// Alpha = 1.0f;
//
//	Normal[0] = Normal[1] = Normal[2] = 0.0f;
//	Normal[1] = 1.0f;
//
//	return;
//}
