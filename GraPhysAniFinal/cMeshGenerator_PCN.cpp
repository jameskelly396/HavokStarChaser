#include "cMeshGenerator_PCN.h"
#include <glm/glm.hpp>
#include <fstream>
#include <string>

cMeshGenerator_PCN::cMeshGenerator_PCN(void)
{
}


cMeshGenerator_PCN::~cMeshGenerator_PCN(void)
{
}

void cMeshGenerator_PCN::GenerateGrid( float width, float height, int numNodesHoriz, int numNodesVert, std::vector<sVert_PCN>& vertices, std::vector<unsigned int>& indices, const glm::vec3& color, const glm::vec3& norm, const glm::vec3& axisX, const glm::vec3& axisY, const glm::vec3& origin )
{
	unsigned int indexOffset = vertices.size();
	glm::vec3 xOffset = axisX * -width / 2.0f;
	glm::vec3 yOffset = axisY * -height / 2.0f;
	glm::vec3 dx = axisX * width / (float)(numNodesHoriz - 1);
	glm::vec3 dy = axisY * height / (float)(numNodesVert - 1);
	glm::vec3 normalVec = glm::normalize(norm);
	for (int idxRow = 0; idxRow < numNodesVert; idxRow++)
	{
		glm::vec3 y = axisY * (-(yOffset + dy * (float)idxRow));
		for (int idxCol = 0; idxCol < numNodesHoriz; idxCol++)
		{
			glm::vec3 x = origin + axisX * (xOffset + dx * (float)idxCol);
			sVert_PCN vert;
			vert.SetNormal(normalVec.x, normalVec.y, normalVec.z);
			vert.SetPosition(x.x + y.x, x.y + y.y, x.z + y.z);
			vert.SetColor(color.r, color.g, color.b);
			vertices.push_back(vert);
		}
	}

	for (int idxRow = 0; idxRow < numNodesVert - 1; idxRow++)
	{
		for (int idxCol = 0; idxCol < numNodesHoriz - 1; idxCol++)
		{
			unsigned int idxA = (idxRow * numNodesHoriz) + idxCol + indexOffset;
			unsigned int idxB = idxA + 1;
			unsigned int idxC = idxA + numNodesHoriz;
			unsigned int idxD = idxC + 1;

			indices.push_back(idxA);
			indices.push_back(idxC);
			indices.push_back(idxB);

			indices.push_back(idxB);
			indices.push_back(idxC);
			indices.push_back(idxD);
		}
	}
}

void LoadPlyFile(std::vector<sVert_PCN>& vertices, std::vector<unsigned int>& indices, const glm::vec3& color)
{
	std::ifstream myFile("ply/sphere_UV_onlyXYZ.ply");
	if (!myFile.is_open())
		return;

	// Read everything up to the word "vertex"
	std::string temp;
	bool bKeepReading = true;
	while (bKeepReading)
	{
		myFile >> temp;				// Read next "string" into temp
		if (temp == "vertex")
		{
			bKeepReading = false;
		}
	};
	int numberOfVertices = 0;
	// Next value is number of vertices
	myFile >> numberOfVertices;

	// Same thing, but number of triangles (aka "elements")
	bKeepReading = true;
	while (bKeepReading)
	{
		myFile >> temp;
		if (temp == "face")
		{
			bKeepReading = false;
		}
	};
	int numberOfTriangles = 0;
	// Next value is number of vertices
	myFile >> numberOfTriangles;

	// Same thing, but look for 
	bKeepReading = true;
	while (bKeepReading)
	{
		myFile >> temp;
		if (temp == "end_header")
		{
			bKeepReading = false;
		}
	};

	// Now, I can read the actual information... 
	for (int count = 0; count != numberOfVertices; count++)
	{
		float width = -1.f;
		float height = 3.f;
		float depth = -1.f;
		float x, y, z;
		myFile >> x >> y >> z;
		sVert_PCN vert;
		vert.SetNormal(0.0f, 0.0f, 1.0f);
		vert.SetPosition(x, y, z);
		vert.SetColor(color.r, color.g, color.b);
		vertices.push_back(vert);
	}

	// 
	for (int count = 0; count != numberOfTriangles; count++)
	{
		unsigned int idxA, idxB, idxC;
		int discard;
		// 3 564 612 611 
		myFile >> discard >> idxA >> idxB >> idxC;

		indices.push_back(idxA);
		indices.push_back(idxC);
		indices.push_back(idxB);
	}

	myFile.close();  // It's nice to be nice. Close the file.

	return;
}

void cMeshGenerator_PCN::GenerateSphere( std::vector<sVert_PCN>& vertices, std::vector<unsigned int>& indices, const glm::vec3& color)
{
	LoadPlyFile(vertices, indices, color);

}

void cMeshGenerator_PCN::GenerateBox( const glm::vec3& halfWidths, int numNodesPerEdge, std::vector<sVert_PCN>& vertices, std::vector<unsigned int>& indices, const glm::vec3& color )
{
	glm::vec3 axisX(1.f, 0.f, 0.f);
	glm::vec3 axisY(0.f, 1.f, 0.f);
	glm::vec3 axisZ(0.f, 0.f, 1.f);

	// top x-z plane
	GenerateGrid(halfWidths.x * 2, halfWidths.z * 2, numNodesPerEdge, numNodesPerEdge, vertices, indices, color, glm::vec3(0.f, 1.f, 0.f), axisX, axisZ, glm::vec3(0.f, halfWidths.y, 0.f));
	// bottom x-z plane
	GenerateGrid(halfWidths.x * 2, halfWidths.z * 2, numNodesPerEdge, numNodesPerEdge, vertices, indices, color, glm::vec3(0.f, -1.f, 0.f), axisX, axisZ, glm::vec3(0.f, -halfWidths.y, 0.f));
	// front x-y plane
	GenerateGrid(halfWidths.x * 2, halfWidths.y * 2, numNodesPerEdge, numNodesPerEdge, vertices, indices, color, glm::vec3(0.f, 0.f, 1.f), axisX, axisY, glm::vec3(0.f, 0.f, halfWidths.z));
	// back x-y plane
	GenerateGrid(halfWidths.x * 2, halfWidths.y * 2, numNodesPerEdge, numNodesPerEdge, vertices, indices, color, glm::vec3(0.f, 0.f, -1.f), axisX, axisY, glm::vec3(0.f, 0.f, -halfWidths.z));
	// left y-z plane
	GenerateGrid(halfWidths.y * 2, halfWidths.z * 2, numNodesPerEdge, numNodesPerEdge, vertices, indices, color, glm::vec3(-1.f, 0.f, 0.f), axisY, axisZ, glm::vec3(-halfWidths.x, 0.f, 0.f));
	// right y-z plane
	GenerateGrid(halfWidths.y * 2, halfWidths.z * 2, numNodesPerEdge, numNodesPerEdge, vertices, indices, color, glm::vec3(1.f, 0.f, 0.f), axisY, axisZ, glm::vec3(halfWidths.x, 0.f, 0.f));

	glm::vec3 mx(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
	glm::vec3 mn(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	std::vector<sVert_PCN>::iterator it = vertices.begin();
	while (it != vertices.end())
	{
		mx.x = std::max(mx.x, it->Position[0]);
		mx.y = std::max(mx.y, it->Position[1]);
		mx.z = std::max(mx.y, it->Position[2]);
		mn.x = std::min(mn.x, it->Position[0]);
		mn.y = std::min(mn.y, it->Position[1]);
		mn.z = std::min(mn.y, it->Position[2]);
		it++;
	}
}
