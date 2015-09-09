#pragma once

#include "../../stdafx.h"
#include "componentRenderer.h"

class Texture;
class Material;

class ComponentSpriteRenderer : public ComponentRenderer
{
public:
	ComponentSpriteRenderer();
	virtual void Initialize(GameObject* _parent) { parent = _parent; };
	virtual void Dispose();
	virtual Texture* GetTexture() { return texture; }
	virtual D3DXVECTOR2 GetSize() { return size; }
	virtual float GetRotation() { return rotation; }
	virtual D3DXVECTOR2 GetPosition() { return position; }
	virtual void SetTexture(Texture* t);
	virtual void SetSize(D3DXVECTOR2 s);
	virtual void SetRotation(float rot);
	virtual void SetPosition(D3DXVECTOR2 pos);
	virtual void Update() { }
	virtual void Render(D3DXMATRIX* world) { }
	virtual void GUI() { GUI(&mWorld); }
	virtual void GUI(D3DXMATRIX* world);

protected:
	virtual void updateMatrix();

protected:
	Texture* texture;
	ID3D11DeviceContext* devcon;
	D3DXVECTOR2 position;
	float rotation;
	D3DXVECTOR2 size;
	D3DXMATRIX mWorld, tpPos, tpRot, tpScale;

public://static
	static void StaticDispose();

private://static
	static void StaticInit();

private://static
	struct VERTEX
	{
		D3DXVECTOR2 POSITION;
		float U, V;
	};
	static ID3D11Buffer* vBuffer;
};