#pragma once

#include <vector>
#include <string>


class File {

public:

	File();
	File(const std::string name);

	~File();


	std::string name;
private:

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

};

