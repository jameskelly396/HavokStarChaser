#ifndef _IMediator_HG_
#define _IMediator_HG_

#include "CCommonTypes/CVector3f.h"
#include "CMessage.h"	
#include <vector>
#include "CPhysics/CPhysicalProp.h"
#include "CRender/CMeshDesc.h"

// Interface ("pure virtual" ---  virtual, also "= 0"
class IMediator		// BUNNIES (game objects) "see" this interface
{
public:
	virtual ~IMediator() {};		// need a destructor on interfaces

	// Call this to find objects nearby
	// Returns an empty (zero size) vector if there's nothing in that sphere
	virtual std::vector<unsigned int> GetIDOfNearestObjects( CVector3f &location, float radius ) = 0;

	// Call this to get location of object
	virtual bool getPhysicalPropertiesByID( unsigned int ID, CPhysicalProp &physProp ) = 0;
	//virtual bool getPositionByID( unsigned int ID, CVector3f &position ) = 0;
	virtual bool getRenderingPropertiesByID( unsigned int ID, std::vector<CMeshDesc> &vecMeshDescs ) = 0;

	// Gets a location based on the orientation of the object
	//virtual bool getRelativePositionByID( unsigned int ID, CVector3f &position, CVector3f &relativePosition ) = 0; 

	// Allows objets to communicate with each other
	// Note that we could also do things like have a special TargetID to do special messages, like:
	// TargetID = 0 --> Send to mediator itself
	// TargetID = WHATEVER --> Send a message to everyone (you could set the identifer ID to start
	//                         at some value other than 1, say 1000 (i.e. the first object to be 
	//                         created woudl have ID = 1000; we've done that here: see CGameObject
	//                         constructor cpp code to see the starting value). Any SenderID below 
	//                         that would indicate a "special" message. 
	virtual bool SendMessageToObject( unsigned int TargetID, unsigned int SenderID, CMessage theMessage ) = 0;

};

#endif
