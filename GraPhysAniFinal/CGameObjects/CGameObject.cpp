#include "CGameObject.h"

// Initial value of the first CGameObject ID
// (i.e. the unique IDs of the objects will start at this number)
// Note that using an unsigned int, and assuming that it's 32 bits
//	means that it may potentially wrap around at some point. 
// (but you'd have to create 4 billion objects, first)
static const unsigned int FIRSTGAMEOBJECTID = 1000;
unsigned int CGameObject::m_nextID = FIRSTGAMEOBJECTID;



// Called by the constructors
void CGameObject::m_Init(void)
{
	// Assign ID here
	this->m_ID = this->m_nextID;   
	this->m_nextID++;	// Increment

	this->bIsVisible = true;

	this->m_pBrain = 0;

	//this->specularShininess = 1.0f;		// "Meh" shiny
	//this->specularStrength = 1.0f;		// Not shiny
	//this->bIsImposter = false;
	//this->bIsParticle = false;
	//this->bIsSkybox = false;			
	//this->bUseDebugColour = false;

	//this->bIncludeInPhysicsUpdate = true;

	return;
}

CGameObject::CGameObject()
{
	this->m_Init();

	this->m_pMediator = 0;	

	this->m_bContainsImposters = false;
	this->m_bContainsSkyBox = false;

	return;
}

bool CGameObject::getIsIncludedInPhysicsUpdate(void)
{
	//return this->m_physicalProperties_lastFrame.bIncludeInPhysicsUpdate;
	return this->m_physicalProperties.bIncludeInPhysicsUpdate;
}

void CGameObject::getPhysicalProperties(CPhysicalProp &physProp)
{
	// Copy return physical properties
	//physProp = this->m_physicalProperties_lastFrame;
	physProp = this->m_physicalProperties;
	return;
}

void CGameObject::setPhysicalProperties(const CPhysicalProp &physProp)
{
	// TODO: See how these properties can be combined with the existing ones. setMeshDescriptions std::vector<CMeshDesc>
	this->m_physicalProperties = physProp;
	return;
}

void CGameObject::setMeshDescriptions(const std::vector<CMeshDesc> &meshDesc)
{
	this->m_vecMeshDescriptions = meshDesc;
	return;
}

// From IGameObjectPhysics
void CGameObject::commitPhysicalProperties(void)					
{
	// The physics update is done (i.e. all objects have been updated), so commit the changes.
	//this->m_physicalProperties_lastFrame = this->m_physicalProperties;
	return;
}

void CGameObject::addMeshDescription(const CMeshDesc& meshDesc)
{
	if ( meshDesc.bIsImposter ) { this->m_bContainsImposters = true; }
	if ( meshDesc.bIsSkybox )	{ this->m_bContainsSkyBox = true; }

	this->m_vecMeshDescriptions.push_back( meshDesc );
	return;
}

bool CGameObject::bContainsSkybox(void)
{
	return this->m_bContainsSkyBox;
}

bool CGameObject::bContainsImposters(void)
{
	return this->m_bContainsImposters;
}

// This adds any meshes to render onto this vector (called by renderer)
void CGameObject::getMeshDescriptions( std::vector< CMeshDesc > &vecMeshDescs )
{
	for ( std::vector<CMeshDesc>::iterator itMesh = this->m_vecMeshDescriptions.begin();
		  itMesh != this->m_vecMeshDescriptions.end(); itMesh++ )
	{
		CMeshDesc finalMeshDesc = *itMesh;
		// Adding
		//finalMeshDesc.relPosition += this->m_physicalProperties_lastFrame.position;
		if (itMesh == this->m_vecMeshDescriptions.begin())
		{

			finalMeshDesc.relPosition += this->m_physicalProperties.position;
			// Multiplying because it's a quaternion, yo
			//finalMeshDesc.orientation *= this->m_physicalProperties_lastFrame.orientation;
			finalMeshDesc.orientation *= this->m_physicalProperties.orientation;
		}
		finalMeshDesc.matrix = this->m_physicalProperties.matrix;

		vecMeshDescs.push_back( finalMeshDesc );
	}
	return;
}


void CGameObject::setMediator( IMediator* pMediator )
{
	// The mediator should only be set at construction, so if something
	//	tries to set it again, we prevent it
	if ( this->m_pMediator == 0 )
	{	// Hasn't been set, so assume creator is calling it
		this->m_pMediator = pMediator;
	}
	return;
}

void CGameObject::setBrain( IBrain* pBrain )
{
	this->m_pBrain = pBrain;
	return;
}

CGameObject::~CGameObject()
{
	return;
}


void CGameObject::Update( float deltaTime )
{
	// Insert behaviour code in here
	if ( this->m_pBrain != 0 )
	{
		this->m_pBrain->Update( deltaTime );
	}

	return;
}

unsigned int CGameObject::getID(void)
{
	return this->m_ID;
}

void CGameObject::receiveMessage( CMessage theMessage, unsigned int senderID )
{
	if ( this->m_pBrain != 0 )
	{
		this->m_pBrain->receiveMessage( theMessage, senderID );
	}

	// Some of the messages we "understand"
	//	vecCommand.push_back( CNameValuePair( "ChangeDirection", "X" ) );
	//	vecCommand.push_back( CNameValuePair( "NewXVel", 0.1f ) );


	// Decode message
	// Note: some of this code is sort of "dangerous" as it just assumes that the 
	//	messages are sent in the correct format. You could do some additional checking
	//	for that (is the message the right lenght, etc.?), but that also takes some
	//	time (and makes the code more involved, too). 
	CNameValuePair firstNVPair;

	//if ( theMessage.findNVPairByName( "ChangeDirection", firstNVPair ) )
	//{	// It's "Change direction", but which?
	//	if ( firstNVPair.sValue == "X" )
	//	{	// 2nd NV pair has velocity
	//		this->velocity.x = theMessage.vecNVPairs.at(1).fValue;
	//	}
	//	if ( firstNVPair.sValue == "Y" )
	//	{
	//		this->velocity.y = theMessage.vecNVPairs.at(1).fValue;
	//	}
	//	if ( firstNVPair.sValue == "Z" )
	//	{
	//		this->velocity.z = theMessage.vecNVPairs.at(1).fValue;
	//	}
	//}
	//else if ( theMessage.findNVPairByName( "StopMoving", firstNVPair ) )
	//{
	//	this->velocity = CVector3f(0.0f, 0.0f, 0.0f);
	//}

	return;
}

// These will ADD to whatever is on the vector
void CGameObject::getDebugMeshObjects( std::vector< CMeshDesc > &vecDebugMeshes, IGameObjectRender::enumDebugObjectType debugObjectType )
{
	// TODO: Figure out what happens when there isn't a mesh attachte to this object...

	std::vector<CMeshDesc> vecMeshes;
	this->getMeshDescriptions( vecMeshes );

	switch ( debugObjectType )
	{
	case IGameObjectRender::AABB_CENTRED:
		break;
	case IGameObjectRender::BOUNDING_SPHERE:
		break;
	case IGameObjectRender::ORIENTATION_VECTORS:
		break;
	case IGameObjectRender::ORIENTED_BOUNDING_BOX_CENTRED:
		break;
	};
	return;
}

