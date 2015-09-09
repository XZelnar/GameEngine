#pragma once

#include "../../stdafx.h"
#include "iComponent.h"

class Material;
class Transformation;

class ComponentRenderer : public IGameObjectComponent
{
public:
	ComponentRenderer() : IGameObjectComponent() { }

	virtual const Material* GetMaterial() { return material; }
	virtual void SetMaterial(Material* e) { material = e; }
	void SetVisible(bool v) { IsVisible = v; }

	virtual void Initialize(GameObject* _parent);
	virtual void Render();
	virtual void Render(D3DXMATRIX* world) = 0;
	virtual void GUI();
	virtual void GUI(D3DXMATRIX* world) { }

protected:
	bool IsVisible;
	Material* material;
	Transformation* transform;
};