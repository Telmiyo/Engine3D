#pragma once
#include "Globals.h"

#include <vector>

#include "Math/float3.h"
#include "Math/float2.h"

class MeshFile {

public:
	//Needed Information
	//uint vertexBufferId = 0, indexBufferId = 0, textureBufferId = 0;

	unsigned numVertices = 0;
	unsigned verticesSize = 0;
	std::vector<float3> vertices;
	std::vector<float2> texCoords;
	uint numNormalFaces = 0;
	std::vector<float3> normals;
	uint numIndices = 0;
	std::vector<uint> indices;
	std::string name = std::string("");

	//// File Bytes Data
	//unsigned numVerticesSizeBytes = 0;
	//float3 verticesSizeBytes = float3(0.0f, 0.0f, 0.0f);
	//float2 textCoordsSizeBytes = float2(0.0f,0.0f);
	//unsigned numNormalFacesSizeBytes = 0;
	//float3 normalsSizeBytes = float3(0.0f, 0.0f, 0.0f);
	//unsigned numIndicesSizeBytes = 0;
	//float3 indicesSizeBytes = float3(0.0f, 0.0f, 0.0f);
	//std::string nameSizeBytes = 0;

};
