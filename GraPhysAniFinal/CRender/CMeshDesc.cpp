#include "CMeshDesc.h"

CMeshDesc::CMeshDesc()
{
	this->m_Init();
	return;
}

CMeshDesc::CMeshDesc(std::string modelName_)
{
	this->m_Init();
	this->modelName = modelName_;
	return;
}

void CMeshDesc::m_Init(void)
{
	this->scale = CVector3f(1.0f);
	this->modelID = 0;
	this->debugColour = CVector4f( 0.82f, 1.00f, 0.60f, 1.0f );	// F433FF	Bright Neon Pink
	this->tranparency = 1.0f;
	this->bIsWireframe = false;
	this->bIsTransparent = false;
	this->bHasTexture = false;

	this->specularShininess = 1.0f;		// "Meh" shiny
	this->specularStrength = 1.0f;		// Not shiny

	this->bUseDebugColour = false;		// If true, the debugColour is used
	this->bIsParticle = false;
	this->bIsImposter = false;		// An "imposter" is an object that will always "face" the camera. Usually a 2D quad
	this->bIsSkybox = false;  
	this->bIsPlayer = false;
	this->bIsHavok = true;
	this->blend = 0;
	this->firstTex = 0;
	this->ObjectType = "";
	return;
}



