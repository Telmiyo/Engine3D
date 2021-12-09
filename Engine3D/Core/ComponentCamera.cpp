#include "ComponentCamera.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "GameObject.h"
#include "ModuleEditor.h"

#include "glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

ComponentCamera::ComponentCamera(GameObject* parent) : Component(parent)
{
	LOG("Setting up the camera");

	aspectRatio = atan(verticalFOV / 2) / atan(horizontalFOV / 2);
	componentType = ComponentType::COMPONENT_CAMERA;

	//Set Fustum Parameters
	cameraFrustum.pos = owner->GetComponent<ComponentTransform>()->GetPosition();
	cameraFrustum.front = owner->GetComponent<ComponentTransform>()->Front().Normalized();
	cameraFrustum.up = owner->GetComponent<ComponentTransform>()->Up().Normalized();
	cameraFrustum.horizontalFov = horizontalFOV;
	cameraFrustum.verticalFov = verticalFOV;
	viewMatrix = cameraFrustum.ViewMatrix();

	
}

ComponentCamera::~ComponentCamera()
{
}

bool ComponentCamera::Update(float dt)
{


	CalculateViewMatrix();
	DrawCamera();
	return true;
}

void ComponentCamera::CalculateViewMatrix()
{
	
}

void ComponentCamera::DrawCamera()
{
	float3 cornerPoints[8];
	cameraFrustum.GetCornerPoints(cornerPoints);

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glLineWidth(3.5f);
	glBegin(GL_LINES);

	glVertex3f(cornerPoints[0].x, cornerPoints[0].y, cornerPoints[0].z);
	glVertex3f(cornerPoints[1].x, cornerPoints[1].y, cornerPoints[1].z);

	glVertex3f(cornerPoints[0].x, cornerPoints[0].y, cornerPoints[0].z);
	glVertex3f(cornerPoints[2].x, cornerPoints[2].y, cornerPoints[2].z);

	glVertex3f(cornerPoints[2].x, cornerPoints[2].y, cornerPoints[2].z);
	glVertex3f(cornerPoints[3].x, cornerPoints[3].y, cornerPoints[3].z);

	glVertex3f(cornerPoints[1].x, cornerPoints[1].y, cornerPoints[1].z);
	glVertex3f(cornerPoints[3].x, cornerPoints[3].y, cornerPoints[3].z);

	glVertex3f(cornerPoints[0].x, cornerPoints[0].y, cornerPoints[0].z);
	glVertex3f(cornerPoints[4].x, cornerPoints[4].y, cornerPoints[4].z);

	glVertex3f(cornerPoints[5].x, cornerPoints[5].y, cornerPoints[5].z);
	glVertex3f(cornerPoints[4].x, cornerPoints[4].y, cornerPoints[4].z);

	glVertex3f(cornerPoints[5].x, cornerPoints[5].y, cornerPoints[5].z);
	glVertex3f(cornerPoints[1].x, cornerPoints[1].y, cornerPoints[1].z);

	glVertex3f(cornerPoints[5].x, cornerPoints[5].y, cornerPoints[5].z);
	glVertex3f(cornerPoints[7].x, cornerPoints[7].y, cornerPoints[7].z);

	glVertex3f(cornerPoints[7].x, cornerPoints[7].y, cornerPoints[7].z);
	glVertex3f(cornerPoints[6].x, cornerPoints[6].y, cornerPoints[6].z);

	glVertex3f(cornerPoints[6].x, cornerPoints[6].y, cornerPoints[6].z);
	glVertex3f(cornerPoints[2].x, cornerPoints[2].y, cornerPoints[2].z);

	glVertex3f(cornerPoints[6].x, cornerPoints[6].y, cornerPoints[6].z);
	glVertex3f(cornerPoints[4].x, cornerPoints[4].y, cornerPoints[4].z);

	glVertex3f(cornerPoints[7].x, cornerPoints[7].y, cornerPoints[7].z);
	glVertex3f(cornerPoints[3].x, cornerPoints[3].y, cornerPoints[3].z);

	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glLineWidth(1.0f);

}

void ComponentCamera::OnGui()
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		ImGui::Text("AspectRatio %f", aspectRatio);
		ImGui::Text("VerticalFov %f", verticalFOV);
		ImGui::Text("HorizontalFov %f", horizontalFOV);
		ImGui::Text("Near Plane Distance %f", nearPlaneDistance);
		ImGui::Text("Far Plane Distance %f", farPlaneDistance);
		ImGui::Checkbox("Projection is Dirty", &projectionIsDirty);
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
