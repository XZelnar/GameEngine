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
	void SetVertexCBuffer(int index, const D3DXVECTOR2& value);
	void SetVertexCBuffer(int index, const D3DXVECTOR3& value);
	void SetVertexCBuffer(int index, const D3DXVECTOR4& value);
	void SetVertexCBuffer(int index, const D3DMATRIX& value);
	void SetVertexCBuffer(int index, const float* value, int arrayLength);
	void SetGeometryCBuffer(int index, const D3DXVECTOR2& value);
	void SetGeometryCBuffer(int index, const D3DXVECTOR3& value);
	void SetGeometryCBuffer(int index, const D3DXVECTOR4& value);
	void SetGeometryCBuffer(int index, const D3DMATRIX& value);
	void SetGeometryCBuffer(int index, const float* value, int arrayLength);
	void SetPixelCBuffer(int index, const D3DXVECTOR2& value);
	void SetPixelCBuffer(int index, const D3DXVECTOR3& value);
	void SetPixelCBuffer(int index, const D3DXVECTOR4& value);
	void SetPixelCBuffer(int index, const D3DMATRIX& value);
	void SetPixelCBuffer(int index, const float* value, int arrayLength);
	void SetTexture(Texture* t, int index = 0);
	const Shader* GetShader() { return shader; }

	void SetInputLayout(const D3D11_INPUT_ELEMENT_DESC* ied3d, int numElements);

private:
	void SetCBuffer(int index, const D3DXVECTOR2& value, void** buffer, int bufferLength);
	void SetCBuffer(int index, const D3DXVECTOR3& value, void** buffer, int bufferLength);
	void SetCBuffer(int index, const D3DXVECTOR4& value, void** buffer, int bufferLength);
	void SetCBuffer(int index, const D3DMATRIX& value, void** buffer, int bufferLength);
	void SetCBuffer(int index, const float* value, int arrayLength, void** buffer, int bufferLength);

public:
	bool updateTxtures;

private:
	Shader* shader;
	Texture** textures;
	int texturesLength;
	void** cBufferVertexValues;
	void** cBufferGeometryValues;
	void** cBufferPixelValues;
	int cBuffersVertexLength;
	int cBuffersGeometryLength;
	int cBuffersPixelLength;
	bool isActive;
};