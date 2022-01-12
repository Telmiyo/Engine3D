#pragma once

#include "Component.h"

#include "glew.h"

class ComponentMesh;

class ComponentImage : public Component {
public:
	ComponentImage(GameObject* parent);

	bool Update(float dt) override;
	void OnGui() override;

private:
	ComponentMesh* plane = nullptr;
};