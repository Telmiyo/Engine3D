#include "ComponentText.h"

ComponentText::ComponentText(GameObject* parent) : Component(parent)
{
	componentType = ComponentType::COMPONENT_TEXT;

	text = textBuffer;
}

ComponentText::~ComponentText()
{

}

bool ComponentText::Update(float dt)
{
	return true;
}

void ComponentText::OnGui()
{
	if (ImGui::CollapsingHeader("Text")) 
	{
		
		// Just for debugging inside the engine
		if (text != nullptr)
		{
			ImGui::Text("DEBUG: ");
			ImGui::SameLine();
			ImGui::Text(text);
		}

		if (ImGui::InputTextWithHint("Input text", text, textBuffer, 400, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
		{
			text = textBuffer;
		}
	}
}

void ComponentText::OnLoad(const JSONReader& reader)
{
	// Loading input text
	if (reader.HasMember("Input text"))
	{
		const rapidjson::Value& itemText = reader["Input text"];
		text = itemText.GetString();
	}
}
void ComponentText::OnSave(JSONWriter& writer) const
{
	writer.String("Text");
	writer.StartObject();

	// Saving input text
	writer.String("Input text");
	writer.String(text);

	writer.EndObject();
}