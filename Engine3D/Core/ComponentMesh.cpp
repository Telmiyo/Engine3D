#include "ComponentMesh.h"

#include "glew.h"
#include "SDL/include/SDL_opengl.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "ImGui/imgui.h"
#include "Geometry/Sphere.h"
#include "par_shapes.h"


ComponentMesh::ComponentMesh(GameObject* parent) : Component(parent) {

	componentType = ComponentType::COMPONENT_MESH;
}

ComponentMesh::ComponentMesh(GameObject* parent, Shape shape) : Component(parent)
{
	switch (shape)
	{
	case Shape::CUBE:
		CopyParMesh(par_shapes_create_cube());
		break;
	case Shape::CYLINDER:
		CopyParMesh(par_shapes_create_cylinder(20, 20));
		break;
	case Shape::SPHERE:
		CopyParMesh(par_shapes_create_parametric_sphere(20, 20));
		break;
	}
}

ComponentMesh::~ComponentMesh()
{
	vertexBufferId ? glDeleteBuffers(1, &vertexBufferId) : 0;
	textureBufferId ? glDeleteBuffers(1, &textureBufferId) : 0;
	indexBufferId ? glDeleteBuffers(1, &indexBufferId) : 0;
}

void ComponentMesh::CopyParMesh(par_shapes_mesh* parMesh)
{
	numVertices = parMesh->npoints;
	numIndices = parMesh->ntriangles * 3;
	numNormalFaces = parMesh->ntriangles;
	vertices.resize(numVertices);
	normals.resize(numVertices);
	indices.resize(numIndices);
	par_shapes_compute_normals(parMesh);

	for (size_t i = 0; i < numVertices; i +=2)
	{
		float u = *(parMesh->tcoords + i);
		float v = *(parMesh->tcoords + (i + 1));
		texCoords.push_back((float2(*(parMesh->tcoords + i), *(parMesh->tcoords +( i + 1)))));
		
		//*(file->textCoords_ + i * 2) = m->mTextureCoords[0][i].x;
		//*(file->textCoords_ + i * 2 + 1) = m->mTextureCoords[0][i].y; //this coord image is inverted
	
	}

	//TexCoords(u,v|u,v|u,v)

	for (size_t i = 0; i < numVertices; ++i)
	{
		memcpy(&vertices[i], &parMesh->points[i * 3], sizeof(float) * 3);
		memcpy(&normals[i], &parMesh->normals[i * 3], sizeof(float) * 3);
	}
	for (size_t i = 0; i < indices.size(); ++i)
	{
		indices[i] = parMesh->triangles[i];
	}
	memcpy(&normals[0], parMesh->normals, numVertices);

	par_shapes_free_mesh(parMesh);

	GenerateBuffers();
	ComputeNormals();
	GenerateBounds();
}


void ComponentMesh::GenerateBuffers() {

	//-- Generate Vertex
	vertexBufferId = 0;
	glGenBuffers(1, &vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * numVertices, &vertices[0], GL_STATIC_DRAW);

	//-- Generate Index
	indexBufferId = 0;
	glGenBuffers(1, &indexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * numIndices, &indices[0], GL_STATIC_DRAW);

	//-- Generate Texture_Buffers
	if (texCoords.size() != 0)
	{
		textureBufferId = 0;
		glGenBuffers(1, &textureBufferId);
		glBindBuffer(GL_ARRAY_BUFFER, textureBufferId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float2) * texCoords.size(), &texCoords[0], GL_STATIC_DRAW);
	}
	if (vertexBufferId == 0 || indexBufferId == 0)
		LOG("Error creating mesh on gameobject %s", owner->name.c_str());
}

void ComponentMesh::ComputeNormals()
{

	numNormalFaces = numIndices * 3;
	faceNormals.resize(numNormalFaces);
	faceCenters.resize(numNormalFaces);

	for (size_t i = 0; i < numIndices; i += 3)
	{
		const float3 p0 = vertices[indices[i + 1]] - vertices[indices[i]];
		const float3 p1 = vertices[indices[i + 2]] - vertices[indices[i]];

		const float3 faceNormal = float3(p0.x, p0.y, p0.z).Cross(float3(p1.x, p1.y, p1.z));
		faceNormals[i / 3] = faceNormal.Normalized();

		const float3 faceCenter = (vertices[indices[i]] + vertices[indices[i + 1]] + vertices[indices[i + 2]]) / 3.f;
		faceCenters[i / 3] = faceCenter;
	}
	int kk = 0;
}

void ComponentMesh::GenerateBounds()
{

	localAABB.SetNegativeInfinity();
	localAABB.Enclose(&vertices[0], vertices.size());

	Sphere sphere;
	sphere.r = 0.f;
	sphere.pos = localAABB.CenterPoint();
	sphere.Enclose(localAABB);



	radius = sphere.r;
	centerPoint = sphere.pos;
}

void ComponentMesh::UpdateBounds()
{
	obb.SetFrom(localAABB);
	obb.Transform(owner->GetComponent<ComponentTransform>()->transformMatrix);

	localAABB.SetNegativeInfinity();
	localAABB.Enclose(obb);

}

void ComponentMesh::DrawNormals() const
{
	if (drawFaceNormals)
	{
		for (size_t i = 0; i < faceNormals.size(); ++i)
		{
			glColor3f(0.f, 0.f, 1.f);
			glBegin(GL_LINES);
			const float3 faceCenter = owner->transform->transformMatrix.TransformPos(faceCenters[i]);
			const float3 faceNormalPoint = faceCenter + faceNormals[i] * normalScale;
			glVertex3f(faceCenter.x, faceCenter.y, faceCenter.z);
			glVertex3f(faceNormalPoint.x, faceNormalPoint.y, faceNormalPoint.z);
			glEnd();
		}
	}
	if (drawVertexNormals)
	{
		for (size_t i = 0; i < normals.size(); ++i)
		{
			glColor3f(1.f, 0.f, 0.f);
			glBegin(GL_LINES);
			const float3 vertexPos = owner->transform->transformMatrix.TransformPos(vertices[i]);
			const float3 vertexNormalPoint = vertexPos + normals[i] * normalScale;
			glVertex3f(vertexPos.x, vertexPos.y, vertexPos.z);
			glVertex3f(vertexNormalPoint.x, vertexNormalPoint.y, vertexNormalPoint.z);
			glEnd();
		}
	}
}

float3 ComponentMesh::GetCenterPointInWorldCoords() const
{
	return owner->transform->transformMatrix.TransformPos(centerPoint);
}

bool ComponentMesh::SetFileValues(MeshFile* meshFile)
{
	vertices = meshFile->vecVertices;
	normals = meshFile->vecNormals;
	indices = meshFile->vecIndices;
	texCoords = meshFile->vecTextCoords;

	numVertices = vertices.size();
	numIndices = indices.size();

	texturePath = meshFile->texturePath;

	GenerateBuffers();
	GenerateBounds();
	ComputeNormals();

	return true;
}

void ComponentMesh::DrawAABB()
{
	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	glLineWidth(3.5f);
	glBegin(GL_LINES);
	float3 corners[8];
	localAABB.GetCornerPoints(corners);
	glVertex3f(corners[0].x, corners[0].y, corners[0].z);
	glVertex3f(corners[1].x, corners[1].y, corners[1].z);

	glVertex3f(corners[1].x, corners[1].y, corners[1].z);
	glVertex3f(corners[3].x, corners[3].y, corners[3].z);

	glVertex3f(corners[0].x, corners[0].y, corners[0].z);
	glVertex3f(corners[2].x, corners[2].y, corners[2].z);

	glVertex3f(corners[2].x, corners[2].y, corners[2].z);
	glVertex3f(corners[3].x, corners[3].y, corners[3].z);

	glVertex3f(corners[4].x, corners[4].y, corners[4].z);
	glVertex3f(corners[5].x, corners[5].y, corners[5].z);

	glVertex3f(corners[0].x, corners[0].y, corners[0].z);
	glVertex3f(corners[4].x, corners[4].y, corners[4].z);

	glVertex3f(corners[1].x, corners[1].y, corners[1].z);
	glVertex3f(corners[5].x, corners[5].y, corners[5].z);

	glVertex3f(corners[5].x, corners[5].y, corners[5].z);
	glVertex3f(corners[7].x, corners[7].y, corners[7].z);

	glVertex3f(corners[6].x, corners[6].y, corners[6].z);
	glVertex3f(corners[7].x, corners[7].y, corners[7].z);

	glVertex3f(corners[4].x, corners[4].y, corners[4].z);
	glVertex3f(corners[6].x, corners[6].y, corners[6].z);

	glVertex3f(corners[6].x, corners[6].y, corners[6].z);
	glVertex3f(corners[2].x, corners[2].y, corners[2].z);

	glVertex3f(corners[7].x, corners[7].y, corners[7].z);
	glVertex3f(corners[3].x, corners[3].y, corners[3].z);

	glEnd();
}

bool ComponentMesh::Update(float dt)
{

	//Update AABB Position
	GenerateBounds();
	UpdateBounds();
	DrawAABB();

	if (render)
	{
		drawWireframe || App->renderer3D->wireframeMode ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glBindTexture(GL_TEXTURE_2D, 0);

		//--Enable States--//
		glEnableClientState(GL_VERTEX_ARRAY);

		//-- Buffers--//
		if (this->textureBufferId)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, this->textureBufferId);
			glTexCoordPointer(2, GL_FLOAT, 0, NULL);
		}

		glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferId);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		if (ComponentMaterial* material = owner->GetComponent<ComponentMaterial>())
		{
			drawWireframe || !App->renderer3D->useTexture || App->renderer3D->wireframeMode ? 0 : glBindTexture(GL_TEXTURE_2D, material->GetTextureId());
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBufferId);

		//-- Draw --//
		glPushMatrix();
		glMultMatrixf(owner->transform->transformMatrix.Transposed().ptr());
		glColor3f(1.0f, 1.0f, 1.0f);
		glDrawElements(GL_TRIANGLES, this->numIndices, GL_UNSIGNED_INT, NULL);
		glPopMatrix();
		//-- UnBind Buffers--//
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		if (this->textureBufferId)
		{
			glBindBuffer(GL_TEXTURE_COORD_ARRAY, 0);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		}

		glBindTexture(GL_TEXTURE_2D, 0);

		//--Disables States--//
		glDisableClientState(GL_VERTEX_ARRAY);

		App->renderer3D->wireframeMode ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		if (drawFaceNormals || drawVertexNormals)
			DrawNormals();

		//UPDATE BBAB
		//localAABB.SetFromCenterAndSize(owner->GetComponent<ComponentTransform>()->GetPosition(), owner->GetComponent<ComponentTransform>()->GetScale());
		//float3x3 aabbPos = float3x3();

		//aabbPos.Set(owner->GetComponent<ComponentTransform>()->GetPosition(), owner->GetComponent<ComponentTransform>()->GetRotation(), owner->GetComponent<ComponentTransform>()->GetScale());
		//localAABB.TransformAsAABB();

	}
	return true;
}

void ComponentMesh::OnGui()
{
	if (ImGui::CollapsingHeader("Mesh"))
	{
		ImGui::Text("Num vertices %d", numVertices);
		ImGui::Text("Num faces %d", numIndices / 3);
		ImGui::Checkbox("Wireframe", &drawWireframe);
		ImGui::DragFloat("Normal draw scale", &normalScale);
		ImGui::Checkbox("Draw face normals", &drawFaceNormals);
		ImGui::Checkbox("Draw vertex normals", &drawVertexNormals);
	}
}

void ComponentMesh::OnLoad(const JSONReader& reader)
{
	SetFileValues(App->resources->LoadMeshFile(owner->name));
	GenerateBuffers();

}
void ComponentMesh::OnSave(JSONWriter& writer) const
{
	writer.String("Mesh");
	writer.StartObject();
	writer.EndObject();
}


