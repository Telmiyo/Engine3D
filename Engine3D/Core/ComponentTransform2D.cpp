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
		float2 newPosition = GetPosition();
		if (ImGui::DragFloat2("Location", &newPosition[0]))
		{
			SetPosition(newPosition);
		}
		float2 newPivot = GetPivot();
		if (ImGui::DragFloat2("Pivot", &newPivot[0]))
		{
			SetPivot(newPivot);
		}
		float newRotation = GetRotation();
		if (ImGui::DragFloat("Rotation", &newRotation))
		{
			SetRotation(newRotation);
		}
		float2 newSize = GetSize();
		if (ImGui::DragFloat2("Size", &newSize[0]))
		{
			SetSize(newSize);
		}
		int newAnchor = (int)GetAnchor();
		if (ImGui::Combo("Anchor", &newAnchor, "TOP_LEFT\0TOP_CENTER\0TOP_RIGHT\0LEFT\0CENTER\0RIGHT\0BOTTOM_LEFT\0BOTTOM_CENTER\0BOTTOM_RIGHT\0\0"))
		{
			SetAnchor((Anchor)newAnchor);
		}
	}
}

void ComponentTransform2D::GetScreenRect(float2& a, float2& b)
{
	a = { 0, 0 };
	b = { 100, 100 };
}

void ComponentTransform2D::SetPosition(const float2& newPosition)
{
	position = newPosition;
}
void ComponentTransform2D::SetPivot(const float2& newPivot)
{
	pivot = newPivot;
}
void ComponentTransform2D::SetRotation(const float& newRotation)
{
	rotation = newRotation;
}
void ComponentTransform2D::SetSize(const float2& newSize)
{
	size = newSize;
}
void ComponentTransform2D::SetAnchor(const Anchor& newAnchor)
{
	anchor = newAnchor;
}


