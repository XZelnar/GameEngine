#pragma once

#include "../../stdafx.h"
#include "componentSpriteRenderer.h"

/// Differs from sprite renderer in that it draws texture in a non-stretched way
class ComponentGUIRenderer : public ComponentSpriteRenderer
{
public:
	ComponentGUIRenderer();
	virtual void Initialize(GameObject* _parent) { parent = _parent; };
	virtual void Dispose();
	void SetBorder(float b, float bPix);
	virtual void GUI();

protected:
	virtual void updateMatrix();
	virtual void createVBO();

protected:
	float border, borderPix;
	ID3D11Buffer* vBuffer;

private:
	struct VERTEX
	{
		D3DXVECTOR2 POSITION;
		float U, V;
	};
};