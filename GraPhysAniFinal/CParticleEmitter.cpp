#include "CParticleEmitter.h"

template <class T>
T getRand(T lowRange, T highRange)
{
	if (lowRange > highRange)
	{
		T temp = lowRange;
		lowRange = highRange;
		highRange = temp;
	}
	T delta = highRange - lowRange;
	delta = (static_cast<T>(rand()) / static_cast<T>(RAND_MAX)) * delta;
	return delta + lowRange;
}


CParticleEmitter::CParticleEmitter()
{
	return;
}

CParticleEmitter::~CParticleEmitter()
{
	return;
}

void CParticleEmitter::SetAcceleration(CVector3f newAcceleration)
{
	this->m_acceleration = newAcceleration;
	return;
}


void CParticleEmitter::SetLocation(CVector3f newLocation)
{
	// Emitter's location (not the particles)
	this->m_location = newLocation;
	return;
}

void CParticleEmitter::SetRespawn(bool bNewRespawn)
{
	this->m_bRespawn = bNewRespawn;
	return;
}

void CParticleEmitter::GenerateParticles(
	int numParticles, CVector3f initVelocity,
	float maxDistanceFromSouce,
	float startingAge,
	bool bRespawn)
{
	this->m_startingAge = startingAge;
	this->m_bRespawn = bRespawn;
	this->m_maxDistanceFromSouce = maxDistanceFromSouce;
	this->m_initVelocity = initVelocity;

	this->m_myParticles.clear();	// Delete old particles
	// Allocate space for the number of particles (may have already done this)
	this->m_myParticles.reserve(numParticles);

	for (int count = 0; count != numParticles; count++)
	{
		CParticle tempParticle;
		this->m_CreateParticle(tempParticle);
		tempParticle.age = getRand(this->m_startingAge / 2.0f,
			this->m_startingAge);
		// Add the particle to the vector
		this->m_myParticles.push_back(tempParticle);
	}


	return;
}

// Returns any "alive" particles
void CParticleEmitter::GetParticles(std::vector< CParticle > &vecLiveParticles)
{
	// copy the "alive" particles onto the vector that was passed.
	vecLiveParticles.clear();
	for (int index = 0; index != this->m_myParticles.size(); index++)
	{
		if (this->m_myParticles[index].age >= 0.0f)
		{	// It's ALIVE!!
			vecLiveParticles.push_back(this->m_myParticles[index]);
		}
	}
	return;
}

void CParticleEmitter::m_CreateParticle(CParticle &tempParticle)
{
	tempParticle.age = getRand(this->m_startingAge / 2.0f,
		this->m_startingAge);

	// Pick a location of the particle based on the emitter location
	// Pick a sort-of random location (within box defined by maxDistanceFromSource)
	tempParticle.position.x
		= this->m_location.x
		+ getRand<float>(-(this->m_maxDistanceFromSouce), this->m_maxDistanceFromSouce);

	tempParticle.position.y
		= this->m_location.y
		+ getRand<float>(-(this->m_maxDistanceFromSouce), this->m_maxDistanceFromSouce);

	tempParticle.position.z
		= this->m_location.z
		+ getRand<float>(-(this->m_maxDistanceFromSouce), this->m_maxDistanceFromSouce);

	// Same thing with velocity
	tempParticle.velocity.x = getRand<float>(-(this->m_initVelocity.x), this->m_initVelocity.x);
	tempParticle.velocity.y = getRand<float>(-(this->m_initVelocity.y), this->m_initVelocity.y);
	tempParticle.velocity.z = getRand<float>(-(this->m_initVelocity.z), this->m_initVelocity.z);

	return;
}


void CParticleEmitter::Update(float deltaTime)
{
	// Just like the physics update, we are integrating the new position
	for (int index = 0; index != this->m_myParticles.size(); index++)
	{
		// "Age" the particles
		this->m_myParticles[index].age -= deltaTime;
		// Dead, yet, Jim?
		if (this->m_myParticles[index].age <= 0)
		{	// Yup, he's dead.
			if (this->m_bRespawn)
			{	// Create a new particle....
				this->m_CreateParticle(this->m_myParticles[index]);
			}
		}

		// HACK:
		if (this->m_myParticles[index].age >= 0)
		{
			// Update new velocity from acceleration
			this->m_myParticles[index].velocity.x += (this->m_acceleration.x * deltaTime);
			this->m_myParticles[index].velocity.y += (this->m_acceleration.y * deltaTime);
			this->m_myParticles[index].velocity.z += (this->m_acceleration.z * deltaTime);

			// Update new position from velocity
			this->m_myParticles[index].position.x += (this->m_myParticles[index].velocity.x * deltaTime);
			this->m_myParticles[index].position.y += (this->m_myParticles[index].velocity.y * deltaTime);
			this->m_myParticles[index].position.z += (this->m_myParticles[index].velocity.z * deltaTime);
		}// if ( this->m_myParticles[index].age >= 0 )
	}

	return;
}