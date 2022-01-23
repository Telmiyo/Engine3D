#include "ModuleUI.h"
#include "glew.h"

#include "GameObject.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "ModuleInput.h"

#include "ComponentTransform2D.h"
#include "ComponentCanvas.h"
#include "ComponentImage.h"
#include "ComponentCamera.h"
#include "ComponentWindow.h"

#include "ComponentMesh.h"

#include <queue>

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
	//GameObject* canvas = App->scene->CreateGameObjectByName("OptionsCanvas", App->scene->root, false);// WINDOW
	//ComponentCanvas* can = new ComponentCanvas(canvas);

	//GameObject* win = App->scene->CreateGameObjectByName("Window", nullptr, false);// WINDOW
	//ComponentTransform2D* transform = new ComponentTransform2D(win);
	//transform->SetSize({ 200.f, 185.f });
	//ComponentImage* defaultImage = new ComponentImage(win);
	////defaultImage->SetTexture(App->textures->Load(""))
	//ComponentWindow* window = new ComponentWindow(win);
	//
	//canvas->AttachChild(win);

	//GameObject* text = App->scene->CreateGameObjectByName("Text", nullptr, false); // TEXT
	//ComponentTransform2D *textTransform = new ComponentTransform2D(text);
	//textTransform->SetPosition({ -198.f, -117.f });
	//textTransform->SetSize({ 94.f, 103.f });
	//ComponentImage* textImage = new ComponentImage(text);
	//textImage->SetTexture(App->textures->Load("vsyinc.png"));
	//win->AttachChild(text);

	//GameObject* checkbox = App->scene->CreateGameObjectByName("Checkbox", nullptr, false); // TEXT
	//ComponentTransform2D *checkTransform = new ComponentTransform2D(checkbox);
	//checkTransform->SetPosition({ -180.f, -118.f });
	//checkTransform->SetSize({ 48.f, 89.f });
	//ComponentImage* checkboxImage = new ComponentImage(checkbox);
	//checkboxImage->SetTexture(App->textures->Load("button.png"));
	//win->AttachChild(checkbox);

	//optionsList.push_back(canvas);
	//App->scene->DeleteSelectedGameObject(canvas);
	//optionsList.push_back(win);
	//App->scene->DeleteSelectedGameObject(win);
	//optionsList.push_back(text);
	//App->scene->DeleteSelectedGameObject(text);
	//optionsList.push_back(checkbox);
	//App->scene->DeleteSelectedGameObject(checkbox);

	return true;

	//canvas = App->scene->CreateGameObjectByName("OptionsCanvas", nullptr, false);// WINDOW
	//ComponentCanvas* can = new ComponentCanvas(canvas);

	//win = App->scene->CreateGameObjectByName("Window", canvas, false);// WINDOW
	//ComponentTransform2D* transform = new ComponentTransform2D(win);
	//transform->SetPosition({ 0, 100.f });
	//transform->SetSize({ 200.f, 185.f });
	//ComponentImage* defaultImage = new ComponentImage(win);
	////defaultImage->SetTexture(App->textures->Load(""))
	//ComponentWindow* window = new ComponentWindow(win);
	//defaultImage->plane->GenerateBounds();
	//defaultImage->plane->UpdateBounds(defaultImage);


	//text = App->scene->CreateGameObjectByName("Text", win, false); // TEXT
	//ComponentTransform2D* textTransform = new ComponentTransform2D(text);
	//textTransform->SetPosition({ -114.f, -170.f });
	//textTransform->SetSize({ 94.f, 103.f });
	//ComponentImage* textImage = new ComponentImage(text);
	//textImage->SetTexture(App->textures->Load("vsync.png"));
	//textImage->plane->GenerateBounds();
	//textImage->plane->UpdateBounds(textImage);

	//checkbox = App->scene->CreateGameObjectByName("Checkbox", win, false); // TEXT
	//ComponentTransform2D* checkTransform = new ComponentTransform2D(checkbox);
	//checkTransform->SetPosition({ -261.f, -170.f });
	//checkTransform->SetSize({ 48.f, 89.f });
	//ComponentImage* checkboxImage = new ComponentImage(checkbox);
	//checkboxImage->SetTexture(App->textures->Load("button.png"));
	//checkboxImage->plane->GenerateBounds();
	//checkboxImage->plane->UpdateBounds(checkboxImage);

	return true;
}

update_status ModuleUI::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleUI::Update(float dt)
{
	win = App->scene->FindObjectByName("OptionsWindow");
	text = App->scene->FindObjectByName("VsyncText");
	checkbox = App->scene->FindObjectByName("OptionCheckbox");

	/*GLUquadric* quad = gluNewQuadric();
	glTranslatef(uiCamera->cameraFrustum.pos.x, uiCamera->cameraFrustum.pos.y, uiCamera->cameraFrustum.pos.z);
	glColor3f(1, 0, 0);
	gluSphere(quad, 2, 100, 20);*/
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		options = !options;

	if (win != nullptr)
		win->GetComponent<ComponentImage>()->imageColor.w = 1.0f;
	if (text != nullptr)
		text->GetComponent<ComponentImage>()->imageColor.w = 1.0f;
	if (checkbox != nullptr)
		checkbox->GetComponent<ComponentImage>()->imageColor.w = 1.0f;

	if (options)
	{
		/*	for (auto o : optionsList)
			{
				o->GetComponent<ComponentImage>()->imageColor.w = 0.0f;
			}*/
			//win->active = true;
			//text->active = true;
			//checkbox->active = true;
		if (win != nullptr)
			win->active = true;
		if (text != nullptr)
			text->active = true;
		if (checkbox != nullptr)
			checkbox->active = true;

	}
	else
	{
		//win->active = false;
		//text->active = false;
		//checkbox->active = false;
		if (win != nullptr)
			win->active = false;
		if (text != nullptr)
			text->active = false;
		if (checkbox != nullptr)
			checkbox->active = false;

	}

	if (fadeOut)
	{
		FadeOut();
	}

	float3 right = App->camera->right;
	float3 up = App->camera->up;
	float3 front = App->camera->front;
	float3 position = App->camera->position;

	App->camera->position = { 0, 0, 2 };
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

	uiCameraViewport[0] = viewport[0];
	uiCameraViewport[1] = viewport[1];
	uiCameraViewport[2] = viewport[2];
	uiCameraViewport[3] = viewport[3];

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
		if (child->active)
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
			if (child->active)
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

float2 ModuleUI::GetMousePosition()
{
	float mouseX = App->editor->onSceneMousePos.x;
	float mouseY = -(App->editor->onSceneMousePos.y - App->editor->lastViewportSize.y);

	return { mouseX, mouseY };
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

void ModuleUI::FadeOut()
{
	for (auto o : App->scene->gameObjectList)
	{

		if (o->GetComponent<ComponentTransform2D>() )
		{
			if (o->GetComponent<ComponentImage>() != nullptr) {
				if (o->GetComponent<ComponentImage>()->GetOpacity() <= 0)
				{
					fadeOut = false;
				}
				else {

					o->GetComponent<ComponentImage>()->SetOpacity(0.01f);
				}
			}

			if (win != nullptr)
				win->GetComponent<ComponentImage>()->imageColor.w = 1.0f;
			if (text != nullptr)
				text->GetComponent<ComponentImage>()->imageColor.w = 1.0f;
			if (checkbox != nullptr)
				checkbox->GetComponent<ComponentImage>()->imageColor.w = 1.0f;


		}

	}

}

void ModuleUI::FadeIn()
{
	for (auto o : App->scene->gameObjectList)
	{
		if (o->GetComponent<ComponentTransform2D>() != nullptr && o->GetComponent<ComponentImage>() != nullptr)
			o->GetComponent<ComponentImage>()->SetOpacity(-0.01f);

	}
}
