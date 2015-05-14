#ifndef _CParticleEmitter_HG_
#define _CParticleEmitter_HG_

// Manage all the particles
#include "CCommonTypes/CVector3f.h"
#include "CGameObjects\CGameObject.h"
#include <vector>



class CParticleEmitter
{
public:
	CParticleEmitter();
	~CParticleEmitter();
	void SetLocation( CVector3f newLocation );
	void SetAcceleration( CVector3f newAcceleration );
	void GenerateParticles( int numParticles, CVector3f initVelocity,
		                    float maxDistanceFromSouce, 
							float startingAge,	// Added
							bool bRespawn );	// Added
	void SetRespawn( bool bNewRespawn );		// Added
	void Update( float deltaTime );

	// "Shouldn't" make this public, but for time, we'll do it
	class CParticle
	{
	public:
		CParticle() {};	// construtor defined here
		~CParticle() {};	// 
		CVector3f position;		// c'tor sets these to zeros
		CVector3f velocity;		// c'tor sets these to zeros
		float age;
		CMeshDesc meshDesc;
	};
	// Returns any "alive" particles
	void GetParticles( std::vector< CParticle > &vecLiveParticles ); 

private:
	std::vector< CParticle > m_myParticles;	
	unsigned int m_numParticles;
	CVector3f m_location;
	CVector3f m_acceleration;
	// 
	CVector3f m_initVelocity;
	float m_maxDistanceFromSouce;
	float m_startingAge;
	bool m_bRespawn;

	// Called internally
	void m_CreateParticle(CParticle &tempParticle);
	static const unsigned int INITPARTICLEARRAYSIZE = 100;
};

#endif
