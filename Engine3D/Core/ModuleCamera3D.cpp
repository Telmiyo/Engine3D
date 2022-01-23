#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "GameObject.h"
#include "ModuleScene.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{

	right = float3(1.0f, 0.0f, 0.0f);
	up = float3(0.0f, 1.0f, 0.0f);
	front = float3(0.0f, 0.0f, 1.0f);

	position = float3(0.0f, 5.0f, -15.0f);
	reference = float3(0.0f, 0.0f, 0.0f);
	
	CalculateViewMatrix();

}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");

	LookAt(float3::zero);

	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	if (App->editor->isMouseOnScene())
	{
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			GameObject* gObj = GetGameObjectMousePicked();
			if (gObj != nullptr)
			{
				App->editor->gameobjectSelected = gObj;
			}
			else
			{
				// App->editor->gameobjectSelected = nullptr;
			}
		}
			

		float3 newPos(0, 0, 0);
		float speed = cameraSpeed * dt;
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed *= 4.f;

		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)
			newPos.y += speed;
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
			newPos.y -= speed;

		//Focus
		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			if (App->editor->gameobjectSelected != nullptr)
			{
				if (ComponentMesh* mesh = App->editor->gameobjectSelected->GetComponent<ComponentMesh>())
				{
					const float3 meshCenter = mesh->GetCenterPointInWorldCoords();
					LookAt(meshCenter);
					const float meshRadius = mesh->GetSphereRadius();
					const float currentDistance = meshCenter.Distance(position);
					const float desiredDistance = (meshRadius * 2) / atan(cameraFrustum.horizontalFov);
					position = position + front * (currentDistance - desiredDistance);
				}
				else
				{
					LookAt(App->editor->gameobjectSelected->transform->GetPosition());
				}
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			newPos += front * speed;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			newPos -= front * speed;


		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			newPos += right * speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			newPos -= right * speed;

		if (App->input->GetMouseZ() > 0)
			newPos += front * speed;
		if (App->input->GetMouseZ() < 0)
			newPos -= front * speed;

		position += newPos;

		// Mouse motion ----------------

		bool hasRotated = false;

		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) {
				if (App->editor->gameobjectSelected != nullptr)
				{
					const float newDeltaX = (float)dx * cameraSensitivity;
					const float newDeltaY = (float)dy * cameraSensitivity;

					reference = App->editor->gameobjectSelected->transform->GetPosition();
					Quat orbitMat = Quat::RotateY(newDeltaX * .1f);

					if (abs(up.y) < 0.3f) // Avoid gimball lock on up & down apex
					{
						if (position.y > reference.y && newDeltaY < 0.f)
							orbitMat = orbitMat * math::Quat::RotateAxisAngle(right, newDeltaY * .1f);
						if (position.y < reference.y && newDeltaY > 0.f)
							orbitMat = orbitMat * math::Quat::RotateAxisAngle(right, newDeltaY * .1f);
					}
					else
					{
						orbitMat = orbitMat * math::Quat::RotateAxisAngle(right, newDeltaY * .1f);
					}

					position = orbitMat * (position - reference) + reference;

					CalculateViewMatrix();
					LookAt(reference);
				}
			}
			else
			{

				if (dx != 0)
				{
					const float newDeltaX = (float)dx * cameraSensitivity;
					float deltaX = newDeltaX + 0.95f * (lastDeltaX - newDeltaX); //lerp for smooth rotation acceleration to avoid jittering
					lastDeltaX = deltaX;
					Quat rotateY = Quat::RotateY(up.y >= 0.f ? deltaX * .1f : -deltaX * .1f);
					up = rotateY * up;
					front = rotateY * front;
					CalculateViewMatrix();
					hasRotated = true;
				}

				if (dy != 0)
				{
					const float newDeltaY = (float)dy * cameraSensitivity;
					float deltaY = newDeltaY + 0.95f * (lastDeltaY - newDeltaY); //lerp for smooth rotation acceleration to avoid jittering
					lastDeltaY = deltaY;
					Quat rotateX = Quat::RotateAxisAngle(right, -deltaY * .1f);
					up = rotateX * up;
					front = rotateX * front;
					CalculateViewMatrix();
					hasRotated = true;
				}
			}
		}

		!hasRotated ? lastDeltaX = lastDeltaY = 0.f : 0.f;
	}
	return UPDATE_CONTINUE;
}


// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const float3& point)
{		
	reference = point;

	front = (reference - position).Normalized();
	right = float3(0.0f, 1.0f, 0.0f).Cross(front).Normalized();
	up = front.Cross(right);

	CalculateViewMatrix();
}



// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	if (projectionIsDirty)
		RecalculateProjection();

	cameraFrustum.pos = position;
	cameraFrustum.front = front.Normalized();
	cameraFrustum.up = up.Normalized();
	float3::Orthonormalize(cameraFrustum.front, cameraFrustum.up);
	right = up.Cross(front);
	viewMatrix = cameraFrustum.ViewMatrix();
}

void ModuleCamera3D::RecalculateProjection()
{
	cameraFrustum.type = FrustumType::PerspectiveFrustum;
	cameraFrustum.nearPlaneDistance = nearPlaneDistance;
	cameraFrustum.farPlaneDistance = farPlaneDistance;
	cameraFrustum.verticalFov = (verticalFOV * 3.141592 / 2) / 180.f;
	cameraFrustum.horizontalFov = 2.f * atanf(tanf(cameraFrustum.verticalFov * 0.5f) * aspectRatio);
}

void ModuleCamera3D::OnGui()
{
	if (ImGui::CollapsingHeader("Editor Camera"))
	{
		if (ImGui::DragFloat("Vertical fov", &verticalFOV))
		{
			projectionIsDirty = true;
		}
		if (ImGui::DragFloat("Near plane distance", &nearPlaneDistance))
		{
			projectionIsDirty = true;
		}
		if (ImGui::DragFloat("Far plane distance", &farPlaneDistance))
		{
			projectionIsDirty = true;
		}
	}
}

void ModuleCamera3D::OnSave(JSONWriter& writer) const
{
	writer.String("camera");	
	writer.StartObject();
	SAVE_JSON_FLOAT(verticalFOV)
	SAVE_JSON_FLOAT(nearPlaneDistance)
	SAVE_JSON_FLOAT(farPlaneDistance)
	SAVE_JSON_FLOAT(cameraSpeed)
	SAVE_JSON_FLOAT(cameraSensitivity)
	writer.EndObject();
}

void ModuleCamera3D::OnLoad(const JSONReader& reader)
{
	if (reader.HasMember("camera"))
	{
		const auto& config = reader["camera"];
		LOAD_JSON_FLOAT(verticalFOV);
		LOAD_JSON_FLOAT(nearPlaneDistance);
		LOAD_JSON_FLOAT(farPlaneDistance);
		LOAD_JSON_FLOAT(cameraSpeed);
		LOAD_JSON_FLOAT(cameraSensitivity);
	}
	RecalculateProjection();
}

GameObject* ModuleCamera3D::GetGameObjectMousePicked()
{
	float normalized_x = -1.0 + 2.0 * App->editor->onSceneMousePos.x / App->editor->lastViewportSize.x;
	float normalized_y = 1.0 - 2.0 * App->editor->onSceneMousePos.y / App->editor->lastViewportSize.y;

	 LineSegment picking = cameraFrustum.UnProjectLineSegment(normalized_x, normalized_y);

	for (unsigned int i = 0; i < App->scene->gameObjectList.size(); ++i)
	{
		// If game object has mesh
		if (ComponentMesh* mesh = App->scene->gameObjectList[i]->GetComponent<ComponentMesh>())
		{
			//bool hit;
			bool hit = picking.Intersects(mesh->localAABB); // ray vs. AABB
			if (hit)
			{
				hitList.push_back(App->scene->gameObjectList[i]);

				return App->scene->gameObjectList[i];

			}
		}
		// If game object is a camera
		if (ComponentCamera* camera = App->scene->gameObjectList[i]->GetComponent<ComponentCamera>())
		{

		}

		// First we test against all AABBs
		// We should end with a list of hits ordered by distance
		// On the closest one, using the vertex, we test the ray against all mesh triangles
		// Do not stop on the first hit, test alland keep the closest to ray origin
		// bool hit = my_ray.Intersects(game_object->aabb); // ray vs. AABB
		// bool hit = ray_local_space.Intersects(tri, &distance, &hit_point); // ray vs. triangle
		// You will need to construct one triangle at a time from the vertex list
		// Remember : Its coordinates are in local space!
		// And our ray coordinates are in global space ...
		// You have two choices :
		//		Transform each triangle to global space to test for ray intersection
		//		Transform once the ray into Game Object space to test against all triangles
	}

	return nullptr;
}

float2 ModuleCamera3D::WorldToScreen(float3 pos)
{
	float4 clipSpacePos = cameraFrustum.ProjectionMatrix() * (cameraFrustum.ViewMatrix() * float4(pos, 1.0));

	float3 ndcSpacePos;
	ndcSpacePos.x = clipSpacePos.x / clipSpacePos.w;
	ndcSpacePos.y = clipSpacePos.y / clipSpacePos.w;
	ndcSpacePos.z = clipSpacePos.z / clipSpacePos.w;

	float2 viewSize = { App->editor->GetSceneSize().x, App->editor->GetSceneSize().y };
	float2 viewOffset = { 0, 0 };

	float2 windowSpacePos;
	windowSpacePos.x = ((ndcSpacePos.x + 1.0) / 2.0) * viewSize.x + viewOffset.x;
	windowSpacePos.y = ((ndcSpacePos.y + 1.0) / 2.0) * viewSize.y + viewOffset.y;

	return windowSpacePos;
}
