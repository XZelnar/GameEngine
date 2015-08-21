#pragma once

#include "../../stdafx.h"
#include "iComponent.h"

class Material;

class ComponentRenderer : public IGameObjectComponent
{
public:
	ComponentRenderer() : IGameObjectComponent() { }

	virtual const Material* GetMaterial() { return material; }
	virtual void SetMaterial(Material* e) { material = e; }
	void SetVisible(bool v) { IsVisible = v; }

protected:
	bool IsVisible;
	Material* material;
};