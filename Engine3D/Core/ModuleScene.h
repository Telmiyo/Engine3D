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

	GameObject* CreateGameObject(GameObject* parent = nullptr);	
	GameObject* CreateGameObject(const std::string name, GameObject* parent = nullptr);	
	
public:
	
	GameObject* root;
};
