#include "Application.h"
#include "ModuleTextures.h"
#include "ImGui/imgui.h"
#include "ComponentMaterial.h"

ComponentMaterial::ComponentMaterial(GameObject* parent) : Component(parent) {}

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

}
void ComponentMaterial::OnSave(JSONWriter& writer) const
{
	writer.StartObject();
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
	writer.EndObject();
}

