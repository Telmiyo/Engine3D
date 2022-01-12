#include "ComponentTransform2D.h"
#include "ImGui/imgui.h"

ComponentTransform2D::ComponentTransform2D(GameObject* parent) : Component(parent)
{
	componentType = ComponentType::COMPONENT_TRANSFORM_2D;
}

bool ComponentTransform2D::Update(float dt)
{
	return true;
}

void ComponentTransform2D::OnGui()
{
	if (ImGui::CollapsingHeader("Transform 2D"))
	{
		float2 newPosition = position;
		if (ImGui::DragFloat2("Location", &newPosition[0]))
		{
			position = newPosition;
		}
		float2 newPivot = pivot;
		if (ImGui::DragFloat2("Pivot", &newPivot[0]))
		{
			pivot = newPivot;
		}
		float newRotation = rotation;
		if (ImGui::DragFloat("Rotation", &newRotation))
		{
			rotation = newRotation;
		}
		float2 newSize = size;
		if (ImGui::DragFloat2("Size", &newSize[0]))
		{
			size = newSize;
		}
	}
}

void ComponentTransform2D::GetScreenRect(float2& a, float2& b)
{
	a = { 0, 0 };
	b = { 100, 100 };
}
