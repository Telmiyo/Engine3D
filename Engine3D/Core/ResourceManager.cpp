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

// Converts data from aiMesh to MontuMeshFile
MontuMeshFile* ResourceManager::MontuImportMyModelData(const aiMesh* m)
{
	// TODO: Future resource manager
	// if the mesh doesn't exist, aka, the uuid doesn't exist

	MontuMeshFile* mymodel = (MontuMeshFile*)malloc(sizeof(MontuMeshFile));

	// Vertices Data
	mymodel->verticesSizeBytes = m->mNumVertices * sizeof(float) * 3;
	mymodel->vertices_ = (float*)malloc(mymodel->verticesSizeBytes);
	memcpy(mymodel->vertices_, m->mVertices, mymodel->verticesSizeBytes);

	// Normals Data
	mymodel->normalsSizeBytes = m->mNumVertices * sizeof(float) * 3;
	mymodel->normals_ = (float*)malloc(mymodel->normalsSizeBytes);
	memcpy(mymodel->normals_, m->mNormals, mymodel->normalsSizeBytes);

	// Text Coord Data
	mymodel->textCoordSizeBytes = m->mNumVertices * sizeof(float) * 2;
	mymodel->textCoords_ = (float*)malloc(mymodel->textCoordSizeBytes);
	for (int i = 0; i < m->mNumVertices; i++)
	{
		*(mymodel->textCoords_ + i * 2) = m->mTextureCoords[0][i].x;
		*(mymodel->textCoords_ + i * 2 + 1) = 1.0 - m->mTextureCoords[0][i].y;
	}

	// Indices Data
	mymodel->indiceSizeBytes = m->mNumFaces * sizeof(unsigned int) * 3;
	mymodel->indices_ = (unsigned*)malloc(mymodel->indiceSizeBytes);
	for (int i = 0; i < m->mNumFaces; i++)
	{
		aiFace* f = m->mFaces + i;
		*(mymodel->indices_ + 0 + i * 3) = f->mIndices[0];
		*(mymodel->indices_ + 1 + i * 3) = f->mIndices[1];
		*(mymodel->indices_ + 2 + i * 3) = f->mIndices[2];
	}

	return mymodel;
}

bool ResourceManager::MontuMeshToFile(const MontuMeshFile* m, const char* path)
{
	/*
	Header:
	unsigned int verticesSizeBytes = 0;
	unsigned int normalsSizeBytes = 0;
	unsigned int indiceSizeBytes = 0; 
	unsigned int textCoordSizeBytes = 0; 
	total = 4 * sizeof(unsigned int)
	*/
	std::string fullPath = "Assets/Files/";
	fullPath += path;
	fullPath += ".amapola";

	std::ofstream myfile;
	myfile.open(fullPath,/* std::ios::in | std::ios::app |*/ std::ios::binary);
	if (myfile.is_open())
	{
		myfile.write((char*)m, 4 * sizeof(unsigned int)); // Save header data & numVertices & numFaces

		myfile.write((char*)m->vertices_, m->verticesSizeBytes);
		myfile.write((char*)m->normals_, m->normalsSizeBytes);
		myfile.write((char*)m->textCoords_, m->textCoordSizeBytes);
		myfile.write((char*)m->indices_, m->indiceSizeBytes);

		myfile.close();
		return true;
	}
	else
	{
		LOG("Error creating MontuMeshFile at '%s'", fullPath);
		return false;
	}
}

MontuMeshFile* ResourceManager::MontuLoadMyModelFile(const char* path)
{
	std::string fullPath = "Assets/Files/";
	fullPath += path;
	fullPath += ".amapola";

	std::ifstream myfile;
	myfile.open(fullPath, std::ios::binary);
	if (myfile.is_open())
	{
		/*MontuMeshFile* mymodel = (MontuMeshFile*)malloc(sizeof(MontuMeshFile));*/
		MontuMeshFile* mymodel = new MontuMeshFile();
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
		LOG("Error loading MontuMeshFile from '%s'", fullPath);
		return nullptr;
	}


}

void ResourceManager::ArrayToVectorConversion(MontuMeshFile* mymodel)
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


