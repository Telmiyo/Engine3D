#ifndef _COMPONENT_TRANSFORM_2D_H_
#define _COMPONENT_TRANSFORM_2D_H_

#include "Component.h"
#include "Math/float2.h"

class ComponentTransform2D : public Component {
public:
	enum class Anchor {
		TOP_LEFT,
		TOP_CENTER,
		TOP_RIGHT,
		LEFT,
		CENTER,
		RIGHT,
		BOTTOM_LEFT,
		BOTTOM_CENTER,
		BOTTOM_RIGHT,
	};
public:
	ComponentTransform2D(GameObject* parent);

	bool Update(float dt) override;
	void OnGui() override;

	void GetScreenRect(float2& a, float2& b);

	void SetPosition(const float2& newPosition);
	void SetPivot(const float2& newPivot);
	void SetRotation(const float& newRotation);
	void SetSize(const float2& newSize);
	void SetAnchor(const Anchor& newAnchor);

	inline float2 GetPosition() const { return position; };
	inline float2 GetPivot() const { return pivot; };
	inline float GetRotation() const { return rotation; };
	inline float2 GetSize() const { return size; };
	inline Anchor GetAnchor() const { return anchor; };



	float2 position = { 0, 0 };
	float2 pivot = { 0, 0 };
	float rotation = 0.0f;
	float2 size = { 50, 50 };

	Anchor anchor = Anchor::CENTER;
};

#endif