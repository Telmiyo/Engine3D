#include "ComponentTransform.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Math/TransformOps.h"
#include "glew.h"
#include "ImGui/imgui.h"

ComponentTransform::ComponentTransform(GameObject* parent) : Component(parent) {

	position = float3::zero;
	rotation = Quat::identity;
	scale = float3::one;

	transformMatrix.SetIdentity();
	transformMatrixLocal.SetIdentity();

	componentType = ComponentType::COMPONENT_TRANSFORM;
}


bool ComponentTransform::Update(float dt) {
	if (isDirty)
	{
		transformMatrixLocal = float4x4::FromTRS(position, rotation, scale);

		right = transformMatrixLocal.Col3(0).Normalized();
		up = transformMatrixLocal.Col3(1).Normalized();
		front = transformMatrixLocal.Col3(2).Normalized();
		RecomputeGlobalMatrix();
		owner->PropagateTransform();
		isDirty = false;
	}
	return true;
}

void ComponentTransform::OnGui()
{
	if (ImGui::CollapsingHeader("Transform"))
	{
		float3 newPosition = position;
		if (ImGui::DragFloat3("Location", &newPosition[0]))
		{
			SetPosition(newPosition);
		}
		float3 newRotationEuler;
		newRotationEuler.x = RADTODEG * rotationEuler.x;
		newRotationEuler.y = RADTODEG * rotationEuler.y;
		newRotationEuler.z = RADTODEG * rotationEuler.z;
		if (ImGui::DragFloat3("Rotation", &(newRotationEuler[0])))
		{
			newRotationEuler.x = DEGTORAD * newRotationEuler.x;
			newRotationEuler.y = DEGTORAD * newRotationEuler.y;
			newRotationEuler.z = DEGTORAD * newRotationEuler.z;
			SetRotation(newRotationEuler);
		}
		float3 newScale = scale;
		if (ImGui::DragFloat3("Scale", &(newScale[0])))
		{
			SetScale(newScale);
		}
	}
}

void ComponentTransform::SetPosition(const float3& newPosition)
{
	position = newPosition;
	isDirty = true;
}

void ComponentTransform::SetRotation(const float3& newRotation)
{
	Quat rotationDelta = Quat::FromEulerXYZ(newRotation.x - rotationEuler.x, newRotation.y - rotationEuler.y, newRotation.z - rotationEuler.z);
	rotation = rotation * rotationDelta;
	rotationEuler = newRotation;
	isDirty = true;
}

void ComponentTransform::SetScale(const float3& newScale)
{
	scale = newScale;
	isDirty = true;
}

void ComponentTransform::NewAttachment()
{
	if (owner->parent != App->scene->root)
		transformMatrixLocal = owner->parent->transform->transformMatrix.Inverted().Mul(transformMatrix);

	float3x3 rot;
	transformMatrixLocal.Decompose(position, rot, scale);
	rotationEuler = rot.ToEulerXYZ();
}

void ComponentTransform::OnParentMoved()
{
	RecomputeGlobalMatrix();
}

void ComponentTransform::RecomputeGlobalMatrix()
{
	if (owner->parent != nullptr)
	{
		transformMatrix = owner->parent->transform->transformMatrix.Mul(transformMatrixLocal);
	}
	else
	{
		transformMatrix = transformMatrixLocal;
	}
}

void ComponentTransform::OnSave(JSONWriter& writer) const
{
	writer.String("Transform");
	writer.StartObject();
		writer.String("Position");
		writer.StartArray();
			writer.Double(position.x);
			writer.Double(position.y);
			writer.Double(position.z);
		writer.EndArray();
		writer.String("Rotation");
		writer.StartArray();
			writer.Double(GetRotation().x);
			writer.Double(GetRotation().y);
			writer.Double(GetRotation().z);
		writer.EndArray();
		writer.String("Scale");
		writer.StartArray();
			writer.Double(scale.x);
			writer.Double(scale.y);
			writer.Double(scale.z);
		writer.EndArray();
	writer.EndObject();
}

void ComponentTransform::OnLoad(const JSONReader& reader)
{
	if (reader.HasMember("Position"))
	{
		const rapidjson::Value& itemPosition = reader["Position"];
		float posX = 0.0f;
		float posY = 0.0f; 
		float posZ = 0.0f;
		int i = 0;
		for (rapidjson::Value::ConstValueIterator it = itemPosition.Begin(); it != itemPosition.End(); ++it)
		{
			if (i == 0) posX = it->GetDouble();
			else if (i == 1) posY = it->GetDouble();
			else if (i == 2) posZ = it->GetDouble();
			i++;
		}
		SetPosition(float3(posX, posY, posZ));
	}
	if (reader.HasMember("Rotation"))
	{
		const rapidjson::Value& itemRotation = reader["Rotation"];
		float3 newRotation;
		int i = 0;
		for (rapidjson::Value::ConstValueIterator it = itemRotation.Begin(); it != itemRotation.End(); ++it)
		{
			if (i == 0) newRotation.x = it->GetDouble();
			else if (i == 1) newRotation.y = it->GetDouble();
			else if (i == 2) newRotation.z = it->GetDouble();
			i++;
		}
		SetRotation(float3(newRotation.x, newRotation.y, newRotation.z));
	}
	if (reader.HasMember("Scale"))
	{
		const rapidjson::Value& itemScale = reader["Scale"];
		float scaleX = 0.0f;
		float scaleY = 0.0f;
		float scaleZ = 0.0f;
		int i = 0;
		for (rapidjson::Value::ConstValueIterator it = itemScale.Begin(); it != itemScale.End(); ++it)
		{
			if (i == 0) scaleX = it->GetDouble();
			else if (i == 1) scaleY = it->GetDouble();
			else if (i == 2) scaleZ = it->GetDouble();
			i++;
		}
		SetScale(float3(scaleX, scaleY, scaleZ));
	}
}
