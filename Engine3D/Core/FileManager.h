#pragma once
#include "Module.h"

#include "Assimp/include/mesh.h"
#include <string>
#include <vector>

struct aiScene;

typedef struct _model
{
    unsigned verticesSizeBytes = 0;
    unsigned normalsSizeBytes = 0;
    unsigned indiceSizeBytes = 0;
    unsigned textCoordSizeBytes = 0;
    unsigned infoSizeBytes = 0;

    float* vertices = NULL;//float,float,float  every vertex => sizeof(float) *3
    unsigned* indices = NULL;//int                every indice => sizeof(int)
    float* normals = NULL;//float,float,float  every normal => sizeof(float) *3
    float* textCoords = NULL;//float,float        every normal => sizeof(float) *2
    char* info = NULL;
    const char* name ;
    std::string name2;


}TMYMODEL;

class FileManager : public Module
{
public:

	FileManager(Application* app, bool start_enabled = true);

	bool Init() override;
	bool CleanUp() override;

    bool createMymodel(const aiMesh* m, const char* path, const char* name);
    bool saveModel(TMYMODEL* m, const char* path);
    TMYMODEL* loadModel(const char* path);
    void destroyMymodel(TMYMODEL* mymodel);

    void pushMymodel(TMYMODEL* mymodel);


    std::vector<TMYMODEL*> models;

};
