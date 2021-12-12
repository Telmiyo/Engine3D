#pragma once
#include "Module.h"

#include <vector>
#include <string>
#include <map>

#include "Assimp/include/mesh.h"
#include "MeshFile.h"

typedef std::string UID;

class ResourceManager : public Module{

public:
	ResourceManager(Application* app, bool start_enabled = true);
	~ResourceManager();

	bool Init() override;
	void CheckMeshFiles();

	UID generateNewUID();


	//Model
	bool CreateModelFile(const aiMesh* m, const char* path, std::string name);
	bool saveModelFile(MeshFile* file, const char* path, std::string name);
	MeshFile* LoadMeshFile(std::string name);
	void DestroyMeshFile(MeshFile* mesh);
	
	void ArrayToVectorConversion(MeshFile* mymodel);
	
	std::vector<float3> FloatArray2VecFloat3(float* array, unsigned int n);
	std::vector<float2> FloatArray2VecFloat2(float* array, unsigned int n);
	std::vector<unsigned int> UnsignedIntArray2VecFloat3(unsigned int* array, unsigned int n);
	//Load Texture

	//Data Containers
	std::vector<MeshFile*> models;
	std::string dir;
	std::map<UID, MeshFile*> resources;

};

