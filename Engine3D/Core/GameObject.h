#pragma once

#include <vector>
#include <string>
#include "rapidjson-1.1.0/include/rapidjson/prettywriter.h"
#include "rapidjson-1.1.0/include/rapidjson/document.h"

typedef rapidjson::PrettyWriter<rapidjson::StringBuffer> JSONWriter;
typedef rapidjson::Value JSONReader;

class Component;
class ComponentTransform;
class ComponentTransform2D;

class GameObject {

public:

	GameObject(bool is3D = true);
	GameObject(const std::string name, bool is3D = true);

	~GameObject();

	void Update(float dt);
	void OnGui();

	template<class T> T* CreateComponent()
	{
		T* newComponent = new T(this);
		return newComponent;
	}

	template<class T> T* GetComponent()
	{
		T* component = nullptr; 
		for (Component* c : components)
		{
			component = dynamic_cast<T*>(c);
			if (component)
				break;
		}
		return component;
	}

	void DeleteComponent(Component* component);
	void AddComponent(Component* component);
	void AttachChild(GameObject* child);
	void RemoveChild(GameObject* child);
	void PropagateTransform();

	void OnSave(JSONWriter& writer) const;
	void OnLoad(const rapidjson::GenericObject<true, rapidjson::Value>& reader);

	std::string name;
	unsigned int uuid;
	unsigned int parentUuid;
	GameObject* parent = nullptr;
	ComponentTransform* transform = nullptr;
	ComponentTransform2D* transform2d = nullptr;
	std::vector<GameObject*> children;
	std::vector<Component*> components;
	
	bool active = true;
	bool isSelected = false;

};

