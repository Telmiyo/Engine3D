#include "ComponentCanvas.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleUI.h"

#include "glew.h"

ComponentCanvas::ComponentCanvas(GameObject* parent) : ComponentTransform2D(parent)
{
}

bool ComponentCanvas::Update(float dt)
{
	AdjustPositionAndSize();
	//AdjustPositionAndSize();

	/*glBegin(GL_LINES);

	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(3.0f);

	float2 finalPosition = position;
	float2 finalSize;
	GetRealSize(finalSize);

	glVertex3f(finalPosition.x, finalPosition.y, 0);
	glVertex3f(finalPosition.x + finalSize.x, finalPosition.y, 0);

	glVertex3f(finalPosition.x + finalSize.x, finalPosition.y, 0);
	glVertex3f(finalPosition.x + finalSize.x, finalPosition.y + finalSize.y, 0);

	glVertex3f(finalPosition.x + finalSize.x, finalPosition.y + finalSize.y, 0);
	glVertex3f(finalPosition.x, finalPosition.y + finalSize.y, 0);

	glVertex3f(finalPosition.x, finalPosition.y + finalSize.y, 0);
	glVertex3f(finalPosition.x, finalPosition.y, 0);

	glEnd();*/

	return true;
}

void ComponentCanvas::OnGui()
{
	if (ImGui::CollapsingHeader("Canvas 2D"))
	{
		ImGui::Text("Size: %f, %f", size.x, size.y);
	}
}

void ComponentCanvas::AdjustPositionAndSize()
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	float propX = viewport[2] / App->editor->lastViewportSize.x;
	float propY = viewport[3] / App->editor->lastViewportSize.y;

	position.x = 0;
	position.y = 0;
	size.x = App->editor->lastViewportSize.x * propX;
	size.y = App->editor->lastViewportSize.y * propY;
}

void ComponentCanvas::GetRealPosition(float2& realPosition, bool ignoreCanvas)
{
	float propX = App->ui->uiCameraViewport[2] / App->editor->lastViewportSize.x;
	float propY = App->ui->uiCameraViewport[3] / App->editor->lastViewportSize.y;

	realPosition.x = 0;
	realPosition.y = 0;
}

void ComponentCanvas::GetRealSize(float2& realSize)
{
	float propX = App->ui->uiCameraViewport[2] / App->editor->lastViewportSize.x;
	float propY = App->ui->uiCameraViewport[3] / App->editor->lastViewportSize.y;

	realSize.x = App->editor->lastViewportSize.x * propX;
	realSize.y = App->editor->lastViewportSize.y * propY;
}
