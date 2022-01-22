#include "ComponentButton.h"

#include "Application.h"
#include "ModuleInput.h"
#include "Globals.h"

ComponentButton::ComponentButton(GameObject* parent) : Component(parent)
{
	componentType = ComponentType::COMPONENT_BUTTON;
	buttonState = ButtonState::DISABLED;

	callback = []() {
		LOG("HELLO");
	};
}

ComponentButton::~ComponentButton()
{

}

bool ComponentButton::Update(float dt)
{
	ComponentTransform2D* tmp = owner->GetComponent<ComponentTransform2D>();
	float2 pos;
	float2 size;
	tmp->GetBoundingBox(pos, size);

	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(pos.x, pos.y + size.y, 0.f);
	glVertex3f(pos.x + size.x, pos.y + size.y, 0.f);
	
	glVertex3f(pos.x + size.x, pos.y + size.y, 0.f);
	glVertex3f(pos.x + size.x, pos.y,0.f);

	glVertex3f(pos.x + size.x, pos.y, 0.f);
	glVertex3f(pos.x, pos.y , 0.f);

	glVertex3f(pos.x, pos.y, 0.f);
	glVertex3f(pos.x, pos.y + size.y, 0.f);

	glEnd();

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		// Button holding press
		if (tmp->CheckMouseInsideBounds()) {

		}
	}
	else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
	{
		// Button pressed
		if (tmp->CheckMouseInsideBounds()) {
			callback();
		}
	}
	//else if (CheckMouseInsideBounds()) {
		// Hover
	//}

	return true;
}

void ComponentButton::OnGui()
{
	if (ImGui::CollapsingHeader("Button")) {
		int newButtonState = (int)buttonState;
		if (ImGui::Combo("Button state", &newButtonState, "DISABLED\0IDLE\FOCUSED\0PRESSED\0"))
		{
			buttonState = (ButtonState)newButtonState;
		}
	}
}

void ComponentButton::OnLoad(const JSONReader& reader)
{
	// Loading button state
	if (reader.HasMember("Button state"))
	{
		const rapidjson::Value& itemButtonState = reader["Button state"];
		buttonState = (ButtonState)itemButtonState.GetInt();
	}
}
void ComponentButton::OnSave(JSONWriter& writer) const
{
	writer.String("Button");
	writer.StartObject();

	// Saving button state
	writer.String("Button state");
	writer.Int((int)buttonState);

	writer.EndObject();
}
