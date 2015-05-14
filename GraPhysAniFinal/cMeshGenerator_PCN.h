#pragma once
#ifndef __cMeshGenerator_2015_02_06
#define __cMeshGenerator_2015_02_06

#include "VertexTypes.h"
#include <vector>
#include <glm/vec3.hpp>

class cMeshGenerator_PCN
{
public:
	cMeshGenerator_PCN(void);
	~cMeshGenerator_PCN(void);
	static void GenerateBox(const glm::vec3& halfWidths, int numNodesPerEdge, std::vector<sVert_PCN>& vertices, std::vector<unsigned int>& indices, const glm::vec3& color);
	static void GenerateSphere(std::vector<sVert_PCN>& vertices, std::vector<unsigned int>& indices, const glm::vec3& color);
	static void GenerateGrid(float width, float height, int numNodesHoriz, int numNodesVert, std::vector<sVert_PCN>& vertices, std::vector<unsigned int>& indices, const glm::vec3& color, const glm::vec3& norm, const glm::vec3& axisX, const glm::vec3& axisY, const glm::vec3& origin);
};

#endif