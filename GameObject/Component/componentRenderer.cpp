#include "componentRenderer.h"

#include "../gameObject.h"
#include "transformation.h"

void ComponentRenderer::Initialize(GameObject* _parent)
{
	parent = _parent;
	transform = parent->GetTransformation();
}

void ComponentRenderer::Render()
{
	Render(transform->GetTransformationMatrix());
}

void ComponentRenderer::GUI()
{
	GUI(transform->GetTransformationMatrix());
}