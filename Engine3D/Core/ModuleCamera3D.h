#pragma once
#include "Module.h"
#include "Globals.h"
#include "Math/float3.h"
#include "Math/float4x4.h"
#include "Geometry/Frustum.h"
#include "GameObject.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start() override;
	update_status Update(float dt)override;
	bool CleanUp() override;

	void LookAt(const float3&point);
	void CalculateViewMatrix();
	void RecalculateProjection();
	void OnGui() override;
	void OnSave(JSONWriter& writer) const override;
	void OnLoad(const JSONReader& reader) override;

	GameObject* GetGameObjectMousePicked();
	float2 WorldToScreen(float3 pos);

	float3 right, up, front, position, reference;
	Frustum cameraFrustum;
	float4x4 viewMatrix;
	float aspectRatio = 1.f;
	float verticalFOV = 60.f;
	float nearPlaneDistance = 0.1f;
	float farPlaneDistance = 5000.f;
	float cameraSensitivity = .5f; 
	float cameraSpeed = 60.f;
	bool projectionIsDirty = false;

private:

	float lastDeltaX = 0.f, lastDeltaY = 0.f;

	std::vector<GameObject*> hitList;
};