#ifndef _IFactory_HG_
#define _IFactory_HG_

//#include "CGameObjects/CGameObject.h"
#include "CGameObjects/IGameObjectRender.h"
#include "CGameObjects/IGameObjectPhysics.h"
#include "CMessage.h"

#include <string>
#include <vector>	

class IFactory		// Things that can create object see this
{
public:
	virtual ~IFactory() {};		// implement a desructor for this
	// Returns a "handle" (aka an ID) of the object (NOT a pointer, though)
	virtual unsigned int CreateObjectByType( const std::string &type ) = 0; 
	virtual unsigned int CreateObjectByType( const std::string &type, const CVector3f &initPosition ) = 0;
	virtual unsigned int CreateObjectByType( const std::string &type, const CPhysicalProp &initPhysProps ) = 0;
	virtual unsigned int CreateObjectByType( const std::string &type, const CMeshDesc &initMeshProps ) = 0;
	virtual unsigned int CreateObjectByType( const std::string &type, const CPhysicalProp &initPhysProps, CMeshDesc &initMeshProps ) = 0;

	virtual unsigned int CreateObject( CMessage theCreationParams ) = 0;

	//virtual bool getPhysicalPropertiesByID( unsigned int ID, CPhysicalProp &physProp ) = 0;
	virtual bool setPhysicalPropertiesByID( unsigned int ID, CPhysicalProp &physProp ) = 0;
	virtual bool addMeshDescription(unsigned int ID, CMeshDesc &MeshDescs) = 0;
	virtual bool setRenderingPropertiesByID(unsigned int ID, std::vector<CMeshDesc> &meshDesc) = 0;

	//virtual bool getRenderingPropertiesByID( unsigned int ID, CMeshDesc &meshDesc ) = 0;
	//virtual bool setRenderingPropertiesByID( unsigned int ID, CMeshDesc &meshDesc ) = 0;

	//
//	virtual bool UpdateObjectPositionByID( unsigned int ID, CVector3f newPosition ) = 0;
//	virtual bool UpdateObjectScaleByID( unsigned int ID, float newScale ) = 0;
//	virtual bool GetObjectVelocityByID( unsigned int ID, CVector3f &velocity ) = 0;
//	virtual bool UpdateObjectVelocityByID( unsigned int ID, CVector3f newVelocity ) = 0;
//	virtual bool UpdateObjectRotationByID( unsigned int ID, CVector3f newRotation ) = 0;
	// ADDED: November 4, 2014
//	virtual bool AdjustObjectRotationByID( unsigned int ID, CVector3f EulerRotationAngleAdjust ) = 0;
	// ADDED: November 5, 2014
//	virtual bool AdjustObjectRotationSPEEDByID( unsigned int ID, CVector3f EulerRotationSPEEDAngleAdjust ) = 0;
//	virtual bool UpdatePlyFileByID( unsigned int ID, std::string newPlyFileName ) = 0;
	// ADDED on September 29, 2014
	//virtual bool UpdateColourByID( unsigned int ID, CVector3f newColour ) = 0;
	// Setting the a (or w) param to 0.0f will turn "off" the debug colour
//	virtual bool UpdateDebugColourByID( unsigned int ID, CVector4f newColour ) = 0;
//	virtual bool SetObjectVisibility( unsigned int ID, bool bNewIsVisible ) = 0;
//	virtual bool SetObjectWireframe( unsigned int ID, bool bNewIsWireframe ) = 0;
	enum enumRenderObjectType
	{
		EVERYTHING_VISIBLE = 5000,	// So it won't start at 0
		//EVERYTHING,
		ONLY_REGULAR_MESH_OBJECTS,
		ONLY_SKYBOXES
	};
	// Called by the renderer
	virtual void getRenderedObjects( std::vector< IGameObjectRender* > &vecMeshesToRender) = 0;
	virtual void getRenderedObjects( std::vector< IGameObjectRender* > &vecMeshesToRender, enumRenderObjectType types) = 0;
	// This is called by the physics updater
	virtual void getPhysicsObjects( std::vector< IGameObjectPhysics* > &vecObjectsToUpdate ) = 0;
};

#endif
