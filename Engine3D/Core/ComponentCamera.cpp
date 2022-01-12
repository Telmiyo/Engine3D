#include "ComponentCamera.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "GameObject.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleScene.h"

#include "glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

ComponentCamera::ComponentCamera(GameObject* parent) : Component(parent)
{
	LOG("Setting up the camera");

	componentType = ComponentType::COMPONENT_CAMERA;

	cameraFrustum.type = FrustumType::PerspectiveFrustum;
	if (parent) {
		cameraFrustum.pos = parent->GetComponent<ComponentTransform>()->GetPosition();
		cameraFrustum.front = parent->GetComponent<ComponentTransform>()->Front().Normalized();
		cameraFrustum.up = parent->GetComponent<ComponentTransform>()->Up().Normalized();

		cameraFrustum.horizontalFov = horizontalFOV;
		cameraFrustum.verticalFov = verticalFOV;
		cameraFrustum.nearPlaneDistance = nearPlaneDistance;
		cameraFrustum.farPlaneDistance = farPlaneDistance;
	}
}

ComponentCamera::~ComponentCamera()
{
}

bool ComponentCamera::Update(float dt)
{
	CheckObjects();
	DrawCamera();
	return true;
}

void ComponentCamera::CalculateViewMatrix(float3 pos, float3 front, float3 up)
{
	if (projectionIsDirty)
		RecalculateProjection();

	if (owner != nullptr) {
		cameraFrustum.pos = owner->GetComponent<ComponentTransform>()->GetPosition();
		cameraFrustum.front = owner->GetComponent<ComponentTransform>()->Front().Normalized();
		cameraFrustum.up = owner->GetComponent<ComponentTransform>()->Up().Normalized();
	}
	else {
		cameraFrustum.pos = pos;
		cameraFrustum.front = front;
		cameraFrustum.up = up;
	}

	viewMatrix = cameraFrustum.ViewMatrix();
}

void ComponentCamera::RecalculateProjection()
{
	cameraFrustum.type = FrustumType::PerspectiveFrustum;
	cameraFrustum.nearPlaneDistance = nearPlaneDistance;
	cameraFrustum.farPlaneDistance = farPlaneDistance;
	cameraFrustum.horizontalFov = horizontalFOV;
	UpdateVerticalFov();
	UpdateHorizontalFov();
	cameraFrustum.verticalFov = verticalFOV;
	projectionIsDirty = false;
}

void ComponentCamera::SetHorizontalFov(float value)
{
	horizontalFOV = value;
	projectionIsDirty = true;
}

void ComponentCamera::SetVerticalFov(float value)
{
	verticalFOV = value;
	projectionIsDirty = true;
}

void ComponentCamera::SetNearPlaneDistance(float value)
{
	nearPlaneDistance = value;
	projectionIsDirty = true;
}

void ComponentCamera::SetFarPlaneDistance(float value)
{
	farPlaneDistance = value;
	projectionIsDirty = true;
}

void ComponentCamera::UpdateAspectRatio()
{
	aspectRatio = tan(verticalFOV / 2) / tan(horizontalFOV / 2);
}

void ComponentCamera::UpdateVerticalFov()
{
	verticalFOV = 2 * atan(tan(horizontalFOV / 2) * 1 / aspectRatio);

}

void ComponentCamera::LookAt(const float3& position)
{
	float3 v = position - cameraFrustum.pos;

	float3x3 m = float3x3::LookAt(cameraFrustum.front, v.Normalized(), cameraFrustum.up, float3::unitY);

	cameraFrustum.front = m.MulDir(cameraFrustum.front).Normalized();
	cameraFrustum.up = m.MulDir(cameraFrustum.up).Normalized();
}

void ComponentCamera::CheckObjects()
{
	//BY POSITION
	/*for (auto o : App->scene->gameObjectList)
	{
		if (o->GetComponent<ComponentMesh>() != nullptr)
		{
			bool status = cameraFrustum.Contains(o->GetComponent<ComponentTransform>()->GetPosition());
			if (status)
			{
				o->GetComponent<ComponentMesh>()->render = true;
			}
			else
			{
				o->GetComponent<ComponentMesh>()->render = false;

			}

		}

	}*/
	//BY AABB
	for (auto o : App->scene->gameObjectList)
	{
		if (o->GetComponent<ComponentMesh>() != nullptr)
		{
			float3 corners[8];
			o->GetComponent<ComponentMesh>()->localAABB.GetCornerPoints(corners);
			for (float3 corner : corners)
			{
				bool status = cameraFrustum.Contains(corner);
				if (status)
				{
					o->GetComponent<ComponentMesh>()->render = true;
					break;
				}
				else
				{
					o->GetComponent<ComponentMesh>()->render = false;

				}

			}

		}

	}

}

void ComponentCamera::UpdateHorizontalFov()
{
	horizontalFOV = 2 * atan(tan(verticalFOV / 2) * aspectRatio);
}

void ComponentCamera::DrawCamera()
{
	/// Finds a ray in world space that originates at the eye point and looks in the given direction inside the frustum.
		/** The (x,y) coordinate specifies the normalized viewport coordinate through which the ray passes.
			Both x and y must be in the range [-1,1].
			Specifying (-1, -1) returns the bottom-left corner of the near plane.
			The point (1, 1) corresponds to the top-right corner of the near plane. */
	Ray bottomLeft = cameraFrustum.UnProject(-1, -1);
	Ray bottomRight = cameraFrustum.UnProject(-1, 1);
	Ray topLeft = cameraFrustum.UnProject(1, 1);
	Ray topRigth = cameraFrustum.UnProject(1, -1);

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glLineWidth(3.5f);
	glBegin(GL_LINES);

	//Pos: The origin of the Ray
	//BOTTOM LEFT POINT: the origin point + dir * nearPlaneDistance. x -; y:-;z:+
	//NEAR
	glVertex3f(bottomLeft.pos.x, bottomLeft.pos.y, bottomLeft.pos.z);
	glVertex3f(bottomLeft.pos.x + bottomLeft.dir.x * nearPlaneDistance, bottomLeft.pos.y + bottomLeft.dir.y * nearPlaneDistance, bottomLeft.pos.z + bottomLeft.dir.z * nearPlaneDistance);

	glVertex3f(bottomRight.pos.x, bottomRight.pos.y, bottomRight.pos.z);
	glVertex3f(bottomRight.pos.x + bottomRight.dir.x * nearPlaneDistance, bottomRight.pos.y + bottomRight.dir.y * nearPlaneDistance, bottomRight.pos.z + bottomRight.dir.z * nearPlaneDistance);

	glVertex3f(topLeft.pos.x, topLeft.pos.y, topLeft.pos.z);
	glVertex3f(topLeft.pos.x + topLeft.dir.x * nearPlaneDistance, topLeft.pos.y + topLeft.dir.y * nearPlaneDistance, topLeft.pos.z + topLeft.dir.z * nearPlaneDistance);

	glVertex3f(topRigth.pos.x, topRigth.pos.y, topRigth.pos.z);
	glVertex3f(topRigth.pos.x + topRigth.dir.x * nearPlaneDistance, topRigth.pos.y + topRigth.dir.y * nearPlaneDistance, topRigth.pos.z + topRigth.dir.z * nearPlaneDistance);

	//NEAR PLANE
	glVertex3f(bottomLeft.pos.x + bottomLeft.dir.x * nearPlaneDistance, bottomLeft.pos.y + bottomLeft.dir.y * nearPlaneDistance, bottomLeft.pos.z + bottomLeft.dir.z * nearPlaneDistance);
	glVertex3f(bottomRight.pos.x + bottomRight.dir.x * nearPlaneDistance, bottomRight.pos.y + bottomRight.dir.y * nearPlaneDistance, bottomRight.pos.z + bottomRight.dir.z * nearPlaneDistance);

	glVertex3f(bottomLeft.pos.x + bottomLeft.dir.x * nearPlaneDistance, bottomLeft.pos.y + bottomLeft.dir.y * nearPlaneDistance, bottomLeft.pos.z + bottomLeft.dir.z * nearPlaneDistance);
	glVertex3f(topRigth.pos.x + topRigth.dir.x * nearPlaneDistance, topRigth.pos.y + topRigth.dir.y * nearPlaneDistance, topRigth.pos.z + topRigth.dir.z * nearPlaneDistance);

	glVertex3f(bottomRight.pos.x + bottomRight.dir.x * nearPlaneDistance, bottomRight.pos.y + bottomRight.dir.y * nearPlaneDistance, bottomRight.pos.z + bottomRight.dir.z * nearPlaneDistance);
	glVertex3f(topLeft.pos.x + topLeft.dir.x * nearPlaneDistance, topLeft.pos.y + topLeft.dir.y * nearPlaneDistance, topLeft.pos.z + topLeft.dir.z * nearPlaneDistance);

	glVertex3f(topLeft.pos.x + topLeft.dir.x * nearPlaneDistance, topLeft.pos.y + topLeft.dir.y * nearPlaneDistance, topLeft.pos.z + topLeft.dir.z * nearPlaneDistance);
	glVertex3f(topRigth.pos.x + topRigth.dir.x * nearPlaneDistance, topRigth.pos.y + topRigth.dir.y * nearPlaneDistance, topRigth.pos.z + topRigth.dir.z * nearPlaneDistance);



	glColor4f(1.0f, 0.0f, 1.0f, 1.0f);

	//FAR
	glVertex3f(bottomLeft.pos.x, bottomLeft.pos.y, bottomLeft.pos.z);
	glVertex3f(bottomLeft.pos.x + bottomLeft.dir.x * farPlaneDistance, bottomLeft.pos.y + bottomLeft.dir.y * farPlaneDistance, bottomLeft.pos.z + bottomLeft.dir.z * farPlaneDistance);

	glVertex3f(bottomRight.pos.x, bottomRight.pos.y, bottomRight.pos.z);
	glVertex3f(bottomRight.pos.x + bottomRight.dir.x * farPlaneDistance, bottomRight.pos.y + bottomRight.dir.y * farPlaneDistance, bottomRight.pos.z + bottomRight.dir.z * farPlaneDistance);

	glVertex3f(topLeft.pos.x, topLeft.pos.y, topLeft.pos.z);
	glVertex3f(topLeft.pos.x + topLeft.dir.x * farPlaneDistance, topLeft.pos.y + topLeft.dir.y * farPlaneDistance, topLeft.pos.z + topLeft.dir.z * farPlaneDistance);

	glVertex3f(topRigth.pos.x, topRigth.pos.y, topRigth.pos.z);
	glVertex3f(topRigth.pos.x + topRigth.dir.x * farPlaneDistance, topRigth.pos.y + topRigth.dir.y * farPlaneDistance, topRigth.pos.z + topRigth.dir.z * farPlaneDistance);

	//Far PLANE
	glVertex3f(bottomLeft.pos.x + bottomLeft.dir.x * farPlaneDistance, bottomLeft.pos.y + bottomLeft.dir.y * farPlaneDistance, bottomLeft.pos.z + bottomLeft.dir.z * farPlaneDistance);
	glVertex3f(bottomRight.pos.x + bottomRight.dir.x * farPlaneDistance, bottomRight.pos.y + bottomRight.dir.y * farPlaneDistance, bottomRight.pos.z + bottomRight.dir.z * farPlaneDistance);

	glVertex3f(bottomLeft.pos.x + bottomLeft.dir.x * farPlaneDistance, bottomLeft.pos.y + bottomLeft.dir.y * farPlaneDistance, bottomLeft.pos.z + bottomLeft.dir.z * farPlaneDistance);
	glVertex3f(topRigth.pos.x + topRigth.dir.x * farPlaneDistance, topRigth.pos.y + topRigth.dir.y * farPlaneDistance, topRigth.pos.z + topRigth.dir.z * farPlaneDistance);

	glVertex3f(bottomRight.pos.x + bottomRight.dir.x * farPlaneDistance, bottomRight.pos.y + bottomRight.dir.y * farPlaneDistance, bottomRight.pos.z + bottomRight.dir.z * farPlaneDistance);
	glVertex3f(topLeft.pos.x + topLeft.dir.x * farPlaneDistance, topLeft.pos.y + topLeft.dir.y * farPlaneDistance, topLeft.pos.z + topLeft.dir.z * farPlaneDistance);

	glVertex3f(topLeft.pos.x + topLeft.dir.x * farPlaneDistance, topLeft.pos.y + topLeft.dir.y * farPlaneDistance, topLeft.pos.z + topLeft.dir.z * farPlaneDistance);
	glVertex3f(topRigth.pos.x + topRigth.dir.x * farPlaneDistance, topRigth.pos.y + topRigth.dir.y * farPlaneDistance, topRigth.pos.z + topRigth.dir.z * farPlaneDistance);


	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glLineWidth(1.0f);

}

void ComponentCamera::OnGui()
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		ImGui::Text("AspectRatio %f", aspectRatio);

		if (ImGui::SliderFloat("FieldOfView", &horizontalFOVGui, 1.f, 110.f))
		{
			SetHorizontalFov(DegToRad(horizontalFOVGui));
		}
		if (ImGui::SliderFloat("NearPlaneDistance", &nearPlaneDistanceGui, 1.f, farPlaneDistance))
		{
			SetNearPlaneDistance(nearPlaneDistanceGui);
		}
		if (ImGui::SliderFloat("FarPlaneDistance", &farPlaneDistanceGui, nearPlaneDistance, 10000.f))
		{
			SetFarPlaneDistance(farPlaneDistanceGui);
		}
	}

}

void ComponentCamera::OnLoad(const JSONReader& reader)
{
	if (reader.HasMember("camera"))
	{
		/*const auto& config = reader["camera"];
		LOAD_JSON_FLOAT(verticalFOV);
		LOAD_JSON_FLOAT(nearPlaneDistance);
		LOAD_JSON_FLOAT(farPlaneDistance);
		LOAD_JSON_FLOAT(cameraSpeed);
		LOAD_JSON_FLOAT(cameraSensitivity);*/
	}
	//	RecalculateProjection();
}

void ComponentCamera::OnSave(JSONWriter& writer) const
{
	/*writer.String("camera");
	writer.StartObject();
	SAVE_JSON_FLOAT(verticalFOV)
		SAVE_JSON_FLOAT(nearPlaneDistance)
		SAVE_JSON_FLOAT(farPlaneDistance)
		SAVE_JSON_FLOAT(cameraSpeed)
		SAVE_JSON_FLOAT(cameraSensitivity)
		writer.EndObject();*/
}
