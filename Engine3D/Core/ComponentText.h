#ifndef _COMPONENT_TEXT_H_
#define _COMPONENT_TEXT_H_

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

class ComponentText : public Component {
public:

public:
	ComponentText(GameObject* parent);
	~ComponentText();

	bool Update(float dt) override;
	void OnGui() override;

	void OnLoad(const JSONReader& reader) override;
	void OnSave(JSONWriter& writer) const override;

	inline const char* GetText()const { return text; };
	inline void SetText(const char* newText) { text = newText; };

private:
	// Ui component visible
	bool visible = true;

	// Input text
	const char* text = nullptr;

	// Imgui input text buffer && set default text
	char textBuffer[400] = "Default text";
};

#endif
