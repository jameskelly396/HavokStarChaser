// Written by Michael Feeney, Fanshawe College, (c) 2014, mfeeney(at)fanshawec.ca
// For the 'Game Development - Advanced Programming' curriculum:  www.fanshawec.ca/en/GDP1
// It may be distributed under the terms of the MIT License: http://opensource.org/licenses/MIT
// Use this code at your own risk. It is indented only as a learning aid.

#include "CFactoryMediator.h"

#include <math.h>		// for the sqrt()

// Include specific type here (NOT in the header file)
//#include "CBunny.h"
//#include "CNinjaTeapot.h" 

#include <glm/gtc/quaternion.hpp>

// All the brains of the 
#include "CGameObjects/CViperBrain.h"




const std::string BUNNY_PLY_FILENAME = "bun_zipper_res3_onlyXYZ.ply";
const std::string TEAPOT_PLY_FILENAME = "Utah_Teapot_xyz.ply";
const std::string SPHERE_PLY_FILENAME = "sphere_UV_xyz.ply";
const std::string SEAFLOOR2_PLY_FILENAME = "Seafloor2_xyz.ply";
//const std::string GALACTICA_PLY_FILENAME = "Galactica_xyz_nxyz_vertUV.ply";
//const std::string GALACTICA_PLY_FILENAME = "Galactica_FACING_FORWARD_xyz_nxyz_vertUV.ply";
//const std::string GALACTICA_PLY_FILENAME = "Galactica_FACING_and_MOVED_FORWARD_xyz_nxyz_vertUV.ply";
const std::string GALACTICA_PLY_FILENAME = "Galactica_FACING_and_MOVED_FORWARD_xyz_nxyz_vertUV_MERGED_VERTICES.ply";
//const std::string VIPER_PLY_FILENAME = "ViperMKVII.ply";
const std::string VIPER_PLY_FILENAME = "ViperMKVII_FACING_FORWARD_xyz_nyxz_vertUV.ply";
//const std::string SKYBOX_PLY_FILENAME = "SphereUVSmooth.ply";
//const std::string SKYBOX_PLY_FILENAME = "SphereUVSmooth_xyz_nxyz.ply";
const std::string SKYBOX_PLY_FILENAME = "Isoshphere_xyz_nxyz.ply";
const std::string SIX_SIDED_STAR_PLY_FILENAME = "1x1_6_Star_2_Sided.ply";
const std::string SIX_SIDED_CLUSTER_PLY_FILENAME = "1x1_6_Cluster_2_Sided.ply";


const std::string BASIC_GAME_OBJECT = "Basic Rendered Object";


CFactoryMediator::CFactoryMediator()
{

	return;
}

CFactoryMediator::~CFactoryMediator()
{
	// Delete all the objects
	for ( std::map< unsigned int, CGameObject* >::iterator itObj = this->m_mapGameObjects.begin();
		  itObj != this->m_mapGameObjects.end(); itObj++ )
	{
		delete itObj->second;
	}
	this->m_mapGameObjects.clear();		// Bye, bye

	return;
}

// Added, INCOMPLETE
// Params that we can pass:
// name = "Type", sValue = type_to_create	<< this is required
// name = "Position", vec4Value
// name = "Orientation", vec4Value
// name = "Colour", vec4Value
// name = "Scale", fValue
// name = "ModelFile", sValue
// name = "BoundingBox", fValue	(Scale updated to fit max extent into this box)
unsigned int CFactoryMediator::CreateObject( CMessage theCreationParams )
{
	// Get "Type" 

	return 0;
}

bool CFactoryMediator::m_AddObjectToWorld( CGameObject* pGameObject )
{
	if ( pGameObject == 0 )
	{
		return false;
	}
	if ( this->m_mapGameObjects.find( pGameObject->getID() ) != this->m_mapGameObjects.end() )
	{	// object with that ID is already there!
		return false;
	}

	if (pGameObject->m_vecMeshDescriptions[0].bIsParticle)
	{
		pGameObject->bIsVisible = false;
	}

	// We're good, so add it.
	this->m_mapGameObjects[pGameObject->getID()] = pGameObject;
	return true;
}
	
unsigned int CFactoryMediator::CreateObjectByType( const std::string &type )
{
	static CPhysicalProp tempPhysProps;
	static CMeshDesc tempMeshProps;
	//std::vector<CMeshDesc> vec_tempMeshProps;
	//vec_tempMeshProps.push_back(tempMeshProps);
	CGameObject* pTempObject = this->m_CreateObjectByType( type, tempPhysProps, tempMeshProps );
	if ( !this->m_AddObjectToWorld( pTempObject ) )
	{	// Something is wrong
		return 0;
	}
	return pTempObject->getID();
}


unsigned int CFactoryMediator::CreateObjectByType( const std::string &type, const CVector3f &initPosition )
{
	static CPhysicalProp tempPhysProps;
	tempPhysProps.position = initPosition;
	static CMeshDesc tempMeshProps;
	//std::vector<CMeshDesc> vec_tempMeshProps;
	//vec_tempMeshProps.push_back(tempMeshProps);
	CGameObject* pTempObject = this->m_CreateObjectByType( type, tempPhysProps, tempMeshProps );
	if ( !this->m_AddObjectToWorld( pTempObject ) )
	{	// Something is wrong
		return 0;
	}
	return pTempObject->getID();
}

unsigned int CFactoryMediator::CreateObjectByType( const std::string &type, const CPhysicalProp &initPhysProps )
{
	static CMeshDesc tempMeshProps;
	//std::vector<CMeshDesc> vec_tempMeshProps;
	//vec_tempMeshProps.push_back(tempMeshProps);
	CGameObject* pTempObject = this->m_CreateObjectByType( type, initPhysProps, tempMeshProps );
	if ( !this->m_AddObjectToWorld( pTempObject ) )
	{	// Something is wrong
		return 0;
	}
	return pTempObject->getID();
}

unsigned int CFactoryMediator::CreateObjectByType( const std::string &type, const CMeshDesc &initMeshProps )
{
	static CPhysicalProp tempPhysProps;
	//std::vector<CMeshDesc> vec_initMeshProps;
	//vec_initMeshProps.push_back(initMeshProps);
	CGameObject* pTempObject = this->m_CreateObjectByType( type, tempPhysProps, initMeshProps );
	if ( !this->m_AddObjectToWorld( pTempObject ) )
	{	// Something is wrong
		return 0;
	}
	return pTempObject->getID();
}

unsigned int CFactoryMediator::CreateObjectByType( const std::string &type, const CPhysicalProp &initPhysProps, CMeshDesc &initMeshProps )
{
	//std::vector<CMeshDesc> vec_initMeshProps;
	//vec_initMeshProps.push_back(initMeshProps);
	CGameObject* pTempObject = this->m_CreateObjectByType( type, initPhysProps, initMeshProps );
	if ( !this->m_AddObjectToWorld( pTempObject ) )
	{	// Something is wrong
		return 0;
	}
	return pTempObject->getID();
}



// This can only be called internally
// Reutrns 0 if can't find the specific type
CGameObject* CFactoryMediator::m_CreateObjectByType(const std::string &type, CPhysicalProp initPhysProps, CMeshDesc initMeshProps)
{
	// Right now, these are all the same type....
	CGameObject* pTempObject = new CGameObject();

	pTempObject->setPhysicalProperties(initPhysProps);
	pTempObject->commitPhysicalProperties();			// Copy the "new" values to the "last frame"

	/*if ( type == "Sphere UV SkyBox" )
	{
		initMeshProps.bIsSkybox = true;
	}
*/
	//for (int i = 0; i < initMeshProps.size(); i++){
		pTempObject->addMeshDescription(initMeshProps);


		if (initMeshProps.bIsPlayer)
		{
			IBrain* pTempBrain = new CViperBrain();
			pTempBrain->setMediator(this);
			pTempBrain->setGameObject(pTempObject);
			pTempObject->setBrain(pTempBrain);
		}

	//}

	return pTempObject;
}


bool CFactoryMediator::getPhysicalPropertiesByID( unsigned int ID, CPhysicalProp &physProp )
{
	CGameObject* pTheGO = this->m_findObjectByID( ID );
	if ( pTheGO == 0 )
	{
		return false;	// Didn't find it
	}
	// Found it
	pTheGO->getPhysicalProperties( physProp );
	return true;
}

bool CFactoryMediator::setPhysicalPropertiesByID( unsigned int ID, CPhysicalProp &physProp )
{
	CGameObject* pTheGO = this->m_findObjectByID( ID );
	if ( pTheGO == 0 )
	{
		return false;	// Didn't find it
	}
	// Found it
	pTheGO->setPhysicalProperties( physProp );
	return true;
}

bool CFactoryMediator::getRenderingPropertiesByID( unsigned int ID, std::vector<CMeshDesc> &vecMeshDescs )
{
	CGameObject* pTheGO = this->m_findObjectByID( ID );
	if ( pTheGO == 0 )
	{
		return false;	// Didn't find it
	}
	// Found it
	pTheGO->getMeshDescriptions( vecMeshDescs );
	return true;
}

bool CFactoryMediator::addMeshDescription(unsigned int ID, CMeshDesc &MeshDescs)
{
	CGameObject* pTheGO = this->m_findObjectByID(ID);
	if (pTheGO == 0)
	{
		return false;	// Didn't find it
	}
	// Found it
	pTheGO->addMeshDescription(MeshDescs);
	return true;
}

bool CFactoryMediator::setRenderingPropertiesByID(unsigned int ID, std::vector<CMeshDesc> &meshDesc)
{
	CGameObject* pTheGO = this->m_findObjectByID( ID );
	if ( pTheGO == 0 )
	{
		return false;	// Didn't find it
	}
	// Found it
	pTheGO->setMeshDescriptions( meshDesc );
	return true;
}



//bool CFactoryMediator::UpdateObjectPositionByID( unsigned int ID, CVector3f newPosition )
//{
//	CGameObject* pTheGO = this->m_findObjectByID( ID );
//	if ( pTheGO == 0 )
//	{
//		return false;	// Didn't find it
//	}
//	// Found it
//	pTheGO->physicalProperties.position = newPosition;
//	return true;
//}

//// Passing in euler angles (around x,y, and z)
//bool CFactoryMediator::UpdateObjectRotationByID( unsigned int ID, CVector3f newRotation )
//{
//	CGameObject* pTheGO = this->m_findObjectByID( ID );
//	if ( pTheGO == 0 )
//	{
//		return false;	// Didn't find it
//	}
//	// Found it
//	// pTheGO->rotation = newRotation;  // Can't just set the angles as before (they are quaternions)
//
//	//pTheGO->rotation.CreateFromMultipleAngles( newRotation.x, newRotation.y, newRotation.z );
//	
//	// It looks like the constructor of the glm::fquat takes the Euler angles...
//	pTheGO->physicalProperties.orientation = glm::fquat( glm::vec3( newRotation.x, newRotation.y, newRotation.z ) );
//
//	return true;
//}

//// ADDED: November 4, 2014
//bool CFactoryMediator::AdjustObjectRotationByID( unsigned int ID, CVector3f EulerRotationAngleAdjust )
//{
//	CGameObject* pTheGO = this->m_findObjectByID( ID );
//	if ( pTheGO == 0 )
//	{
//		return false;	// Didn't find it
//	}
//
//	//pTheGO->rotation.x += EulerRotationAngleAdjust.x;
//	//pTheGO->rotation.y += EulerRotationAngleAdjust.y;
//	//pTheGO->rotation.x += EulerRotationAngleAdjust.z;
//
//
//	// Found it. 
//	// Unlike Euler angles, we can't just "add" the corresponding angles to the xyz values inside the 
//	//	quaternion. But we can do something like we did with the matrix math 
//
//	// Step 1: make a quaternion that represents the angle we want to rotate
//	glm::fquat rotationAdjust( glm::vec3( EulerRotationAngleAdjust.x, 
//		                                  EulerRotationAngleAdjust.y, 
//										  EulerRotationAngleAdjust.z ) );
//	
//	// Step 2: Multiply this quaternion by the existing quaternion. This "adds" the angle we want.
//	pTheGO->physicalProperties.orientation *= rotationAdjust;
//
//
//	return true;
//}

//// ADDED: November 5, 2014
//bool CFactoryMediator::AdjustObjectRotationSPEEDByID( unsigned int ID, CVector3f EulerRotationSPEEDAngleAdjust )
//{
//	CGameObject* pTheGO = this->m_findObjectByID( ID );
//	if ( pTheGO == 0 )
//	{
//		return false;	// Didn't find it
//	}
//
//	// SAME as the "update the rotation angle", we are updating the rotation speed
//	// Step 1: make a quaternion that represents the angle we want to rotate
//	glm::fquat rotationAdjust( glm::vec3( EulerRotationSPEEDAngleAdjust.x, 
//		                                  EulerRotationSPEEDAngleAdjust.y, 
//										  EulerRotationSPEEDAngleAdjust.z ) );
//	
//	// Step 2: Multiply this quaternion by the existing quaternion. This "adds" the angle we want.
//	pTheGO->physicalProperties.rotationalSpeed *= rotationAdjust;
//
//	return true;
//}

//bool CFactoryMediator::UpdateObjectScaleByID( unsigned int ID, float newScale )
//{
//	CGameObject* pTheGO = this->m_findObjectByID( ID );
//	if ( pTheGO == 0 )
//	{
//		return false;	// Didn't find it
//	}
//	// Found it
//	pTheGO->meshDescription.scale = CVector3f(newScale);
//	return true;
//}

//bool CFactoryMediator::UpdateObjectVelocityByID( unsigned int ID, CVector3f newVelocity )
//{
//	CGameObject* pTheGO = this->m_findObjectByID( ID );
//	if ( pTheGO == 0 )
//	{
//		return false;	// Didn't find it
//	}
//	// Found it
//	pTheGO->physicalProperties.velocity = newVelocity;
//	return true;
//}

//bool CFactoryMediator::GetObjectVelocityByID( unsigned int ID, CVector3f &velocity )
//{
//	CGameObject* pTheGO = this->m_findObjectByID( ID );
//	if ( pTheGO == 0 )
//	{
//		return false;	// Didn't find it
//	}
//	// Found it
//	velocity = pTheGO->physicalProperties.velocity;
//	return true;
//}

//bool CFactoryMediator::UpdatePlyFileByID( unsigned int ID, std::string newPlyFileName )
//{
//	CGameObject* pTheGO = this->m_findObjectByID( ID );
//	if ( pTheGO == 0 )
//	{
//		return false;	// Didn't find it
//	}
//	// Found it
//	pTheGO->meshDescription.modelName = newPlyFileName;
//	return true;
//}

// Added on September 29, 2014
//bool CFactoryMediator::UpdateDebugColourByID( unsigned int ID, CVector4f newColour )
//{
//	CGameObject* pTheGO = this->m_findObjectByID( ID );
//	if ( pTheGO == 0 )
//	{
//		return false;	// Didn't find it
//	}
//	 Found it
//	pTheGO->debugColour = newColour;
//	return true;
//}


//bool CFactoryMediator::SetObjectVisibility( unsigned int ID, bool bNewIsVisible )
//{
//	CGameObject* pTheGO = this->m_findObjectByID( ID );
//	if ( pTheGO == 0 )
//	{
//		return false;	// Didn't find it
//	}
//	// Found it
//	pTheGO->meshDescription.bIsVisible = bNewIsVisible;
//	return true;
//}

//bool CFactoryMediator::SetObjectWireframe( unsigned int ID, bool bNewIsWireframe )
//{
//	CGameObject* pTheGO = this->m_findObjectByID( ID );
//	if ( pTheGO == 0 )
//	{
//		return false;	// Didn't find it
//	}
//	// Found it
//	pTheGO->meshDescription.bIsWireframe = bNewIsWireframe;
//	return true;
//}

// Few options:
// * Make this function "global" i.e. place in a header file that you include
// * Place this in the specifc classes
// * Add it to the CVector3f (it makes sense and CVector3f is already included)
// * Q-n-D: Rename, yo
float getDistanceBetweenPoints_YOLO( CVector3f point1, CVector3f point2 )
{
	float deltaXsquared = ( point2.x - point1.x ) * ( point2.x - point1.x );  // pow()
	float deltaYsquared = ( point2.y - point1.y ) * ( point2.y - point1.y );
	float deltaZsquared = ( point2.z - point1.z ) * ( point2.z - point1.z );

	// There's a number of square root functions. 
	// You many have to include "math.h" or something else. 
	float distance = sqrt( deltaXsquared + deltaYsquared + deltaZsquared );

	return distance;
}

// From IMediator:
// Call this to find objects nearby
std::vector<unsigned int> CFactoryMediator::GetIDOfNearestObjects( CVector3f &location, float radius )
{
	std::vector<unsigned int> vecFoundObjects;

	// TODO: code to find nearest object to be inserted here
	// Loop through all the objects, checking distance: is it closer than radius
	// If so, add to the vecFoundObjects
	for ( std::map< unsigned int, CGameObject* >::iterator itGO = this->m_mapGameObjects.begin();
		  itGO != this->m_mapGameObjects.end(); itGO ++ )
	{
		CPhysicalProp tempPhysProps;
		itGO->second->getPhysicalProperties( tempPhysProps );
		float distance = CVector3f::Distance( CVector3f( tempPhysProps.position.x, 
			                                             tempPhysProps.position.y, 
														 tempPhysProps.position.z ),
											  location );
		if ( distance <= radius )
		{	// Yup, it's "close enough"
			vecFoundObjects.push_back( itGO->second->getID() );
		}
	}

	return vecFoundObjects;
}

// Call this to get location of object
bool CFactoryMediator::getPositionByID( unsigned int ID, CVector3f &position )
{
	// code to find object by ID, then return location
	CGameObject* pObject = this->m_findObjectByID(ID);
	if ( pObject == 0 )
	{
		return false;	// Didn't find object
	}

	CPhysicalProp tempProp;
	pObject->getPhysicalProperties( tempProp );

	position = tempProp.position;

	return true;
}

// Called by the renderer
void CFactoryMediator::getRenderedObjects( std::vector< IGameObjectRender* > &vecMeshesToRender)
{
	this->getRenderedObjects( vecMeshesToRender, IFactory::EVERYTHING_VISIBLE );

	return;
}

void CFactoryMediator::getRenderedObjects( std::vector< IGameObjectRender* > &vecMeshesToRender, enumRenderObjectType types)
{
	// Clear vector
	vecMeshesToRender.clear();

	// Copy all the objects here to external
	for ( std::map< unsigned int, CGameObject* >::iterator itGO = this->m_mapGameObjects.begin();
		  itGO != this->m_mapGameObjects.end(); itGO++ )
	{
		// Are we limiting the types
		switch ( types )
		{
		case IFactory::EVERYTHING_VISIBLE:
			if ( itGO->second->bIsVisible )
			{
				vecMeshesToRender.push_back( itGO->second );
			}
			break;
		case IFactory::ONLY_SKYBOXES:
			if ( (itGO->second->bIsVisible) && (itGO->second->bContainsSkybox() ) )
			{
				vecMeshesToRender.push_back( itGO->second );
			}
			break;
		case IFactory::ONLY_REGULAR_MESH_OBJECTS:
			if ( (itGO->second->bIsVisible) && ( ! itGO->second->bContainsSkybox() ) )
			{
				vecMeshesToRender.push_back( itGO->second );
			}
			break;
		}//switch ( types )
	}// for ( std::map< unsigned int, CGameObject* >::iterator itGO ...
	return;
}


// This is called by the physics updater
void CFactoryMediator::getPhysicsObjects(std::vector< IGameObjectPhysics* > &vecObjectsToUpdate)
{
	// Clear vector
	vecObjectsToUpdate.clear();

	// Copy all the objects here to external
	for ( std::map< unsigned int, CGameObject* >::iterator itGO = this->m_mapGameObjects.begin();
		  itGO != this->m_mapGameObjects.end(); itGO++ )
	{
		if ( itGO->second->getIsIncludedInPhysicsUpdate() )
		{
			vecObjectsToUpdate.push_back( itGO->second );
		}
	}
	return;
}


CGameObject* CFactoryMediator::m_findObjectByID(int ID)
{
	std::map< unsigned int, CGameObject* >::iterator itGO = this->m_mapGameObjects.find( ID );
	if ( itGO != this->m_mapGameObjects.end() )
	{
		return itGO->second;
	}

	// Didn't find it, so return 0
	return 0;
}


bool CFactoryMediator::SendMessageToObject( unsigned int TargetID, unsigned int SenderID, CMessage theMessage )
{
	// Find the target
	CGameObject* pTarget = this->m_findObjectByID( TargetID );
	if ( pTarget == 0 )
	{	// Nope, can't find it. 
		return false;
	}

	// We could do other stuff here, too, maybe.
	
	// At this point, the target has been found
	pTarget->receiveMessage( theMessage, SenderID );


	return true;
}
