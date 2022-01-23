#include "ComponentCheckbox.h"

#include "Application.h"
#include "ModuleInput.h"

ComponentCheckbox::ComponentCheckbox(GameObject* parent) : Component(parent)
{
	componentType = ComponentType::COMPONENT_BUTTON;

	callback = [](bool checked) {
		//LOG("%s", checked ? "true" : "false");
	};
}

ComponentCheckbox::~ComponentCheckbox()
{

}

bool ComponentCheckbox::Update(float dt)
{
	ComponentImage* image = owner->GetComponent<ComponentImage>();

	if (checked)
		image->SetTexture(checkedTexture);
	else
		image->SetTexture(uncheckedTexture);

	ComponentTransform2D* tmp = owner->GetComponent<ComponentTransform2D>();

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
	{
		// Button pressed
		if (tmp->CheckMouseInsideBounds()) {
			checked = !checked;
			callback(checked);
		}
	}

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

		ImGui::Text("Checked texture");
		ImGui::SameLine();
		if (checkedTexture.id == 0) // Supposedly there is no textureId = 0 in textures array
		{
			ImGui::Text("None");
		}
		else
		{
			ImGui::Text(checkedTexture.name.c_str());
			ImGui::Image((ImTextureID)checkedTexture.id, ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
		}

		ImGui::Text("Unchecked texture");
		ImGui::SameLine();
		if (uncheckedTexture.id == 0) // Supposedly there is no textureId = 0 in textures array
		{
			ImGui::Text("None");
		}
		else
		{
			ImGui::Text(uncheckedTexture.name.c_str());
			ImGui::Image((ImTextureID)uncheckedTexture.id, ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
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

void ComponentCheckbox::SetCheckedTexture(TextureObject id)
{
	checkedTexture = id;
}

void ComponentCheckbox::SetUncheckedTexture(TextureObject id)
{
	uncheckedTexture = id;
}
