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

	float2 GetMousePosition();

	void OnGui() override;
	void OnLoad(const JSONReader& reader) override;
	void OnSave(JSONWriter& writer) const override;

	void FadeOut();
	void FadeIn();

public:

	GLint uiCameraViewport[4] = {0, 0, 0, 0};

	bool fadeOut = false;
};