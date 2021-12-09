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
	MeshFile* loadModel(std::string name);

	//Load Texture

	//Data Containers
	std::vector<MeshFile*> models;

};

