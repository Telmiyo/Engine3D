#include "ResourceManager.h"
#include <fstream>

ResourceManager::ResourceManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ResourceManager::~ResourceManager()
{
}

bool ResourceManager::CreateModelFile(const aiMesh* m, const char* path, std::string name)
{
	MeshFile* file = new MeshFile();

	file->numVertices = m->mNumVertices;
	//file->numVerticesSizeBytes = m->mNumVertices * sizeof(float) * 3;//3==x,y,z
	file->verticesSize = file->vertices.size();

	file->vertices.resize(m->mNumVertices);
	memcpy(&file->vertices[0], m->mVertices, sizeof(float3) * m->mNumVertices);
	//file->verticesSizeBytes = file->vertices.size() * sizeof(float3);

	// -- Copying faces --//
	if (m->HasFaces()) {
		file->numIndices = m->mNumFaces * 3;
		file->indices.resize(file->numIndices);

		for (size_t i = 0; i < m->mNumFaces; i++)
		{
			if (m->mFaces[i].mNumIndices != 3) {
				LOG("WARNING, geometry face with != 3 indices!")
			}
			else {
				memcpy(&file->indices[i * 3], m->mFaces[i].mIndices, 3 * sizeof(uint));
			}
		}
	}

	// // -- Copying Normals info --//
	if (m->HasNormals()) {

		file->normals.resize(m->mNumVertices);
		memcpy(&file->normals[0], m->mNormals, sizeof(float3) * m->mNumVertices);
	}

	// -- Copying UV info --//
	if (m->HasTextureCoords(0))
	{
		file->texCoords.resize(m->mNumVertices);
		for (size_t j = 0; j < m->mNumVertices; ++j)
		{
			memcpy(&file->texCoords[j], &m->mTextureCoords[0][j], sizeof(float2));
		}
	}

	file->name = name;
	models.push_back(file);

	return saveModelFile(file, path, name);
}

//LOAD IN BINARY THE FILE
bool ResourceManager::saveModelFile(MeshFile* file, const char* path, std::string name)
{

	std::string fileName = "Assets/Files/" + name + ".amapola";
	std::ofstream myfile;
	myfile.open(fileName.c_str(), std::ios::in | std::ios::app | std::ios::binary);
	if (myfile.is_open())
	{
		//FILE
		//NUMVERTICES
		//VERTICES SIZE
		// Vertices
		unsigned a = sizeof(file->numVertices);
		myfile.write((char*)file, 5 * sizeof(unsigned)); //write header
		myfile.write((char*)&file->numVertices, sizeof(unsigned));
		myfile.write((char*)&file->verticesSize, sizeof(unsigned));

		for (auto v : file->vertices)
		{
			myfile.write((char*)&v, 3* sizeof(float));

		}

		myfile.close();


		return true; //all fine
	}
	else
	{
		return false;//somethig bad
	}
}

MeshFile* ResourceManager::loadModel(std::string name)
{
	std::ifstream newFile;
	std::string fullName = "Assets/Files/" + name + ".amapola";
	newFile.open(fullName.c_str(), std::ios::binary);
	if (newFile.is_open())
	{

		MeshFile* file = (MeshFile*)malloc(sizeof(MeshFile));
		//FILE
		//NUMVERTICES
		//VERTICES SIZE
		// Vertices
		newFile.read((char*)file, 5 * sizeof(unsigned)); //READ HEADER

		newFile.read((char*)&file->numVertices, sizeof(unsigned));
		newFile.read((char*)&file->verticesSize, sizeof(unsigned));
		uint size = file->verticesSize;
		//for (int i = 0; i < size; ++i)
		//{
		//	newFile.read((char*)&file->vertices[i], 3* sizeof(float));

		//}

	

		newFile.close();
		return file;
	}
	else
	{
		return NULL;
	}
}