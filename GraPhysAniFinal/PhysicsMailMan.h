#ifndef _PhysicsMailMan_HG_
#define _PhysicsMailMan_HG_


#include <glm/glm.hpp>
#include <glm/mat4x4.hpp> // glm::mat4
#include "CCommonTypes/CVector3f.h"
#include <glm/gtc/quaternion.hpp>	


class PhysicsMailMan{
public:
	glm::mat4 matrix;
	CVector3f position;
	glm::vec4 orientation;
};


#endif