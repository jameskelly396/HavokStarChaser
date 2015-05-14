#include "CPhysicalProp.h"

CPhysicalProp::CPhysicalProp()
{
	this->m_Init();
	return;
}

CPhysicalProp::CPhysicalProp( CVector3f position_ ) 
{
	this->m_Init();
	this->position = position_;
	return;
}

void CPhysicalProp::m_Init(void)
{
	this->bIncludeInPhysicsUpdate = true;

	this->bInfiniteMass = false;
	this->frictionLinear = 0.0f;
	this->frictionLinearSquared = 0.0f;
	this->frictionRotational = 0.0f;
	this->rotStep = CVector3f(0.0f, 0.0f, 0.0f);
	this->mass = 1.0f;
	this->inverseMass = 1.0f;
	return;
}


void CPhysicalProp::setMatrix(glm::mat4 newMatrix){
	this->matrix = newMatrix;
}

void CPhysicalProp::setMailMan(PhysicsMailMan newMailMan){
	this->matrix = newMailMan.matrix;
	this->position = newMailMan.position;
	this->orientation.x = newMailMan.orientation.x;
	this->orientation.y = newMailMan.orientation.y;
	this->orientation.z = newMailMan.orientation.z;
	this->orientation.w = newMailMan.orientation.w;
}

void CPhysicalProp::setOrientationEulerAngles( CVector3f newAnglesEuler )
{
	// Unlike Euler angles, we can't just "add" the corresponding angles to the xyz values inside the 
	//	quaternion. But we can do something like we did with the matrix math 
	this->orientation = glm::fquat( glm::vec3( newAnglesEuler.x, newAnglesEuler.y, newAnglesEuler.z ) );
	return;
}

void CPhysicalProp::adjOrientationEulerAngles( CVector3f adjAngleEuler )
{
	// Step 1: make a quaternion that represents the angle we want to rotate
	glm::fquat rotationAdjust( glm::vec3( adjAngleEuler.x, 
		                                  adjAngleEuler.y, 
										  adjAngleEuler.z ) );	
	// Step 2: Multiply this quaternion by the existing quaternion. This "adds" the angle we want.
	this->orientation *= rotationAdjust;
	return;
}

void CPhysicalProp::setOrientationQ( glm::fquat newOrientQ )
{
	this->orientation = newOrientQ;
	return;
}

void CPhysicalProp::adjOrientationQ( glm::fquat adjOrientQ )
{
	this->orientation *= adjOrientQ;
	return;
}

void CPhysicalProp::setRotationalSpeedEuler( CVector3f newRotSpeedEuler )
{
	this->rotationalSpeed = glm::fquat( glm::vec3( newRotSpeedEuler.x, newRotSpeedEuler.y, newRotSpeedEuler.z ) );
	return;
}

void CPhysicalProp::adjRotationalSpeedEuler( CVector3f adjRotSpeedEuler )
{
	// Step 1: make a quaternion that represents the angle we want to rotate
	glm::fquat rotationSpeedAdjust( glm::vec3( adjRotSpeedEuler.x, 
		                                       adjRotSpeedEuler.y, 
											   adjRotSpeedEuler.z ) );	
	// Step 2: Multiply this quaternion by the existing quaternion. This "adds" the angle we want.
	this->rotationalSpeed *= rotationSpeedAdjust;
	return;
}

void CPhysicalProp::setRotationalSpeedQ( glm::fquat newRotSpeedQ )
{
	this->rotationalSpeed = newRotSpeedQ;
	return;
}

void CPhysicalProp::adjRotationalSpeedQ( glm::fquat adjRotSpeedQ )
{
	this->rotationalSpeed *= adjRotSpeedQ;
	return;
}
