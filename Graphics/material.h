#pragma once

#include "../stdafx.h"

class Shader;
class Texture;

class Material
{
	friend class MaterialsManager;

public:
	Material(Shader* s);
	Material(Material& mat);
	~Material();
	void Activate();
	void SetTexture(Texture* t, int index = 0);
	const Shader* GetShader() { return shader; }

	void SetInputLayout(const D3D11_INPUT_ELEMENT_DESC* ied3d, int numElements);

	void SetCBuffer(std::string name, const D3DXVECTOR2& value);
	void SetCBuffer(std::string name, const D3DXVECTOR3& value);
	void SetCBuffer(std::string name, const D3DXVECTOR4& value);
	void SetCBuffer(std::string name, const D3DMATRIX& value);
	void SetCBuffer(std::string name, const void* value);
	void SetColor(const D3DXVECTOR4& value);

	D3DXVECTOR4 GetColor() { return color; }

public:
	bool updateTxtures;

private:
	Shader* shader;
	Texture** textures;
	int texturesLength;
	bool isActive;
	D3DXVECTOR4 color;
};