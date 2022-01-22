#include "ComponentCheckbox.h"

ComponentCheckbox::ComponentCheckbox(GameObject* parent) : Component(parent)
{
	componentType = ComponentType::COMPONENT_BUTTON;
}

ComponentCheckbox::~ComponentCheckbox()
{

}

bool ComponentCheckbox::Update(float dt)
{
	return true;
}

void ComponentCheckbox::OnGui()
{
	if (ImGui::CollapsingHeader("Checkbox")) {
		bool newCheckboxState = checked;
		if (ImGui::Checkbox("###Checkbox", &newCheckboxState))
		{
			checked = newCheckboxState;
		}
	}
}

void ComponentCheckbox::OnLoad(const JSONReader& reader)
{
	// Loading button state
	if (reader.HasMember("Checkbox state"))
	{
		const rapidjson::Value& itemCheckboxState = reader["Checkbox state"];
		checked = itemCheckboxState.GetBool();
	}
}
void ComponentCheckbox::OnSave(JSONWriter& writer) const
{
	writer.String("Checkbox");
	writer.StartObject();

	// Saving button state
	writer.String("Checkbox state");
	writer.Bool(checked);

	writer.EndObject();
}