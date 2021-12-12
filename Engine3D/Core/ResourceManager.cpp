#include "ResourceManager.h"
#include <fstream>

ResourceManager::ResourceManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ResourceManager::~ResourceManager()
{
	for (unsigned int i = 0; i < models.size(); ++i)
	{
		DestroyMeshFile(models[i]);
	}
}

bool ResourceManager::CreateModelFile(const aiMesh* m, const char* path, std::string name)
{
	MeshFile* file = new MeshFile();

	file->verticesSizeBytes = m->mNumVertices * sizeof(float) * 3;
	file->vertices_ = (float*)malloc(file->verticesSizeBytes);
	memcpy(file->vertices_, m->mVertices, file->verticesSizeBytes);

	// Normals Data
	file->normalsSizeBytes = m->mNumVertices * sizeof(float) * 3;
	file->normals_ = (float*)malloc(file->normalsSizeBytes);
	memcpy(file->normals_, m->mNormals, file->normalsSizeBytes);

	// Text Coord Data
	file->textCoordSizeBytes = m->mNumVertices * sizeof(float) * 2;
	file->textCoords_ = (float*)malloc(file->textCoordSizeBytes);
	for (int i = 0; i < m->mNumVertices; i++)
	{
		*(file->textCoords_ + i * 2) = m->mTextureCoords[0][i].x;
		*(file->textCoords_ + i * 2 + 1) = m->mTextureCoords[0][i].y; //this coord image is inverted
	}

	// Indices Data
	file->indiceSizeBytes = m->mNumFaces * sizeof(unsigned int) * 3;
	file->indices_ = (unsigned*)malloc(file->indiceSizeBytes);
	for (int i = 0; i < m->mNumFaces; i++)
	{
		aiFace* f = m->mFaces + i;
		*(file->indices_ + 0 + i * 3) = f->mIndices[0];
		*(file->indices_ + 1 + i * 3) = f->mIndices[1];
		*(file->indices_ + 2 + i * 3) = f->mIndices[2];
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
	myfile.open(fileName, std::ios::binary);
	if (myfile.is_open())
	{
		myfile.write((char*)file, 4 * sizeof(unsigned int)); // Save header data & numVertices & numFaces

		myfile.write((char*)file->vertices_, file->verticesSizeBytes);
		myfile.write((char*)file->normals_, file->normalsSizeBytes);
		myfile.write((char*)file->textCoords_, file->textCoordSizeBytes);
		myfile.write((char*)file->indices_, file->indiceSizeBytes);

		myfile.close();
		return true;
	}
	else
	{
		LOG("Error creating MontuMeshFile at '%s'", fileName);
		return false;
	}
}

MeshFile* ResourceManager::LoadMeshFile(std::string name)
{
	std::string fullName = "Assets/Files/" + name + ".amapola";

	std::ifstream myfile;
	myfile.open(fullName, std::ios::binary);
	if (myfile.is_open())
	{
		MeshFile* mymodel = new MeshFile();
		myfile.read((char*)mymodel, 4 * sizeof(unsigned int)); // Load header data & numVertices & numFaces

		mymodel->vertices_ = (float*)malloc(mymodel->verticesSizeBytes);
		myfile.read((char*)mymodel->vertices_, mymodel->verticesSizeBytes);

		mymodel->normals_ = (float*)malloc(mymodel->normalsSizeBytes);
		myfile.read((char*)mymodel->normals_, mymodel->normalsSizeBytes);

		mymodel->textCoords_ = (float*)malloc(mymodel->textCoordSizeBytes);
		myfile.read((char*)mymodel->textCoords_, mymodel->textCoordSizeBytes);

		mymodel->indices_ = (unsigned int*)malloc(mymodel->indiceSizeBytes);
		myfile.read((char*)mymodel->indices_, mymodel->indiceSizeBytes);

		myfile.close();

		ArrayToVectorConversion(mymodel);

		return mymodel;
	}
	else
	{
		LOG("Error loading MontuMeshFile from '%s'", fullName);
		return nullptr;
	}
}

void ResourceManager::DestroyMeshFile(MeshFile* mesh)
{

	free(mesh->vertices_);
	free(mesh->normals_);
	free(mesh->textCoords_);
	free(mesh->indices_);

	mesh->vecVertices.clear();
	mesh->vecVertices.swap(std::vector<float3>());
	mesh->vecNormals.clear();
	mesh->vecNormals.swap(std::vector<float3>());
	mesh->vecTextCoords.clear();
	mesh->vecTextCoords.swap(std::vector<float2>());
	mesh->vecIndices.clear();
	mesh->vecIndices.swap(std::vector<unsigned int>());

	mesh->name.clear();
	mesh->texturePath.clear();

	free(mesh);
}

void ResourceManager::ArrayToVectorConversion(MeshFile* mymodel)
{
	mymodel->vecVertices = FloatArray2VecFloat3(mymodel->vertices_, mymodel->verticesSizeBytes / (sizeof(float)));
	mymodel->vecNormals = FloatArray2VecFloat3(mymodel->normals_, mymodel->normalsSizeBytes / (sizeof(float)));
	mymodel->vecTextCoords = FloatArray2VecFloat2(mymodel->textCoords_, mymodel->textCoordSizeBytes / (sizeof(float)));
	mymodel->vecIndices = UnsignedIntArray2VecFloat3(mymodel->indices_, mymodel->indiceSizeBytes / (sizeof(unsigned int)));
}

std::vector<float3> ResourceManager::FloatArray2VecFloat3(float* array, unsigned int n)
{
	std::vector<float3> dest;
	for (unsigned int i = 0; i < n; i += 3)
	{
		dest.push_back({ array[i], array[i + 1], array[i + 2]});
	}
	return dest;
}

std::vector<float2> ResourceManager::FloatArray2VecFloat2(float* array, unsigned int n)
{
	std::vector<float2> dest;
	for (unsigned int i = 0; i < n; i += 2)
	{
		dest.push_back({ array[i], array[i + 1]});
	}
	return dest;
}

std::vector<unsigned int> ResourceManager::UnsignedIntArray2VecFloat3(unsigned int* array, unsigned int n)
{
	std::vector<unsigned int> dest;
	for (unsigned int i = 0; i < n; ++i)
	{
		dest.push_back(array[i]);
	}
	return dest;
}


