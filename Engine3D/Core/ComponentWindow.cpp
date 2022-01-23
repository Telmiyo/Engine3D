#include "ComponentWindow.h"
#include "Application.h"
#include "ModuleInput.h"

ComponentWindow::ComponentWindow(GameObject* parent) : Component(parent)
{
}

ComponentWindow::~ComponentWindow()
{
}

bool ComponentWindow::Update(float dt)
{
	ComponentTransform2D* tmp = owner->GetComponent<ComponentTransform2D>();
	
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		// Button holding press
		if (tmp->CheckMouseInsideBounds()) {
			tmp->position.x += App->input->GetMouseXMotion() * smoothness;
			tmp->position.y -= App->input->GetMouseYMotion() * smoothness;
		}
	}
	return true;
}

void ComponentWindow::OnGui()
{
	if (ImGui::CollapsingHeader("Options Window")) {
		float newSmoothness = smoothness;
		if (ImGui::InputFloat("Smoothness", &newSmoothness))
		{
			smoothness = newSmoothness;
		}
	}
}

void ComponentWindow::OnLoad(const JSONReader& reader)
{
}

void ComponentWindow::OnSave(JSONWriter& writer) const
{
}
