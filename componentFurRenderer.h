#pragma once

#include "GameObject/Component/componentModelRenderer.h"

class ComponentFurRenderer : public ComponentModelRenderer
{
public:
	virtual void Render(D3DXMATRIX* world);

public:
	int iterations;
};