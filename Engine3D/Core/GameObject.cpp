#include "GameObject.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleFileSystem.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform2D.h"
#include "ComponentImage.h"
#include "ComponentButton.h"
#include "ComponentCheckbox.h"
#include "ComponentText.h"
#include "ComponentWindow.h"

#include "ImGui/imgui.h"

GameObject::GameObject(bool is3D) {

	name = name + ("GameObject");
	//parent = nullptr;

	if (is3D)
	{
		transform = CreateComponent<ComponentTransform>();
		transform2d = nullptr;
	}

	else
	{
		transform = nullptr;
		transform2d = CreateComponent<ComponentTransform2D>();
	}

	active = true;
}

GameObject::GameObject(const std::string name, bool is3D) : name(name)
{
	if (is3D)
	{
		transform = CreateComponent<ComponentTransform>();
		transform2d = nullptr;
	}
	
	active = true;
}

GameObject::GameObject(const char* name)
{
	this->name = name;

	active = true;
}


GameObject::~GameObject() {

	for (size_t i = 0; i < components.size(); i++) {
		RELEASE(components[i]);
	}

	components.clear();

	for (GameObject* go : children)
	{
		RELEASE(go);
	}

	parent = nullptr;
}

void GameObject::Update(float dt)
{
	for (Component* component : components)
	{
		component->Update(dt);
	}
}

void GameObject::OnGui()
{
	if (App->scene->root != this)
	{
		//ImGui::Text("%s", name.c_str());
		ImGui::Separator();

		for (Component* component : components)
		{
			component->OnGui();
		}
		if (ImGui::CollapsingHeader("Identifiers"))
		{
			ImGui::Text("UUID: %u", (unsigned int)uuid);
			ImGui::Text("Parent UUID: %u", (unsigned int)parentUuid);
		}
	}

	
}

void GameObject::DeleteComponent(Component* component) {

	auto componentIt = std::find(components.begin(), components.end(), component);
	if (componentIt != components.end())
	{
		components.erase(componentIt);
		components.shrink_to_fit();
	}
}

void GameObject::AddComponent(Component* component)
{
	components.push_back(component);
}

void GameObject::AttachChild(GameObject* child)
{
	child->parent = this;
	children.push_back(child);
	if (child->transform != nullptr) {
		child->transform->NewAttachment();
		child->PropagateTransform();
	}
	App->scene->gameObjectList.push_back(child);
}

void GameObject::RemoveChild(GameObject* child)
{
	auto it = std::find(children.begin(), children.end(), child);
	if (it != children.end())
	{
		children.erase(it);
	}
}

void GameObject::PropagateTransform()
{
	for (GameObject* go : children)
	{
		go->transform->OnParentMoved();
	}
}

void GameObject::OnSave(JSONWriter& writer) const
{
	writer.StartObject();

	// Game object name
	writer.String("Name");
	writer.String(name.c_str());

	// Game object uuid
	writer.String("UUID");
	writer.Uint(uuid);

	// Game object parent uuid
	if (parent != nullptr)
	{
		writer.String("Parent UUID");
		writer.Uint(parentUuid);
	}

	// Game object components
	if (components.size() > 0)
	{
		writer.String("Components");
		writer.StartObject();
		for (int i = 0; i < components.size(); ++i)
		{
			components[i]->OnSave(writer);
		}
		writer.EndObject();
	}


	// Game object children
	if (children.size() > 0)
	{
		writer.String("Children");
		writer.StartArray();
		for (int i = 0; i < children.size(); ++i)
		{
			children[i]->OnSave(writer);
		}
		writer.EndArray();
	}

	writer.EndObject();
}

void GameObject::OnLoad(const rapidjson::GenericObject<true, rapidjson::Value>& reader)
{
	if (reader.HasMember("Name"))
	{
		name = reader["Name"].GetString();
	}
	if (reader.HasMember("UUID"))
	{
		uuid = reader["UUID"].GetUint();
	}
	if (reader.HasMember("Parent UUID"))
	{
		parentUuid = reader["Parent UUID"].GetUint();
	}

	if (reader.HasMember("Components"))
	{
		rapidjson::Value& itemComponents = (rapidjson::Value&)reader["Components"];//Components:

		if (itemComponents.HasMember("Transform"))
		{
			const rapidjson::Value& itemTransform = itemComponents["Transform"];

			GetComponent<ComponentTransform>()->OnLoad(itemTransform);

		}
		if (itemComponents.HasMember("Material"))
		{
			const rapidjson::Value& itemMaterial = itemComponents["Material"];
			CreateComponent<ComponentMaterial>();
			GetComponent<ComponentMaterial>()->OnLoad(itemMaterial);
		}
		if (itemComponents.HasMember("Mesh"))
		{
			const rapidjson::Value& itemMesh = itemComponents["Mesh"];
			CreateComponent<ComponentMesh>();
			GetComponent<ComponentMesh>()->OnLoad(itemMesh);

		}
		if (itemComponents.HasMember("Transform 2D"))
		{
			const rapidjson::Value& itemTransform2d = itemComponents["Transform 2D"];
			transform = nullptr;
			/*CreateComponent<ComponentTransform2D>();*/
			GetComponent<ComponentTransform2D>()->OnLoad(itemTransform2d);
		}
		if (itemComponents.HasMember("Image"))
		{
			const rapidjson::Value& itemImage = itemComponents["Image"];
			CreateComponent<ComponentImage>();
			GetComponent<ComponentImage>()->OnLoad(itemImage);
		}
		if (itemComponents.HasMember("Button"))
		{
			const rapidjson::Value& itemButton = itemComponents["Button"];
			CreateComponent<ComponentButton>();
			GetComponent<ComponentButton>()->OnLoad(itemButton);
		}
		if (itemComponents.HasMember("Checkbox"))
		{
			const rapidjson::Value& itemCheckbox = itemComponents["Checkbox"];
			CreateComponent<ComponentCheckbox>();
			GetComponent<ComponentCheckbox>()->OnLoad(itemCheckbox);
		}
		if (itemComponents.HasMember("Canvas"))
		{
			const rapidjson::Value& itemCanvas = itemComponents["Canvas"];
			CreateComponent<ComponentCanvas>();
			GetComponent<ComponentCanvas>()->OnLoad(itemCanvas);
			if (GetComponent<ComponentTransform>() != nullptr)
			{
				DeleteComponent(GetComponent<ComponentTransform>());
			}
		}
		if (itemComponents.HasMember("Text"))
		{
			const rapidjson::Value& itemText = itemComponents["Text"];
			CreateComponent<ComponentText>();
			GetComponent<ComponentText>()->OnLoad(itemText);
		}
		if (itemComponents.HasMember("Window"))
		{
			const rapidjson::Value& itemWindow = itemComponents["Window"];
			CreateComponent<ComponentWindow>();
			GetComponent<ComponentWindow>()->OnLoad(itemWindow);
		}
	}

	if (reader.HasMember("Children")) // Load the children
	{
		rapidjson::Value& itemChildren = (rapidjson::Value&)reader["Children"];
		for (rapidjson::Value::ConstValueIterator it = itemChildren.Begin(); it != itemChildren.End(); ++it)
		{
			const rapidjson::Value& attribute = *it;
			assert(attribute.IsObject());
			
			if (attribute.HasMember("Components"))
			{
				const rapidjson::Value& attribute2 = attribute["Components"];
				if (attribute2.HasMember("Transform 2D"))
				{
					GameObject* child = new GameObject(false);
					AttachChild(child);
					child->OnLoad(attribute.GetObjectJSON());
				}
				else
				{
					GameObject* child = new GameObject();
					AttachChild(child);
					child->OnLoad(attribute.GetObjectJSON());
				}
			}


		}

	}
}