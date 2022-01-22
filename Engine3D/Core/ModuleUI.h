#pragma once
#include "Module.h"
#include "Globals.h"
#include "Light.h"

#include "glew.h"

class ComponentCamera;

class ModuleUI : public Module
{
public:
	ModuleUI(Application* app, bool start_enabled = true);
	~ModuleUI();

	bool Init() override;
	bool Start() override;
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp() override;

	void OnGui() override;
	void OnLoad(const JSONReader& reader) override;
	void OnSave(JSONWriter& writer) const override;

public:
	ComponentCamera* uiCamera;

	GLint uiCameraViewport[4] = {0, 0, 0, 0};
};