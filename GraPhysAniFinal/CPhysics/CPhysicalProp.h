#ifndef _CPhysicalProp_HG_
#define _CPhysicalProp_HG_

// Stores physical properties of objects

#include "../PhysicsMailMan.h"

class CPhysicalProp
{
public:
	


	CPhysicalProp();
	CPhysicalProp( CVector3f position_ );
	~CPhysicalProp() {};

	// if false, physics update is ignored (including collision)
	// Basically, this is for things like skyboxes, UI, debug shapes, etc. (Usually no CPhyDesc, too)
	bool bIncludeInPhysicsUpdate;				

	CVector3f position;
	CVector3f velocity;
	CVector3f acceleration;

	void setMatrix(glm::mat4 newMatrix);
	void setMailMan(PhysicsMailMan newMailMan);

	glm::mat4 matrix;

	glm::fquat orientation;	
	CVector3f directedVelocity;			// Based on orientation (like "thrust" in a certain direction)
	CVector3f directedAcceleration;		// Based on orientation
	glm::fquat rotationalSpeed;
	CVector3f rotStep;

	// "set" over-writes the existing values
	// "adj" adjusts ("adds"/updates) the angles
	void setOrientationEulerAngles( CVector3f newAnglesEuler );
	void adjOrientationEulerAngles( CVector3f adjAngleEuler );
	void setOrientationQ( glm::fquat newOrientQ );
	void adjOrientationQ( glm::fquat adjOrientQ );

	void setRotationalSpeedEuler( CVector3f newRotSpeedEuler );
	void adjRotationalSpeedEuler( CVector3f adjRotSpeedEuler );
	void setRotationalSpeedQ( glm::fquat newRotSpeedQ );
	void adjRotationalSpeedQ( glm::fquat adjRotSpeedQ );

	float frictionLinear;			// As a percentage of speed per frame	(default = 0.0f)
	float frictionLinearSquared;	// As a percentage of the square of the speed, per frame (default = 0.0f)
	float frictionRotational;		// As a percentage of speed per frame	
	float mass;
	float inverseMass;
	// Objects that have infinite mass aren't updated by the physics (gravity, etc.)
	bool bInfiniteMass;		

private:
	void m_Init(void);

};

#endif
