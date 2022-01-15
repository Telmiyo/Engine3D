#include "ModuleUI.h"
#include "glew.h"

#include "GameObject.h"
#include <queue>
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleCamera3D.h"

#include "ComponentTransform2D.h"

#include "ComponentCamera.h"

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleUI::~ModuleUI()
{
}

bool ModuleUI::Init()
{
	return true;
}

bool ModuleUI::Start()
{
	/*uiCamera = new ComponentCamera(nullptr);
	
	uiCamera->cameraFrustum.pos = float3(0.f, 30.f, 0.f);
	uiCamera->cameraFrustum.type = PerspectiveFrustum;
	uiCamera->cameraFrustum.verticalFov = uiCamera->horizontalFOV;
	uiCamera->cameraFrustum.horizontalFov = uiCamera->verticalFOV;
	uiCamera->cameraFrustum.nearPlaneDistance = uiCamera->nearPlaneDistance;
	uiCamera->cameraFrustum.farPlaneDistance = uiCamera->farPlaneDistance;
	uiCamera->LookAt({ 0.f, 0.f, 0.f });
	uiCamera->projectionIsDirty = true;

	uiCamera->CalculateViewMatrix(uiCamera->cameraFrustum.pos, uiCamera->cameraFrustum.front, uiCamera->cameraFrustum.up);
	*/
	//uiCamera->viewMatrix = uiCamera->cameraFrustum.ViewMatrix();

	//uiCamera->CalculateViewMatrix();

	return true;
}

update_status ModuleUI::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleUI::Update(float dt)
{
	/*GLUquadric* quad = gluNewQuadric();
	glTranslatef(uiCamera->cameraFrustum.pos.x, uiCamera->cameraFrustum.pos.y, uiCamera->cameraFrustum.pos.z);
	glColor3f(1, 0, 0);
	gluSphere(quad, 2, 100, 20);*/

	float3 right = App->camera->right;
	float3 up = App->camera->up;
	float3 front = App->camera->front;
	float3 position = App->camera->position;

	App->camera->position = { 0, 0, 40 };
	App->camera->LookAt({ 0, 0, 0 });

	App->camera->projectionIsDirty = true;
	App->camera->CalculateViewMatrix();

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(App->camera->cameraFrustum.ProjectionMatrix().Transposed().ptr());
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->viewMatrix.Transposed().ptr());

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(viewport[0], viewport[2], viewport[1], viewport[3], 1, 1000);

	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	std::queue<GameObject*> S;
	for (GameObject* child : App->scene->root->children)
	{
		S.push(child);
	}

	while (!S.empty())
	{
		GameObject* go = S.front();
		if (go->GetComponent<ComponentTransform2D>() != nullptr) {
			go->Update(dt);
		}
		S.pop();
		for (GameObject* child : go->children)
		{
			S.push(child);
		}
	}

	glPopAttrib();

	glPopMatrix();
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	App->camera->right = right;
	App->camera->up = up;
	App->camera->front = front;
	App->camera->position = position;

	App->camera->projectionIsDirty = true;
	App->camera->CalculateViewMatrix();

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(App->camera->cameraFrustum.ProjectionMatrix().Transposed().ptr());
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->viewMatrix.Transposed().ptr());

	return UPDATE_CONTINUE;
}

update_status ModuleUI::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleUI::CleanUp()
{
	return true;
}

void ModuleUI::OnGui()
{
}

void ModuleUI::OnLoad(const JSONReader& reader)
{
}

void ModuleUI::OnSave(JSONWriter& writer) const
{
}
