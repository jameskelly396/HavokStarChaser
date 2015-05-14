#ifndef _CMeshDesc_HG_
#define _CMeshDesc_HG_

// Information for the game object and renderer
// Basic stuff to determine which mesh to draw and where

#include "../CCommonTypes/CVector3f.h"
#include "../CCommonTypes/CVector4f.h"
#include <glm/gtc/quaternion.hpp>	
#include <string>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp> // glm::mat4

class CMeshDesc
{
public:
	CMeshDesc();
	CMeshDesc(std::string modelName_);
	~CMeshDesc() {};

	glm::mat4 matrix;

	CVector3f scale;
	CVector3f relPosition;
	glm::fquat orientation;	
	CVector3f centre;
	float tranparency;

	bool bIsWireframe;
	bool bIsTransparent;
	bool bHasTexture;

	bool bUseDebugColour;		// If true, the debugColour is used
	bool bIsParticle;
	bool bIsImposter;		// An "imposter" is an object that will always "face" the camera. Usually a 2D quad
	bool bIsSkybox;
	bool bIsPlayer;
	bool bIsHavok;
	unsigned int blend;
	unsigned int firstTex;

	CVector4f debugColour;	// Default is Bright Neon Pink. Really.
	// Here's the deal: Instead of including the mesh name in this, we include
	//	a "debug mesh type". That way we can check this and render the actual model
	//  we're using as a "debug" model
	// The assumptions are based on the enum
	enum enumDebugModelType
	{
		AABB_UNIT_SIZED_CENTRED = 50000,		// So won't start at 0
		UNIT_SPHERE_CENTRED,
		ORIENTATION_VECTORS_Z_ALIGNED_ARROWS	// Model is a unit length arrow, aligned along +ve z axis
	};
	enumDebugModelType debugObjectType;
	
	// Specular component (Shininess)
	float specularShininess;		// May want to make this a CVector3f, later (but need to update shader)
	float specularStrength;

	std::string textureName_0;
	std::string textureName_1;
	// The file or friendly name
	std::string ObjectType;

	// The file or friendly name
	std::string modelName;
	// An alternative integer ID (faster?)
	unsigned int modelID;

private:
	void m_Init(void);

};

#endif
