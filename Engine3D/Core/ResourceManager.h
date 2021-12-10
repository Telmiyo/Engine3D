#pragma once
#include "Module.h"

#include <vector>
#include <string>

#include "Assimp/include/mesh.h"
#include "MeshFile.h"

class ResourceManager : public Module{

public:
	ResourceManager(Application* app, bool start_enabled = true);
	~ResourceManager();

	//Model
	bool CreateModelFile(const aiMesh* m, const char* path, std::string name);
	bool saveModelFile(MeshFile* file, const char* path, std::string name);
	MeshFile* LoadMeshFile(std::string name);
	
	void ArrayToVectorConversion(MeshFile* mymodel);
	
	std::vector<float3> FloatArray2VecFloat3(float* array, unsigned int n);
	std::vector<float2> FloatArray2VecFloat2(float* array, unsigned int n);
	std::vector<unsigned int> UnsignedIntArray2VecFloat3(unsigned int* array, unsigned int n);
	//Load Texture

	//Data Containers
	std::vector<MeshFile*> models;

};

