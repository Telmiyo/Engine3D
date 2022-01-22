#pragma once

#include "Component.h"
#include "Math/float2.h"
#include "Math/float3.h"
#include "ImGui/imgui.h"
#include "ComponentTransform2D.h"

class ComponentCanvas : public ComponentTransform2D {
public:
	ComponentCanvas(GameObject* parent);

	bool Update(float dt) override;

	void OnGui() override;

	void AdjustPositionAndSize();

	void GetRealPosition(float2& position) override;
	void GetRealSize(float2& realSize) override;
};