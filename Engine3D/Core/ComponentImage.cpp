#include "ComponentImage.h"

#include "ImGui/imgui.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"
#include "ModuleEditor.h"
#include "Math/float3.h"
#include "Math/float4x4.h"
#include "ModuleScene.h"
#include "Math/Quat.h"
#include "ComponentMesh.h"
#include "glew.h"
#include <vector>

ComponentImage::ComponentImage(GameObject* parent) : Component(parent)
{
	componentType = ComponentType::COMPONENT_IMAGE;
	plane = new ComponentMesh(nullptr);
	App->scene->CreatePlane(plane);
	plane->GenerateBuffers();
	if (plane->texCoords.size() != 0)
	{
		textureBufferId = 0;
		glGenBuffers(1, &textureBufferId);
		glBindBuffer(GL_ARRAY_BUFFER, textureBufferId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float2) * plane->texCoords.size(), &plane->texCoords[0], GL_STATIC_DRAW);
	}
}

bool ComponentImage::Update(float dt)
{
	glBindTexture(GL_TEXTURE_2D, 0); // Bindear Textura a Default
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, plane->vertexBufferId);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, plane->indexBufferId);

	ComponentTransform2D* cTransform = owner->GetComponent<ComponentTransform2D>();

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	/*float3 pos = { cTransform->position, App->camera->nearPlaneDistance + 0.1f };*/
	/*float3 size = {  (float)viewport[2], (float)viewport[3], 1.0f };*/
	float3 pos = { cTransform->position.x, cTransform->position.y, App->camera->nearPlaneDistance + 0.1f };
	float3 size = { cTransform->size.x, cTransform->size.y, 1.0f };

	float4x4 transform;
	transform = transform.FromTRS(pos, Quat::identity, size);

	if (this->textureBufferId)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, this->textureBufferId);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);
	}

	
	if (textureId != 0) glBindTexture(GL_TEXTURE_2D, textureId);
	else // Called once when created an object. Set white fallback as a default texture
	{ 
		glBindTexture(GL_TEXTURE_2D, App->textures->whiteFallback);
		SetTexture(App->textures->textures.at("WHITE_BALLBACK"));
	}

	glPushMatrix();
	glMultMatrixf(transform.Transposed().ptr());
	glDrawElements(GL_TRIANGLES, plane->numIndices, GL_UNSIGNED_INT, NULL);
	glPopMatrix();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//-- Buffers--//
	
	//-- Textures --//
	if (this->textureBufferId)
	{
		glBindBuffer(GL_TEXTURE_COORD_ARRAY, 0);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	}
	
	glBindTexture(GL_TEXTURE_2D, 0);
	//--Disables States--//
	glDisableClientState(GL_VERTEX_ARRAY);
	return true;
}

void ComponentImage::SetTexture(const TextureObject& texture)
{
	textureName = texture.name;
	textureId = texture.id;
	width = texture.width;
	height = texture.height;
}

void ComponentImage::OnGui()
{
	if (ImGui::CollapsingHeader("Image")) {
		ImGui::Text("Texture: ");
		ImGui::SameLine();
		if (textureId == 0) // Supposedly there is no textureId = 0 in textures array
		{
			ImGui::Text("None");
		}
		else
		{
			ImGui::Text(textureName.c_str());
			ImGui::Image((ImTextureID)textureId, ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
		}
			
	}
}

void ComponentImage::OnLoad(const JSONReader& reader)
{
	if (reader.HasMember("Texture name"))
	{
		const rapidjson::Value& itemTextureName = reader["Texture name"];
		textureName = itemTextureName.GetString();
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

void ComponentImage::OnSave(JSONWriter& writer) const
{
	writer.String("Image");
	writer.StartObject();
	writer.String("Texture name");
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
