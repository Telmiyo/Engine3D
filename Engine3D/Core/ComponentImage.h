#ifndef _COMPONENT_IMAGE_H_
#define _COMPONENT_IMAGE_H_

#include "Component.h"
#include "ComponentTransform2D.h"

#include "glew.h"

class ComponentMesh;

class ComponentImage : public Component {
public:
	ComponentImage(GameObject* parent);

	bool Update(float dt) override;
	void OnGui() override;

private:
	ComponentMesh* plane = nullptr;
};

#endif