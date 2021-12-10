#pragma once

#include "Component.h"
#include "Globals.h"
#include "MathGeoLib.h"
#include "Math/float3.h"
#include "Math/float4x4.h"
#include "Geometry/Frustum.h"


class ComponentCamera : public Component {

public:
	ComponentCamera(GameObject* parent);
	~ComponentCamera();

	bool Update(float dt) override;

	void CalculateViewMatrix();
	void RecalculateProjection();
	//Formulas from the power point
	void SetHorizontalFov(float value);
	void SetVerticalFov(float value);
	
	//Setters
	void UpdateAspectRatio();
	void UpdateHorizontalFov();
	void UpdateVerticalFov();

	void DrawCamera();
	void OnGui() override;

	void OnSave(JSONWriter& writer) const override;
	void OnLoad(const JSONReader& reader) override;

	Frustum cameraFrustum;
	float4x4 viewMatrix;

	float aspectRatio = 1.f;
	float verticalFOV = 30.f;
	float horizontalFOV = 80.f;
	float horizontalFOVGui = 80.f;
	float verticalFOVGui = 80.f;
	float nearPlaneDistance = 10.f;
	float farPlaneDistance = 50.f;
	bool projectionIsDirty = false;

};