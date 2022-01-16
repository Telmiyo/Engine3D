#ifndef _COMPONENT_BUTTON_H_
#define _COMPONENT_BUTTON_H_

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

class ComponentMesh;

class ComponentButton : public Component {
public:
	enum class ButtonState {
		DISABLED, // 0
		IDLE,
		FOCUSED,
		PRESSED,
	};
public:
	ComponentButton(GameObject* parent);
	~ComponentButton();

	bool Update(float dt) override;
	void OnGui() override;

	void OnLoad(const JSONReader& reader) override;
	void OnSave(JSONWriter& writer) const override;

	inline ButtonState GetState()const { return buttonState; };
	inline void SetState(const ButtonState& newButtonState) { buttonState = newButtonState; };

private:

	ButtonState buttonState = ButtonState::DISABLED;
};

#endif