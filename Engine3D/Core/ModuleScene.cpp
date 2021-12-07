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
#include "FileManager.h"
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
	
	root = new GameObject("Root");
	App->editor->gameobjectSelected = root;

	//Loading house and textures since beginning
	App->import->LoadGeometry("Assets/Models/BakerHouse.fbx");
	
	return ret;
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

	return true;
}

update_status ModuleScene::Update(float dt)
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

	return UPDATE_CONTINUE;
}

GameObject* ModuleScene::CreateGameObject(GameObject* parent) {

	GameObject* temp = new GameObject();
	if (parent)
		parent->AttachChild(temp);
	else
		root->AttachChild(temp);
	return temp;
}
GameObject* ModuleScene::CreateGameObject(const std::string name, GameObject* parent)
{
	GameObject* temp = new GameObject(name);
	if (parent)
		parent->AttachChild(temp);
	else
		root->AttachChild(temp);
	return temp;
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
	std::string sceneName = "Scene: " + scene;

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
			rapidjson::Value config = document.GetObjectJSON();
			root = new GameObject("Root");

			if (config.HasMember(sceneName.c_str()))
			{
				const rapidjson::Value& itemScene = config[sceneName.c_str()];
				if (itemScene.HasMember("Game Objects"))
				{
					const rapidjson::Value& itemGameObjs = itemScene["Game Objects"];

					if (itemGameObjs.IsArray())
					{
						for (rapidjson::Value::ConstValueIterator it = itemGameObjs.Begin(); it != itemGameObjs.End(); ++it)
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