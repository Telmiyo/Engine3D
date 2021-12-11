#pragma once
#include "Component.h"
#include "Globals.h"
#include <string.h>
#include "Math/float3.h"
#include "Math/float2.h"
#include "Geometry/AABB.h"
#include "par_shapes.h"
#include "MeshFile.h"

class ComponentMesh : public Component {

public:
	
	enum class Shape
	{
		CUBE,
		SPHERE,
		CYLINDER
	};

	ComponentMesh(GameObject* parent);
	ComponentMesh(GameObject* parent, Shape shape);
	~ComponentMesh();

	void CopyParMesh(par_shapes_mesh* parMesh);

	void GenerateBuffers();
	void ComputeNormals();
	void GenerateBounds();
	void DrawNormals() const;
	float3 GetCenterPointInWorldCoords() const;
	inline float GetSphereRadius() const { return radius; }

	bool SetFileValues(MeshFile* meshFile);

	void DrawAABB();

	bool Update(float dt) override;
	void OnGui() override;

	void OnLoad(const JSONReader& reader) override;
	void OnSave(JSONWriter& writer) const override;

public:

	std::vector<float3> vertices;
	std::vector<uint> indices;
	std::vector<float3> normals;
	std::vector<float2> texCoords;
	uint numIndices = 0;
	uint numVertices = 0;

	uint vertexBufferId = 0, indexBufferId = 0, textureBufferId = 0;
	std::string texturePath;
	uint numNormalFaces = 0;
	std::vector<float3> faceNormals;
	std::vector<float3> faceCenters;

	bool drawWireframe = false;
	bool drawVertexNormals = false;
	bool drawFaceNormals = false;
	float normalScale = 1.f;
	

	//Local coords AABB
	AABB localAABB;

	bool render = true;
private:

	//Bounding sphere
	float3 centerPoint = float3::zero;
	float radius;

	
};