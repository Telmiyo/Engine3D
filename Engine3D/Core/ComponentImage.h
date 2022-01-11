#pragma once

#include "Component.h"

class ComponentImage : public Component {
public:
	ComponentImage(GameObject* parent);

	bool Update(float dt) override;
	void OnGui() override;
};