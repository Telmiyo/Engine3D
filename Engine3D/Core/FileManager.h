#pragma once
#pragma once
#include "Module.h"
#include "File.h"

#include "Assimp/include/mesh.h"
#include <string>

class ComponentMesh;
struct aiScene;

class FileManager : public Module
{
public:

	FileManager(Application* app, bool start_enabled = true);

	bool Init() override;
	update_status Update(float dt) override;
	bool CleanUp() override;

	File* createFile(const aiMesh* m);
	bool saveFile(File* file);
	bool loadfile(File* file);
	bool removeFile(File* file);

};
