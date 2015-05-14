#ifndef _CFactoryMediator_HG_
#define _CFactoryMediator_HG_

// Written by Michael Feeney, Fanshawe College, (c) 2014, mfeeney(at)fanshawec.ca
// For the 'Game Development - Advanced Programming' curriculum:  www.fanshawec.ca/en/GDP1
// It may be distributed under the terms of the MIT License: http://opensource.org/licenses/MIT
// Use this code at your own risk. It is indented only as a learning aid.

#include "IFactory.h"
#include "IMediator.h" 
#include "CGameObjects/CGameObject.h"

#include <vector>
#include <map>

class CFactoryMediator : public IFactory, IMediator
{
public:
	CFactoryMediator();
	virtual ~CFactoryMediator();
	
	// *********************************
	// From IFactory:
	virtual unsigned int CreateObjectByType( const std::string &type ); 
	virtual unsigned int CreateObjectByType( const std::string &type, const CVector3f &initPosition );
	virtual unsigned int CreateObjectByType( const std::string &type, const CPhysicalProp &initPhysProps );
	virtual unsigned int CreateObjectByType( const std::string &type, const CMeshDesc &initMeshProps );
	virtual unsigned int CreateObjectByType( const std::string &type, const CPhysicalProp &initPhysProps, CMeshDesc &initMeshProps );

	// Added, INCOMPLETE
	virtual unsigned int CreateObject( CMessage theCreationParams );

	// Called by the renderer
	virtual void getRenderedObjects( std::vector< IGameObjectRender* > &vecMeshesToRender);
	virtual void getRenderedObjects( std::vector< IGameObjectRender* > &vecMeshesToRender, enumRenderObjectType types);
	// This is called by the physics updater
	virtual void getPhysicsObjects( std::vector< IGameObjectPhysics* > &vecObjectsToUpdate );

	virtual bool getPhysicalPropertiesByID( unsigned int ID, CPhysicalProp &physProp );
	virtual bool setPhysicalPropertiesByID( unsigned int ID, CPhysicalProp &physProp );

	virtual bool getRenderingPropertiesByID( unsigned int ID, std::vector<CMeshDesc> &vecMeshDescs );
	virtual bool addMeshDescription(unsigned int ID, CMeshDesc &MeshDescs);
	virtual bool setRenderingPropertiesByID(unsigned int ID, std::vector<CMeshDesc> &meshDesc);
	//virtual bool setRenderingPropertiesByID( unsigned int ID, CMeshDesc &meshDesc );

	// From IMediator (i.e. other objects can call this)
	virtual bool getPositionByID( unsigned int ID, CVector3f &position );


	//virtual bool UpdateObjectPositionByID( unsigned int ID, CVector3f newPosition );
	//virtual bool UpdateObjectScaleByID( unsigned int ID, float newScale );
	//virtual bool UpdateObjectVelocityByID( unsigned int ID, CVector3f newVelocity );
	//virtual bool GetObjectVelocityByID( unsigned int ID, CVector3f &velocity );
	//virtual bool UpdateObjectRotationByID( unsigned int ID, CVector3f newRotation );
	//// ADDED: November 4, 2014
	//virtual bool AdjustObjectRotationByID( unsigned int ID, CVector3f EulerRotationAngleAdjust );
	//// ADDED: November 5, 2014
	//virtual bool AdjustObjectRotationSPEEDByID( unsigned int ID, CVector3f EulerRotationSPEEDAngleAdjust );
	//virtual bool UpdatePlyFileByID( unsigned int ID, std::string newPlyFileName );
	//virtual bool SetObjectVisibility( unsigned int ID, bool bNewIsVisible );
	//virtual bool SetObjectWireframe( unsigned int ID, bool bNewIsWireframe );
	//// *********************************
	////virtual bool UpdateColourByID( unsigned int ID, CVector3f newColour );
	//// Setting the a (or w) param to 0.0f will turn "off" the debug colour
	//virtual bool UpdateDebugColourByID( unsigned int ID, CVector4f newColour );

	// From IMediator:
	// Call this to find objects nearby
	virtual std::vector<unsigned int> GetIDOfNearestObjects( CVector3f &location, float radius );
	// Used for objects to call each other
	virtual bool SendMessageToObject( unsigned int TargetID, unsigned int SenderID, CMessage theMessage );

	// *********************************
private:
	// Helper function that can only be called internally
	CGameObject* CFactoryMediator::m_CreateObjectByType(const std::string &type, CPhysicalProp initPhysProps, CMeshDesc initMeshProps);
	bool m_AddObjectToWorld( CGameObject* pGameObject );

	// Contains the objects...
	//std::vector< CGameObject* > m_GameObjects;
	std::map< unsigned int, CGameObject* > m_mapGameObjects;

	// Called internally to locate objects. Returns zero (0) if didn't find anything
	CGameObject* m_findObjectByID(int ID);
};

#endif
