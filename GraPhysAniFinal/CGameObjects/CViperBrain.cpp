#include "CViperBrain.h"
#include "CGameObject.h"

CViperBrain::CViperBrain()
{
	this->m_pGameObject = 0;
	this->m_pMediator = 0;

	this->m_BrainState = CViperBrain::USER_CONTROLLED;
	this->m_maxSpeed = 10.0f;
	this->m_maxTurnSpeed = 2.0f;		// User turns at 1.0, so this would be pretty fast
	this->m_maxPatrolDistance = 100.0f;
	this->m_minDistanceToWaypoint = 10.0f;

	this->m_maxThrust = 25.0f;

	return;
}


CViperBrain::~CViperBrain()
{
	return;
}

void CViperBrain::setMediator( IMediator* pTheMediator )
{
	this->m_pMediator = pTheMediator;
	return;
}

void CViperBrain::setGameObject( CGameObject* pGameObject )
{
	this->m_pGameObject = pGameObject;
	return;
}

void CViperBrain::Update( float deltaTime )
{
	if ( (this->m_pMediator == 0) || (this->m_pGameObject == 0) )
	{
		return;
	}

	switch ( this->m_BrainState )
	{
	case CViperBrain::ON_PATROL:
		{
			// TODO: Do stuff
			// How close to the waypoint are we?
			CPhysicalProp viperPhysProps;
			this->m_pGameObject->getPhysicalProperties( viperPhysProps );

			float distanceToWaypoint = CVector3f::Distance( viperPhysProps.position, this->m_nextWaypoint );

			if ( distanceToWaypoint < this->m_minDistanceToWaypoint ) 
			{
				// Pick another waypoint
				this->m_nextWaypoint = CVector3f( this->m_getRand( -(this->m_maxPatrolDistance), this->m_maxPatrolDistance ), 
												  this->m_getRand( -(this->m_maxPatrolDistance), this->m_maxPatrolDistance ),
												  this->m_getRand( -(this->m_maxPatrolDistance), this->m_maxPatrolDistance ) );
			}
			else
			{
				// Accel to waypoint
				if ( viperPhysProps.velocity.Magnitude() < this->m_maxSpeed )
				{
					viperPhysProps.directedVelocity.z = ( 25.0f * deltaTime );
				}
				// Point viper in the correct direction
				// Note: If we use something like quaternion lookat

				CVector3f directionXYZ = this->m_nextWaypoint - viperPhysProps.position;
				directionXYZ.Normalize();
				//directionXYZ.x = asin( directionXYZ.x );
				//directionXYZ.y = asin( directionXYZ.y );
				//directionXYZ.z = asin( directionXYZ.z );

				this->m_idealOrientation = glm::fquat( glm::vec3( directionXYZ.x, directionXYZ.y, directionXYZ.z ) );


				glm::fquat adjust = glm::slerp( this->m_idealOrientation, viperPhysProps.orientation, ((this->m_maxTurnSpeed * deltaTime)/100.0f) );

				viperPhysProps.adjOrientationQ( adjust );

				//CVector3f dirEuler;
				//glm::vec3 dirEuler = glm::eulerAngles( viperPhysProps.orientation );

				//glm::vec3 dirIdealEuler = glm::eulerAngles( this->m_idealOrientation );

				//glm::vec3 dirDif = dirEuler - dirIdealEuler;

				//glm::normalize(dirDif);



				//int x = 0;

			}

			this->m_pGameObject->setPhysicalProperties( viperPhysProps );
			}
		break;
	};

	return;
}

// These messages are passed in from the object
void CViperBrain::receiveMessage( CMessage theMessage, unsigned int senderID )
{
	CNameValuePair nvPair;
	if ( theMessage.findNVPairByName( "SetMaxSpeed", nvPair ) )
	{
		this->m_maxSpeed = nvPair.fValue;
		return;
	}
	else if ( theMessage.findNVPairByName( "SetPatrolDistance", nvPair ) )
	{
		this->m_maxPatrolDistance = nvPair.fValue;
		return;
	}
	else if ( theMessage.findNVPairByName( "MinWaypointDistance", nvPair ) )
	{
		this->m_minDistanceToWaypoint = nvPair.fValue;
		return;
	}
	else if ( theMessage.findNVPairByName( "MaxThrust", nvPair ) ) 
	{
		this->m_maxThrust = nvPair.fValue;
		return;
	}
	else if ( theMessage.findNVPairByName( "MaxTurnSpeed", nvPair ) ) 
	{
		this->m_maxTurnSpeed = nvPair.fValue;
		return;
	}	
	else if ( theMessage.findNVPairByName( "StartPatrol", nvPair ) )
	{
		this->m_BrainState = CViperBrain::ON_PATROL;
		// Pick a waypoint
		this->m_nextWaypoint = CVector3f( this->m_getRand( -(this->m_maxPatrolDistance), this->m_maxPatrolDistance ), 
			                              this->m_getRand( -(this->m_maxPatrolDistance), this->m_maxPatrolDistance ),
										  this->m_getRand( -(this->m_maxPatrolDistance), this->m_maxPatrolDistance ) );
		return;

	}
	else if ( theMessage.findNVPairByName( "UnderUserControl", nvPair ) )
	{
		this->m_BrainState = CViperBrain::USER_CONTROLLED;
		return;
	}

	// Depending on state, move viper
	switch ( this->m_BrainState )
	{
	case CViperBrain::USER_CONTROLLED:
		if ( theMessage.findNVPairByName( "SetDirectedVelocity", nvPair ) )
		{
			CPhysicalProp viperProps;
			this->m_pGameObject->getPhysicalProperties( viperProps );
			viperProps.directedVelocity = CVector3f( nvPair.vec4Value.x, nvPair.vec4Value.y, nvPair.vec4Value.z );
			this->m_pGameObject->setPhysicalProperties( viperProps );
		}
		else if ( theMessage.findNVPairByName( "adjOrientationEulerAngles", nvPair ) )
		{
			CPhysicalProp viperProps;
			this->m_pGameObject->getPhysicalProperties( viperProps );
			viperProps.adjOrientationEulerAngles( CVector3f( nvPair.vec4Value.x, nvPair.vec4Value.y, nvPair.vec4Value.z ) );
			this->m_pGameObject->setPhysicalProperties( viperProps );
		}

		break;

	case CViperBrain::ON_PATROL:


		break;
	};


	return;
};


float CViperBrain::m_getRand(float lowRange, float highRange)
{
	if ( lowRange > highRange )
	{
		float temp = lowRange;
		lowRange = highRange;
		highRange = temp;
	}
	float delta = highRange - lowRange;
	delta = ( static_cast<float>( rand() ) / static_cast<float>(RAND_MAX + 1) ) * delta;
	return delta + lowRange;
}


