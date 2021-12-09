#pragma once

#include "Component.h"
#include "Globals.h"
#include "Math/float3.h"
#include "Math/float4x4.h"
#include "Geometry/Frustum.h"


class ComponentCamera : public Component {

public:

	ComponentCamera(GameObject* parent);
	~ComponentCamera();

	bool Update(float dt) override;

	//void LookAt(const float3& point);
	void CalculateViewMatrix();
	//void RecalculateProjection();

	void DrawCamera();
	void OnGui() override;

	void OnSave(JSONWriter& writer) const override;
	void OnLoad(const JSONReader& reader) override;

	Frustum cameraFrustum;
	float4x4 viewMatrix;

	float aspectRatio = 1.f;
	float verticalFOV = 60.f;
	float horizontalFOV = 60.f;
	float nearPlaneDistance = 0.1f;
	float farPlaneDistance = 5000.f;
	bool projectionIsDirty = false;

};