#pragma once

// -- Tools
#include "Module.h"
#include "Globals.h"
#include "ModuleImport.h"

#include "GameObject.h"
class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);

	bool Start() override;
	update_status Update(float dt) override;
	bool CleanUp() override;

	void OnSave(std::string scene) const;
	void OnLoad(std::string scene);

	bool DeleteAllGameObjects();
	bool DeleteSelectedGameObject(GameObject* selectedGameObject);

	GameObject* CreateGameObjectByName(const std::string name, GameObject* parent = nullptr);
	GameObject* CreateEmptyGameObject(GameObject* parent = nullptr);
	GameObject* CreateChildrenGameObject(GameObject* parent = nullptr);
	GameObject* CreateCamera(GameObject* parent = nullptr);

	void DuplicateGameObject(GameObject* parent = nullptr);
	void CreateRoot();
	
private:
	int emptyCounter = 0;

public:
	GameObject* root;
	std::vector<GameObject*> gameObjectList;
	std::vector<GameObject*> rootList;
};
