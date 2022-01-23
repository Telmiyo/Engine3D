#pragma once

#include "Globals.h"
#include "Component.h"
#include "ComponentTransform2D.h"
#include "ModuleInput.h"


#include "glew.h"
#include "Math/float2.h"
#include "Math/float4.h"
#include "Math/float4x4.h"


#include <string.h>
#include <vector>

class ComponentWindow : public Component {
public:
	ComponentWindow(GameObject* parent);
	~ComponentWindow();

	bool Update(float dt) override;
	void OnGui() override;

	void OnLoad(const JSONReader& reader) override;
	void OnSave(JSONWriter& writer) const override;

private:

	float smoothness = 1.f;
};



