#include "ComponentImage.h"

#include "ImGui/imgui.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleCamera3D.h"
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
}

bool ComponentImage::Update(float dt)
{
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

	glPushMatrix();
	glMultMatrixf(transform.Transposed().ptr());
	glDrawElements(GL_TRIANGLES, plane->numIndices, GL_UNSIGNED_INT, NULL);
	glPopMatrix();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableClientState(GL_VERTEX_ARRAY);

	return true;
}

void ComponentImage::OnGui()
{
	if (ImGui::CollapsingHeader("Image")) {
		ImGui::Text("Texture");
		ImGui::Image((ImTextureID)App->textures->checkers, ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0));
	}
}
