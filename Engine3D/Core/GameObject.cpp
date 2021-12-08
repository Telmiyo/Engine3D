#include "GameObject.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleFileSystem.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"

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

	if (ImGui::CollapsingHeader("Identifiers"))
	{
		ImGui::Text("UUID:", (unsigned int)uuid);
		ImGui::Text("Parent UUID:", (unsigned int)parentUuid);
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
			for (auto c : components)
			{
				if (c->componentType == ComponentType::COMPONENT_TRANSFORM)
				{
					c->OnLoad(itemTransform);
				}
			}
		}
		if (itemComponents.HasMember("Material"))
		{
			const rapidjson::Value& itemMaterial = itemComponents["Material"];
			for (auto c : components)
			{
				if (c->componentType == ComponentType::COMPONENT_MATERIAL)
				{
					c->OnLoad(itemMaterial);
				}
			}
		}
		if (itemComponents.HasMember("Mesh"))
		{
			const rapidjson::Value& itemMesh = itemComponents["Mesh"];
			for (auto c : components)
			{
				if (c->componentType == ComponentType::COMPONENT_MESH)
				{
					c->OnLoad(itemMesh);
				}
			}
		}
	}

	if (reader.HasMember("Children")) // Load the children
	{

		rapidjson::Value& itemChildren = (rapidjson::Value&)reader["Children"];
		int counter = 0;

		for (rapidjson::Value::ConstValueIterator it = itemChildren.Begin(); it != itemChildren.End(); ++it)
		{
			const rapidjson::Value& attribute = *it;
			assert(attribute.IsObject());

			children.push_back(new GameObject());
			children[counter]->name = attribute.FindMember("Name")->value.GetString();
			children[counter]->uuid = attribute.FindMember("UUID")->value.GetUint();

			if (attribute.HasMember("Components"))
			{
				rapidjson::Value& childrenComponents = (rapidjson::Value&)attribute.FindMember("Components")->value;
				if (childrenComponents.HasMember("Transform"))
				{
					const rapidjson::Value& itemTransform = childrenComponents["Transform"];
					/*for (auto c : children[counter]->components)
					{
						if (c->componentType == ComponentType::COMPONENT_TRANSFORM)
						{
							c->OnLoad(itemTransform);
						}
					}*/
				}
				if (childrenComponents.HasMember("Material"))
				{
					const rapidjson::Value& itemMaterial = childrenComponents["Material"];
					//children[counter]->AddComponent(new ComponentMaterial(children[counter]->parent));

					/*for (auto c : children[counter]->components)
					{
						if (c->componentType == ComponentType::COMPONENT_MATERIAL)
						{
							c->OnLoad(itemMaterial);
						}
					}*/
				}
				if (childrenComponents.HasMember("Mesh"))
				{
					const rapidjson::Value& itemMesh = childrenComponents["Mesh"];
					//children[counter]->AddComponent(new ComponentMesh(children[counter]->parent));

		/*			for (auto c : children[counter]->components)
					{
						if (c->componentType == ComponentType::COMPONENT_MESH)
						{
							c->OnLoad(itemMesh);
						}
					}*/
				}

			}

		}
	}
}


