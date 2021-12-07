#include "GameObject.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleFileSystem.h"
#include "ComponentTransform.h"
#include "ImGui/imgui.h"

GameObject::GameObject() {

	name = name + ("GameObject");
	parent = nullptr;
	
	transform = CreateComponent<ComponentTransform>();

	active = true;
}

GameObject::GameObject(const std::string name) : name(name) 
{
	transform = CreateComponent<ComponentTransform>();

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
		ImGui::Text("%s", name.c_str());
		ImGui::Separator();

		for (Component* component : components)
		{
			component->OnGui();
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
	child->transform->NewAttachment();
	child->PropagateTransform();
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
	else
	{
		writer.String("Parent UUID");
		writer.String("none");
	}

	// Game object components
	writer.String("Components");
	writer.StartArray();
	for (int i = 0; i < components.size(); ++i)
	{
		components[i]->OnSave(writer);
	}
	writer.EndArray();

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
	else
	{
		writer.String("Children");
		writer.String("none");
	}
	
	writer.EndObject();
}

void GameObject::OnLoad(const rapidjson::GenericObject<true, rapidjson::Value>& reader)
{
	if (reader.HasMember("Name"))
	{
		name = reader["Name"].GetString();
	}
}


