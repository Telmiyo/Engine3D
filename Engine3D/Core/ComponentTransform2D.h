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


	float2 position = { 0, 0 };
	float2 pivot = { 0, 0 };
	float rotation = 0.0f;
	float2 size = { 50, 50 };

	Anchor anchorType = Anchor::CENTER;
};

#endif