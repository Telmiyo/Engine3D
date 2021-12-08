#include "Application.h"
#include "ModuleTextures.h"
#include "ImGui/imgui.h"
#include "ComponentMaterial.h"

ComponentMaterial::ComponentMaterial(GameObject* parent) : Component(parent) {

	componentType = ComponentType::COMPONENT_MATERIAL;
}

void ComponentMaterial::SetTexture(const TextureObject& texture)
{
	textureName = texture.name;
	textureId = texture.id;
	width = texture.width;
	height = texture.height;
}

void ComponentMaterial::OnGui()
{
	if (ImGui::CollapsingHeader("Material"))
	{
		if (textureId != 0)
		{
			ImGui::Text("Name: %s", textureName.c_str());
			ImGui::Image((ImTextureID)textureId, ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::Text("Size: %d x %d", width, height);
		}
	}
}

void ComponentMaterial::OnLoad(const JSONReader& reader)
{
	if (reader.HasMember("Texture path"))
	{
		const rapidjson::Value& itemTexturePath = reader["Texture path"];
		textureName = itemTexturePath.GetString();
	}
	if (reader.HasMember("Size"))
	{
		const rapidjson::Value& itemSize = reader["Size"];
		int width_ = 0;
		int height_ = 0;
		int i = 0;
		for (rapidjson::Value::ConstValueIterator it = itemSize.Begin(); it != itemSize.End(); ++it)
		{
			if (i == 0) width_ = it->GetInt();
			else if (i == 1) height_ = it->GetInt();
			i++;
		}
		width = width_;
		height = height_;
	}
	if (reader.HasMember("Texture ID"))
	{
		const rapidjson::Value& itemTextureId = reader["Texture ID"];
		textureId = itemTextureId.GetInt();
	}
}

void ComponentMaterial::OnSave(JSONWriter& writer) const
{
	writer.String("Material");
	writer.StartObject();
		writer.String("Texture path");
		writer.String(textureName.c_str());
		writer.String("Size");
		writer.StartArray();
			writer.Int(width);
			writer.Int(height);
		writer.EndArray();
		writer.String("Texture ID");
		writer.Int(textureId);
	writer.EndObject();
}

