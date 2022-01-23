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

#include <functional>

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

	void SetCheckedTexture(TextureObject id);
	void SetUncheckedTexture(TextureObject id);

private:
	// Ui component visible
	bool visible = true;

	bool checked = false;

	std::function<void(bool)> callback;

	TextureObject uncheckedTexture;
	TextureObject checkedTexture;
};

#endif
