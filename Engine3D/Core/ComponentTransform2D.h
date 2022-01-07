#pragma once

#include "Component.h"
#include "Math/float3.h"
#include "Math/float4x4.h"
#include "Math/Quat.h"

class ComponentTransform2D : public Component {

public:
	ComponentTransform2D(GameObject* parent);

	bool Update(float dt) override;
	void OnGui() override;

private:
};