#ifndef _IGameObjectRender_HG_
#define _IGameObjectRender_HG_

#include "../CRender/CMeshDesc.h"
#include <vector>

class IGameObjectRender
{
public:
	virtual ~IGameObjectRender() {};

	// In case the object has more than one mesh...
	virtual void getMeshDescriptions( std::vector< CMeshDesc > &vecMeshDescs ) = 0;	
	virtual void addMeshDescription(const CMeshDesc& meshDesc) = 0;
	virtual void setMeshDescriptions(const std::vector<CMeshDesc> &meshDesc) = 0;

	// For faster access (than getting entire CMeshDesc)
	virtual bool bContainsSkybox(void) = 0;
	virtual bool bContainsImposters(void) = 0;

	// These will ADD to whatever is on the vector
	enum enumDebugObjectType 
	{
		AABB_CENTRED = 60000,	// So won't start at 0
		ORIENTED_BOUNDING_BOX_CENTRED,
		BOUNDING_SPHERE, 
		ORIENTATION_VECTORS
	};
	virtual void getDebugMeshObjects( std::vector< CMeshDesc > &vecDebugMeshes, enumDebugObjectType debugObjectType ) = 0;
};

#endif

