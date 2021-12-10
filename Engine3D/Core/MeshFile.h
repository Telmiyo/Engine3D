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

class MontuMeshFile
{
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

};
