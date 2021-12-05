#include "FileManager.h"

FileManager::FileManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

bool FileManager::Init()
{
    return false;
}

update_status FileManager::Update(float dt)
{
    return UPDATE_CONTINUE;
}

bool FileManager::CleanUp()
{
    bool ret = true;
    return true;
}

File* FileManager::createFile(const aiMesh* m)
{
    return nullptr;
}

bool FileManager::saveFile(File* file)
{
    bool ret = true;
    return ret;
}

bool FileManager::loadfile(File* file)
{
    bool ret = true;
    return ret;
}

bool FileManager::removeFile(File* file)
{
    bool ret = true;
    return ret;
}
