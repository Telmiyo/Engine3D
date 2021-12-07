#include "FileManager.h"
#include "Application.h"
#include "ModuleFileSystem.h"

#include <fstream>

FileManager::FileManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

bool FileManager::Init()
{
	bool ret = true;
	return ret;
}

bool FileManager::CleanUp()
{
	bool ret = true;
	return true;
}

bool FileManager::createMymodel(const aiMesh* m, const char* path, std::string name)
{
	//TMYMODEL* mymodel = (TMYMODEL*)malloc(sizeof(TMYMODEL));
	TMYMODEL* mymodel = new TMYMODEL();
	mymodel->verticesSizeBytes = m->mNumVertices * sizeof(float) * 3;//3==x,y,z
	mymodel->vertices = (float*)malloc(mymodel->verticesSizeBytes);
	memcpy(mymodel->vertices, m->mVertices, mymodel->verticesSizeBytes);

	mymodel->normalsSizeBytes = m->mNumVertices * sizeof(float) * 3;//3==x,y,z equal vertex
	mymodel->normals = (float*)malloc(mymodel->normalsSizeBytes);
	memcpy(mymodel->normals, m->mNormals, mymodel->normalsSizeBytes);

	mymodel->textCoordSizeBytes = m->mNumVertices * sizeof(float) * 2;//3==u,v
	mymodel->textCoords = (float*)malloc(mymodel->textCoordSizeBytes);
	for (int i = 0; i < m->mNumVertices; i++)
	{
		*(mymodel->textCoords + i * 2) = m->mTextureCoords[0][i].x;
		*(mymodel->textCoords + i * 2 + 1) = 1.0 - m->mTextureCoords[0][i].y; //this coord image is inverted
	}

	mymodel->indiceSizeBytes = m->mNumFaces * sizeof(unsigned) * 3; //3==indices/face
	mymodel->indices = (unsigned*)malloc(mymodel->indiceSizeBytes);
	for (int i = 0; i < m->mNumFaces; i++)
	{
		aiFace* f = m->mFaces + i;
		*(mymodel->indices + 0 + i * 3) = f->mIndices[0];
		*(mymodel->indices + 1 + i * 3) = f->mIndices[1];
		*(mymodel->indices + 2 + i * 3) = f->mIndices[2];
	}

	const char* someinfo = "This is a great model for my engine. Author: Juan"; // '\0' OjO
	mymodel->infoSizeBytes = 128; //or str length
	mymodel->info = (char*)malloc(mymodel->infoSizeBytes);
	memcpy(mymodel->info, someinfo, mymodel->infoSizeBytes);
	mymodel->info[mymodel->infoSizeBytes - 1] = '\0';
	
	mymodel->name = name;
	
	pushMymodel(mymodel);

	return saveModel(mymodel, path, name);
}

bool FileManager::saveModel(TMYMODEL* m, const char* path, std::string name)
{
	//My format
	//Header
	//a)unsigned for  verticesSizeBytes
	//b)unsigned for   normalsSizeBytes
	//c)unsigned for textCoordSizeBytes
	//d)unsigned for    indiceSizeBytes
	//e)unsigned for      infoSizeBytes
	//so the header size is 5 * sizeof(unsigned) -> 20 bytes
	std::string fileName = "Assets/Files/" + name + ".amapola";
	std::ofstream myfile;
	myfile.open(fileName.c_str(), std::ios::in | std::ios::app | std::ios::binary);
	if (myfile.is_open())
	{
		myfile.write((char*)m, 5 * sizeof(unsigned)); //write header
		myfile.write((char*)m->vertices, m->verticesSizeBytes);
		myfile.write((char*)m->normals, m->normalsSizeBytes);
		myfile.write((char*)m->textCoords, m->textCoordSizeBytes);
		myfile.write((char*)m->indices, m->indiceSizeBytes);
		myfile.write((char*)m->info, m->infoSizeBytes);
		
		myfile.close();

		
		return true; //all fine
	}
	else
	{
		return false;//somethig bad
	}

}

TMYMODEL* FileManager::loadModel(std::string name)
{
	std::ifstream myfile;
	std::string fullName = "Assets/Files/" + name + ".amapola";
	myfile.open(fullName.c_str(), std::ios::binary);
	if (myfile.is_open())
	{

		TMYMODEL* mymodel = (TMYMODEL*)malloc(sizeof(TMYMODEL));
		myfile.read((char*)mymodel, 5 * sizeof(unsigned)); //READ HEADER

		mymodel->vertices = (float*)malloc(mymodel->verticesSizeBytes);
		myfile.read((char*)mymodel->vertices, mymodel->verticesSizeBytes);

		mymodel->normals = (float*)malloc(mymodel->normalsSizeBytes);
		myfile.read((char*)mymodel->normals, mymodel->normalsSizeBytes);

		mymodel->textCoords = (float*)malloc(mymodel->textCoordSizeBytes);
		myfile.read((char*)mymodel->textCoords, mymodel->textCoordSizeBytes);

		mymodel->indices = (unsigned*)malloc(mymodel->indiceSizeBytes);
		myfile.read((char*)mymodel->indices, mymodel->indiceSizeBytes);

		mymodel->info = (char*)malloc(mymodel->infoSizeBytes);
		myfile.read(mymodel->info, mymodel->infoSizeBytes);

		myfile.close();
		return mymodel;
	}
	else
	{
		return NULL;
	}
}

void FileManager::destroyMymodel(TMYMODEL* mymodel)
{
	free(mymodel->vertices);
	free(mymodel->normals);
	free(mymodel->textCoords);
	free(mymodel->indices);
	free(mymodel->info);

	free(mymodel);

}

void FileManager::pushMymodel(TMYMODEL* mymodel)
{
	models.push_back(mymodel);
}

