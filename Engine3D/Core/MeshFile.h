#pragma once
#include "Globals.h"

#include <vector>

#include "Math/float3.h"
#include "Math/float2.h"

class MeshFile {

public:
	unsigned int verticesSizeBytes = 0;
	unsigned int normalsSizeBytes = 0;
	unsigned int indiceSizeBytes = 0;
	unsigned int textCoordSizeBytes = 0;

	float* vertices_ = nullptr; // 1 vertex = sizeof(float) * 3
	float* normals_ = nullptr; // 1 normal = sizeof(float) * 3
	float* textCoords_ = nullptr; // 1 textCoord = sizeof(float) * 2
	unsigned int* indices_ = nullptr; // 1 index = sizeof(unsigned int) * 1

	std::vector<float3> vecVertices;
	std::vector<float3> vecNormals;
	std::vector<unsigned int> vecIndices;
	std::vector<float2> vecTextCoords;


	std::string name = std::string("");
	std::string texturePath = std::string("");
};
