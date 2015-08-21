#pragma once

#include "GameObject/Component/componentModelRenderer.h"

class ComponentFurRenderer : public ComponentModelRenderer
{
public:
	virtual void Render();

public:
	int iterations;
	int materialIterVarIndex;
};