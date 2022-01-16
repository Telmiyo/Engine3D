#ifndef _COMPONENT_IMAGE_H_
#define _COMPONENT_IMAGE_H_

#include "Component.h"
#include "ComponentTransform2D.h"

#include "glew.h"

#include "Math/float2.h"
#include "Globals.h"
#include <string.h>
#include <vector>

class ComponentMesh;
struct TextureObject;

class ComponentImage : public Component {
public:
	ComponentImage(GameObject* parent);

	bool Update(float dt) override;
	void SetTexture(const TextureObject& texture);
	void OnGui() override;

private:
	ComponentMesh* plane = nullptr;
	std::string texturePath;
	uint textureBufferId = 0;


	std::string textureName;
	uint textureId = 0, width = 0, height = 0;
};

#endif

