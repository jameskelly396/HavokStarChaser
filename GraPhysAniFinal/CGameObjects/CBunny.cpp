#include "CBunny.h"
#include <math.h>		// sqrt()

CBunny::CBunny()
{
	// Set state to something
	this->m_CurrentState = CBunny::IS_DOING_NOTHING;
	this->m_ID_of_ObjectToFollow = 0;	// 0 isn't a valid object
	return;
}

CBunny::~CBunny()
{
	return;
}

float getDistanceBetweenPoints( CVector3f point1, CVector3f point2 )
{
	float deltaXsquared = ( point2.x - point1.x ) * ( point2.x - point1.x );
	float deltaYsquared = ( point2.y - point1.y ) * ( point2.y - point1.y );
	float deltaZsquared = ( point2.z - point1.z ) * ( point2.z - point1.z );

	// There's a number of square root functions. 
	// You many have to include "math.h" or something else. 
	float distance = sqrt( deltaXsquared + deltaYsquared + deltaZsquared );

	return distance;
}

// Moves this object towards the target by "increment" amount
void CBunny::m_followAtSpeed( CVector3f target, float increment )
{
	if ( this->position.x < this->m_closestObject.x ) { this->position.x += increment;	}
	if ( this->position.x > this->m_closestObject.x ) {	this->position.x -= increment;	}
	
	if ( this->position.y < this->m_closestObject.y ) { this->position.y += increment;	}
	if ( this->position.y > this->m_closestObject.y ) {	this->position.y -= increment;	}

	if ( this->position.z < this->m_closestObject.z ) { this->position.z += increment;	}
	if ( this->position.z > this->m_closestObject.z ) {	this->position.z -= increment;	}
	return;
}

void CBunny::Update( float deltaTime )
{
	// Do specific stuff, and optionally call the base class, perhaps
	// Like following things...
	switch ( this->m_CurrentState )
	{
	case CBunny::IS_FOLLOWING_CLOSEST_OBJECT:		// Only ints or enum (which is an int... Shhh, don't tell anyone)
		{	// **** STARTOF: ADDED to get around scope operator declaration rules *****
			// Find the closest object
			std::vector< unsigned int > vecIDs;
			vecIDs = this->m_pMediator->GetIDOfNearestObjects( this->position, 20.0f ); 
			// Did we find anything close? 
			if ( !vecIDs.empty() )
			{	// Yup, we did, so pick the first
				// (m_closestObject is a CVector3f)
				this->m_pMediator->GetPositionByID( vecIDs[0], this->m_closestObject );
				
				// Do the "following" stuff.
				float distance = getDistanceBetweenPoints( this->position, this->m_closestObject );
				// Are we close enough
				if ( distance >= this->m_followClosestDistance )
				{	// Move closer
					this->m_followAtSpeed( this->m_closestObject, this->m_followSpeed * deltaTime );
				} // if ( distance >= this->m_followClosestDistance )
			}	// if ( !vecIDs.empty() )
		}	// **** ENDOF: ADDED to get around scope operator declaration rules *****
		break;

	case CBunny::IS_FOLLOWING_SPECIFIC_OBJECT:
		{
			// Get the position of the object we are supposed to follow...
			this->m_pMediator->GetPositionByID( this->m_ID_of_ObjectToFollow, this->m_closestObject );
				
			// Do the "following" stuff.
			float distance = getDistanceBetweenPoints( this->position, this->m_closestObject );
			// Are we close enough
			if ( distance >= this->m_followClosestDistance )
			{	// Move closer
				this->m_followAtSpeed( this->m_closestObject, this->m_followSpeed * deltaTime );
			} // if ( distance >= this->m_followClosestDistance )
		}
		break;
	};

	CGameObject::Update( deltaTime );

	return;
}

void CBunny::receiveMessage( CMessage theMessage, unsigned int senderID )
{
	// Do stuff that's specific to this type of object...
	
	CNameValuePair firstNVPair;

	if ( theMessage.findNVPairByName( "FollowClosest", firstNVPair ) )
	{	
		// ___      _  _                ___  _                    _   
		//| __|___ | || | ___ __ __ __ / __|| | ___  ___ ___  ___| |_ 
		//| _|/ _ \| || |/ _ \\ V  V /| (__ | |/ _ \(_-</ -_)(_-<|  _|
		//|_| \___/|_||_|\___/ \_/\_/  \___||_|\___//__/\___|/__/ \__|
		//                                                            
		//vec[0].name = "FollowClosest"   
		//vec[1].fValue = 0.1f  			<-- closest distance
		//vec[2].fValue = 0.1f				<-- speed we'll go

		// We'll save this inforamtion, and also query the mediator about stuff...
		this->m_followClosestDistance = theMessage.vecNVPairs[1].fValue;
		this->m_followSpeed = theMessage.vecNVPairs[2].fValue;
		// Find the closest object
		std::vector< unsigned int > vecIDs;
		vecIDs = this->m_pMediator->GetIDOfNearestObjects( this->position, 20.0f ); 
		// Did we find anything close? 
		if ( !vecIDs.empty() )
		{	// Yup, we did, so pick the first
			// (m_closestObject is a CVector3f)
			this->m_pMediator->GetPositionByID( vecIDs[0], this->m_closestObject );
			// Update the state of this object
			this->m_CurrentState = CBunny::IS_FOLLOWING_CLOSEST_OBJECT;
		}
	}
	else if ( theMessage.findNVPairByName( "FollowObject", firstNVPair ) )
	{
		// ___      _  _                ___   _      _           _   
		//| __|___ | || | ___ __ __ __ / _ \ | |__  (_) ___  __ | |_ 
		//| _|/ _ \| || |/ _ \\ V  V /| (_) || '_ \ | |/ -_)/ _||  _|
		//|_| \___/|_||_|\___/ \_/\_/  \___/ |_.__/_/ |\___|\__| \__|
		//                                        |__/               
		//vec[0].name = "FollowClosest"   
		//vec[0].iValue = ID of object to follow
		//vec[1].fValue = 0.1f  			<-- closest distance
		//vec[2].fValue = 0.1f				<-- speed we'll go
		// We'll save this inforamtion, and also query the mediator about stuff...
		this->m_followClosestDistance = theMessage.vecNVPairs[1].fValue;
		this->m_followSpeed = theMessage.vecNVPairs[2].fValue;
		this->m_ID_of_ObjectToFollow = static_cast<unsigned int>( theMessage.vecNVPairs[0].iValue );
		this->m_CurrentState = CBunny::IS_FOLLOWING_SPECIFIC_OBJECT;
		// We'll wait for the next update to start following this object		
	}



	// Calls the base class (or in Java, the super class)
	CGameObject::receiveMessage( theMessage, senderID );

	return;
}
