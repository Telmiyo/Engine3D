#include "ComponentTransform2D.h"

ComponentTransform2D::ComponentTransform2D(GameObject* parent) : Component(parent)
{
}

bool ComponentTransform2D::Update(float dt)
{
	return false;
}

void ComponentTransform2D::OnGui()
{
}
