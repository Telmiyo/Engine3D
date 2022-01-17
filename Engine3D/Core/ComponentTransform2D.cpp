#include "ComponentTransform2D.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "Application.h"
#include "ModuleEditor.h"

ComponentTransform2D::ComponentTransform2D(GameObject* parent) : Component(parent)
{
	componentType = ComponentType::COMPONENT_TRANSFORM_2D;

	// Set default position, size, pivot, rotation & anchor
	localPosition = { 0,0 };
	size = { 200, 200 };
	pivot = { 0.5f, 0.5f };
	rotation = 0.0f;
	anchor = Anchor::TOP_LEFT;
}

bool ComponentTransform2D::Update(float dt)
{
	worldPosition = App->editor->GetScenePosition();
	localPosition = worldPosition;
	//localPosition.x -= size.x / 2 - 23;
	localPosition.y += App->editor->GetSceneSize().y - 23;
	//localPosition = GetAnchoredPosition();
	return true;
}

ImVec2 ComponentTransform2D::GetAnchoredPosition() const
{
	ImVec2 temp = {0.0f, 0.0f};

	switch (anchor)
	{
		case (Anchor::TOP_LEFT): // temp.x lo he puesto para ver si anchor va cambiando por el imgui nada mas
			temp.x += 0;
			break;
		case (Anchor::TOP_CENTER):
			temp.x += 50;
			break;
		case (Anchor::TOP_RIGHT):
			temp.x += 100;
			break;
		case (Anchor::LEFT):
			temp.x += 150;
			break;
		case (Anchor::CENTER):
			temp.x += 200;
			break;
		case (Anchor::RIGHT):
			temp.x += 250;
			break;
		case (Anchor::BOTTOM_LEFT):
			temp.x += 300;
			break;
		case (Anchor::BOTTOM_CENTER):
			temp.x += 350;
			break;
		case (Anchor::BOTTOM_RIGHT):
			temp.x += 400;
			break;
	}

	return temp;
}

void ComponentTransform2D::OnGui()
{
	if (ImGui::CollapsingHeader("Transform 2D"))
	{
		ImVec2 newPosition = GetPosition();
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

void ComponentTransform2D::SetPosition(const ImVec2& newPosition)
{
	localPosition = newPosition;
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

void ComponentTransform2D::OnSave(JSONWriter& writer) const
{
	writer.String("Transform 2D");
	writer.StartObject();

	// Saving position
	writer.String("Position");
	writer.StartArray();
	writer.Double(GetPosition().x);
	writer.Double(GetPosition().y);
	writer.EndArray();
	
	// Saving pivot
	writer.String("Pivot");
	writer.StartArray();
	writer.Double(GetPivot().x);
	writer.Double(GetPivot().y);
	writer.EndArray();

	// Saving rotation
	writer.String("Rotation");
	writer.StartArray();
	writer.Double(GetRotation());
	writer.EndArray();

	// Saving size
	writer.String("Size");
	writer.StartArray();
	writer.Double(GetSize().x);
	writer.Double(GetSize().y);
	writer.EndArray();

	// Saving anchor
	writer.String("Anchor");
	writer.StartArray();
	writer.Int((int)GetAnchor());
	writer.EndArray();
	writer.EndObject();
}

void ComponentTransform2D::OnLoad(const JSONReader& reader)
{
	// Loading position
	if (reader.HasMember("Position"))
	{
		const rapidjson::Value& itemPosition = reader["Position"];
		float positionX = 0.0f;
		float positionY = 0.0f;
		int i = 0;
		for (rapidjson::Value::ConstValueIterator it = itemPosition.Begin(); it != itemPosition.End(); ++it)
		{
			if (i == 0) positionX = it->GetDouble();
			else if (i == 1) positionY = it->GetDouble();
			i++;
		}
		SetPosition(ImVec2(positionX, positionY));
	}

	// Loading pivot
	if (reader.HasMember("Pivot"))
	{
		const rapidjson::Value& itemPivot = reader["Pivot"];
		float pivotX = 0.0f;
		float pivotY = 0.0f;
		int i = 0;
		for (rapidjson::Value::ConstValueIterator it = itemPivot.Begin(); it != itemPivot.End(); ++it)
		{
			if (i == 0) pivotX = it->GetDouble();
			else if (i == 1) pivotY = it->GetDouble();
			i++;
		}
		SetPivot(float2(pivotX, pivotY));
	}

	// Loading rotation
	if (reader.HasMember("Rotation"))
	{
		const rapidjson::Value& itemRotation = reader["Rotation"];
		float newRotation;
		int i = 0;
		for (rapidjson::Value::ConstValueIterator it = itemRotation.Begin(); it != itemRotation.End(); ++it)
		{
			if (i == 0) newRotation = it->GetDouble();
			i++;
		}
		SetRotation(newRotation);
	}

	// Loading size
	if (reader.HasMember("Size"))
	{
		const rapidjson::Value& itemSize = reader["Size"];
		float sizeX = 0.0f;
		float sizeY = 0.0f;
		int i = 0;
		for (rapidjson::Value::ConstValueIterator it = itemSize.Begin(); it != itemSize.End(); ++it)
		{
			if (i == 0) sizeX = it->GetDouble();
			else if (i == 1) sizeY = it->GetDouble();
			i++;
		}
		SetSize(float2(sizeX, sizeY));
	}

	// Loading anchor
	if (reader.HasMember("Anchor"))
	{
		const rapidjson::Value& itemAnchor = reader["Anchor"];
		int newAnchor = 0;
		int i = 0;
		for (rapidjson::Value::ConstValueIterator it = itemAnchor.Begin(); it != itemAnchor.End(); ++it)
		{
			if (i == 0) newAnchor = it->GetInt();
			i++;
		}
		SetAnchor((Anchor)newAnchor);
	}
}


