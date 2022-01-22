#ifndef _COMPONENT_CHECKBOX_H_
#define _COMPONENT_CHECKBOX_H_

#include <string.h>
#include <vector>
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "Math/float2.h"
#include "glew.h"

#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleScene.h"
#include "ModuleTextures.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"
#include "Globals.h"

#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTransform2D.h"
#include "ComponentCanvas.h"
#include "ComponentImage.h"

class ComponentMesh;

class ComponentCheckbox : public Component {
public:

public:
	ComponentCheckbox(GameObject* parent);
	~ComponentCheckbox();

	bool Update(float dt) override;
	void OnGui() override;

	void OnLoad(const JSONReader& reader) override;
	void OnSave(JSONWriter& writer) const override;

	inline bool IsChecked()const { return checked; };
	inline void SetCheckboxState(const bool& newCheckboxState) { checked = newCheckboxState; };

private:

	bool active = true;

	bool checked = false;
};

#endif
