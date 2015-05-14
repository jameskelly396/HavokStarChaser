#ifndef _CGameObject_HG_
#define _CGameObject_HG_

#include <string>
#include "../CCommonTypes/CVector3f.h"
#include "../IMediator.h"		// to allow communication with other objects

//#include "CQuaternion.h" 
#include <glm/gtc/quaternion.hpp>	// I'd suggest that you use this one

#include "../CRender/CMeshDesc.h"
#include "../CPhysics/CPhysicalProp.h"

#include "IGameObjectPhysics.h"
#include "IGameObjectRender.h"

#include "IBrain.h"

class CGameObject : public IGameObjectPhysics, public IGameObjectRender
{
public:
	CGameObject();		// Default constructor 
	virtual ~CGameObject();  

	// If false, then isn't rendered (could still have mesh stuff, though)
	bool bIsVisible;		

	// Only called by the IFactory object
	void setMediator( IMediator* pMediator );
	void setBrain( IBrain* pBrain );

	// Used by the Mediator to send messages
	virtual void receiveMessage( CMessage theMessage, unsigned int senderID );

	unsigned int getID(void);


// *** From IGameObjectPhysics ***
	// Quicker than asking for the entire CPhysicalProp structure
	virtual bool getIsIncludedInPhysicsUpdate(void);						// From IGameObjectPhysics
	// Reutrns the "last frame" properties (the stable ones)
	virtual void getPhysicalProperties(CPhysicalProp &physProp);			// From IGameObjectPhysics
	// Forces copy of 
	virtual void setPhysicalProperties(const CPhysicalProp &physProp);		// From IGameObjectPhysics
	// Called at the end of the physics update. 
	virtual void commitPhysicalProperties(void);							// From IGameObjectPhysics

	virtual void Update( float deltaTime );									// From IGameObjectPhysics
// *******************************


// *** From IGameObjectRender ***	
	virtual void getMeshDescriptions( std::vector< CMeshDesc > &vecMeshDescs );	// From IGameObjectRedner

	virtual void addMeshDescription(const CMeshDesc& meshDesc);
	virtual void setMeshDescriptions(const std::vector<CMeshDesc> &meshDesc);
	// For faster access (than getting entire CMeshDesc)
	virtual bool bContainsSkybox(void);
	virtual bool bContainsImposters(void);

	// These will ADD to whatever is on the vector
	virtual void getDebugMeshObjects( std::vector< CMeshDesc > &vecDebugMeshes, enumDebugObjectType debugObjectType ); 
// ******************************
	std::vector<CMeshDesc> m_vecMeshDescriptions;
	CPhysicalProp	m_physicalProperties;					// Updated by set()

protected:
	void m_Init(void);	// Called by the constructor. Set's everything to default state

	// These aren't pointer because they aren't that big
	// (i.e. even the rare objects that don't have these are: a) rare, b) not very many of them)
	
	//CPhysicalProp	m_physicalProperties_lastFrame;			// Used by get()

	// The object might have more than one mesh
	
	bool m_bContainsSkyBox;
	bool m_bContainsImposters;

	// Used to communicate with other objects
	IMediator* m_pMediator;

	IBrain* m_pBrain;

	// Used to figure out what "state" the object is in
	std::string m_State;

	unsigned int m_ID;	// Unique ID value
	// This will "pick" the "next" ID when object is created
	static unsigned int m_nextID;
};

#endif
