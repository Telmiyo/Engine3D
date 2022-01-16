#include "ComponentButton.h"

ComponentButton::ComponentButton(GameObject* parent) : Component(parent)
{
	componentType = ComponentType::COMPONENT_BUTTON;
	buttonState = ButtonState::DISABLED;
}

ComponentButton::~ComponentButton()
{

}

bool ComponentButton::Update(float dt)
{
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
