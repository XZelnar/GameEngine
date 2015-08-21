#pragma once

#include "../../stdafx.h"
#include "componentRenderer.h"

class Transformation;
class GameObject;
class Mesh;
class Texture;
class Material;

class ComponentModelRenderer : public ComponentRenderer
{
public:
	ComponentModelRenderer();
	virtual void Initialize(GameObject* parent);
	virtual void Dispose(){}
	virtual void Update(){}
	virtual void Render();

	void SetMesh(Mesh* m);
	Mesh* GetMesh() { return mesh; }

protected:
	Mesh* mesh;
	Transformation* pTransformation;
};