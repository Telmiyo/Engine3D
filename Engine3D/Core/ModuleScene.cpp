#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "glew.h"
#include "ImGui/imgui.h"
#include "ModuleImport.h"
#include "ModuleTextures.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "ComponentMesh.h"
#include "ModuleFileSystem.h"
#include <stack>
#include <queue>

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

bool ModuleScene::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	
	CreateRoot();
	
	
	//Loading house and textures since beginning
	OnLoad("scene1.scene");
	
	return ret;
}

update_status ModuleScene::Update(float dt)
{
	if (!rootList.empty())
	{
		std::queue<GameObject*> S;
		for (GameObject* child : root->children)
		{
			S.push(child);
		}

		while (!S.empty())
		{
			GameObject* go = S.front();
			go->Update(dt);
			S.pop();
			for (GameObject* child : go->children)
			{
				S.push(child);
			}
		}

		glDisable(GL_DEPTH_TEST);

		if (App->editor->gameobjectSelected)
		{
			ComponentTransform* transform = App->editor->gameobjectSelected->GetComponent<ComponentTransform>();
			float3 pos = transform->GetPosition();
			glLineWidth(10.f);
			glBegin(GL_LINES);
			glColor3f(1.f, 0.f, 0.f);
			glVertex3f(pos.x, pos.y, pos.z);
			glVertex3f(pos.x + transform->Right().x, pos.y + transform->Right().y, pos.z + transform->Right().z);
			glColor3f(0.f, 0.f, 1.f);
			glVertex3f(pos.x, pos.y, pos.z);
			glVertex3f(pos.x + transform->Front().x, pos.y + transform->Front().y, pos.z + transform->Front().z);
			glColor3f(0.f, 1.f, 0.f);
			glVertex3f(pos.x, pos.y, pos.z);
			glVertex3f(pos.x + transform->Up().x, pos.y + transform->Up().y, pos.z + transform->Up().z);
			glEnd();
			glLineWidth(1.f);
		}

		glEnable(GL_DEPTH_TEST);
	}

	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	std::stack<GameObject*> S;
	for (GameObject* child : root->children)
	{
		S.push(child);
	}
	root->children.clear();

	while (!S.empty())
	{
		GameObject* go = S.top();
		S.pop();
		for (GameObject* child : go->children)
		{
			S.push(child);
		}
		go->children.clear();
		delete go;
	}

	delete root;

	rootList.clear();
	gameObjectList.clear();

	return true;
}

void ModuleScene::OnSave(std::string scene) const
{
	std::string sceneName = "Scene: " + scene;

	rapidjson::StringBuffer sb;
	JSONWriter writer(sb);

	writer.StartObject();

	// Scene name
	writer.String(sceneName.c_str());

	// Scene game objects
	writer.StartObject();
	writer.String("Game Objects");
	// Call to every game object save
	root->OnSave(writer);
	writer.EndObject();

	writer.EndObject();

	// LOG checker
	if (App->fileSystem->Save(scene.c_str(), sb.GetString(), strlen(sb.GetString()), false))
	{
		LOG("Scene: '%s' succesfully saved", scene.c_str());
	}
	else
	{
		LOG("Error saving scene: '%s'", scene.c_str());
	}
}

void ModuleScene::OnLoad(std::string scene)
{
	std::string sceneName = "Scene: " + scene;//Scene:Scene1.scene

	char* buffer = nullptr;
	uint bytesFile = App->fileSystem->Load(scene.c_str(), &buffer);

	if (bytesFile)
	{
		rapidjson::Document document;
		if (document.Parse<rapidjson::kParseStopWhenDoneFlag>(buffer).HasParseError())
		{
			LOG("Error loading scene: '%s'", scene.c_str());
		}
		else
		{
			rapidjson::Value config = document.GetObjectJSON();//Document
			root = new GameObject("Root");
			rootList.clear();
			rootList.push_back(root);
			gameObjectList.clear();
			gameObjectList.push_back(root);
			

			if (config.HasMember(sceneName.c_str()))
			{
				const rapidjson::Value& itemScene = config[sceneName.c_str()];//Scene:
				if (itemScene.HasMember("Game Objects"))
				{
					const rapidjson::Value& itemGameObjs = itemScene["Game Objects"];//Game Objects:

					if (itemGameObjs.IsArray())
					{
						for (rapidjson::Value::ConstValueIterator it = itemGameObjs.Begin(); it != itemGameObjs.End(); ++it)//
						{
							root->OnLoad(it->GetObjectJSON());
						}
					}
					else
						root->OnLoad(itemGameObjs.GetObjectJSON());
				}
			}
		

			LOG("Scene: '%s' succesfully loaded", scene.c_str());
		}
	}
	RELEASE_ARRAY(buffer);
}

GameObject* ModuleScene::CreateGameObjectByName(const std::string name, GameObject* parent)
{
	GameObject* temp = new GameObject(name);

	if (name.empty())
		LOG("A name must be sent to CreateGameObjectByName")
	else
	{
		if (parent)
		{
			LOG("Creating game object (%s)to %s", temp->name.c_str(), parent->name.c_str());
			parent->AttachChild(temp);
		}

		else
		{
			LOG("Creating game object (%s) at %s", temp->name.c_str(), root->name.c_str());
			root->AttachChild(temp);
		}
	}

	return temp;
}

GameObject* ModuleScene::CreateEmptyGameObject(GameObject* selectedGameObject)
{
	GameObject* temp = new GameObject();
	if (emptyCounter > 0)
		temp->name += std::to_string(emptyCounter);
	emptyCounter++;

	if (selectedGameObject)
	{
		LOG("Creating empty game object to %s", selectedGameObject->name.c_str());
		selectedGameObject->AttachChild(temp);
	}
	else
	{
		LOG("Creating empty game object at %s", root->name.c_str());
		root->AttachChild(temp);
	}

	return temp;
}

GameObject* ModuleScene::CreateChildrenGameObject(GameObject* selectedGameObject)
{
	GameObject* temp = new GameObject();

	if (selectedGameObject == nullptr)
	{
		LOG("No Game Object has been selected!")
	}
	else
	{
		if (emptyCounter > 0)
			temp->name += std::to_string(emptyCounter);
		emptyCounter++;

		LOG("Creating empty game object at %s", selectedGameObject->name.c_str());
		selectedGameObject->AttachChild(temp);
		
	}
	

	return temp;
}

GameObject* ModuleScene::CreateCamera(GameObject* parent)
{
	GameObject* camera = CreateGameObjectByName("Camera", root);
	camera->CreateComponent<ComponentCamera>();
	ComponentMesh* m = new ComponentMesh(camera, ComponentMesh::Shape::CUBE);
	
	return camera;
}

bool ModuleScene::DeleteAllGameObjects()
{
	bool ret = true;
	if (root->children.empty())
		LOG("No game objects to delete.")
	else
		LOG("Deleting all game objects.")

		for (int i = 0; i < rootList.size(); ++i)
		{
			// Save the game objects in a stack (S) and then clear the root
			std::stack<GameObject*> S;
			for (GameObject* child : rootList[i]->children)
			{
				S.push(child);
			}
			rootList[i]->children.clear();

			// After clearing the root, delete from top to bottom the stack
			while (!S.empty())
			{
				GameObject* go = S.top();
				S.pop();
				for (GameObject* child : go->children)
				{
					S.push(child);
				}
				go->children.clear();
				delete go;
			}
		}

	gameObjectList.clear();
	emptyCounter = 0;

	return ret;


}

bool ModuleScene::DeleteSelectedGameObject(GameObject* selectedGameObject)
{
	bool ret = true;

	if (selectedGameObject == nullptr)
		LOG("No Game Object has been selected!")
	else
	{
		LOG("Deleting game object: %s", selectedGameObject->name.c_str())
			if (selectedGameObject != root)
			{
				for (int i = 0; i < gameObjectList.size(); i++)
				{
					if (gameObjectList[i] == selectedGameObject && selectedGameObject->parent->name == "Root")
					{
						root->RemoveChild(selectedGameObject);
						gameObjectList.erase(gameObjectList.begin() + i);
					}
					else
					{
						selectedGameObject->parent->RemoveChild(selectedGameObject);
						gameObjectList.erase(gameObjectList.begin() + i);
					}
				}
			}
			else
			{
				root->~GameObject();
			}
	}

	return ret;
}

void ModuleScene::CreateRoot()
{
	rootList.clear();

	root = new GameObject("Root");
	gameObjectList.push_back(root);
	rootList.push_back(root);

	for (GameObject* child : root->children)
	{
		gameObjectList.push_back(child);
	}
}

void ModuleScene::DuplicateGameObject(GameObject* parent)
{
	if (parent == nullptr)
		LOG("You have to select a game object to duplicate!")
	else
	{
		if (parent->parent->name == "Root")
			root->AttachChild(parent);
		else parent->parent->AttachChild(parent);
	}
}