#include "material.h"

#include "shader.h"
#include "shaderManager.h"
#include "texture.h"
#include "materialsManager.h"



Material::Material(Shader* s)
{
	updateTxtures = true;
	isActive = false;

	shader = s;
	textures = new Texture*[s->sTexturesCount];
	texturesLength = s->sTexturesCount;
	cBufferVertexValues = new void*[s->cBuffersVertexLength];
	cBuffersVertexLength = s->cBuffersVertexLength;
	cBufferGeometryValues = new void*[s->cBuffersGeometryLength];
	cBuffersGeometryLength = s->cBuffersGeometryLength;
	cBufferPixelValues = new void*[s->cBuffersPixelLength];
	cBuffersPixelLength = s->cBuffersPixelLength;

	for (int i = 0; i < texturesLength; i++)
		textures[i] = null;
	for (int i = 3; i < cBuffersVertexLength; i++)
		cBufferVertexValues[i] = new byte[shader->cBuffersVertexSizes[i]];
	for (int i = 0; i < cBuffersGeometryLength; i++)
		cBufferGeometryValues[i] = new byte[shader->cBuffersGeometrySizes[i]];
	for (int i = 0; i < cBuffersPixelLength; i++)
		cBufferPixelValues[i] = new byte[shader->cBuffersPixelSizes[i]];
}

Material::Material(Material& m)
{
	updateTxtures = m.updateTxtures;
	isActive = false;

	shader = m.shader;
	textures = new Texture*[m.texturesLength];
	texturesLength = m.texturesLength;
	cBufferVertexValues = new void*[shader->cBuffersVertexLength];
	cBuffersVertexLength = shader->cBuffersVertexLength;
	cBufferGeometryValues = new void*[shader->cBuffersGeometryLength];
	cBuffersGeometryLength = shader->cBuffersGeometryLength;
	cBufferPixelValues = new void*[shader->cBuffersPixelLength];
	cBuffersPixelLength = shader->cBuffersPixelLength;

	for (int i = 0; i < texturesLength; i++)
		textures[i] = null;
	memcpy(textures, m.textures, sizeof(Texture*) * texturesLength);
	for (int i = 3; i < cBuffersVertexLength; i++)
	{
		cBufferVertexValues[i] = new byte[shader->cBuffersVertexSizes[i]];
		memcpy(cBufferVertexValues[i], m.cBufferVertexValues[i], shader->cBuffersVertexSizes[i]);
	}
	for (int i = 0; i < cBuffersGeometryLength; i++)
	{
		cBufferGeometryValues[i] = new byte[shader->cBuffersGeometrySizes[i]];
		memcpy(cBufferGeometryValues[i], m.cBufferGeometryValues[i], shader->cBuffersGeometrySizes[i]);
	}
	for (int i = 0; i < cBuffersPixelLength; i++)
	{
		cBufferPixelValues[i] = new byte[shader->cBuffersPixelSizes[i]];
		memcpy(cBufferPixelValues[i], m.cBufferPixelValues[i], shader->cBuffersPixelSizes[i]);
	}

}

Material::~Material()
{
	if (isActive)
	{
		MaterialsManager::setActive(null);
		shader->Deactivate();
	}

	delete[] textures;
	for (int i = 3; i < cBuffersVertexLength; i++)
		delete[] cBufferVertexValues[i];
	delete[] cBufferVertexValues;
	for (int i = 0; i < cBuffersGeometryLength; i++)
		delete[] cBufferGeometryValues[i];
	delete[] cBufferGeometryValues;
	for (int i = 0; i < cBuffersPixelLength; i++)
		delete[] cBufferPixelValues[i];
	delete[] cBufferPixelValues;
}

void Material::Activate()
{
	ShaderManager::SetActiveShader(shader);

	if (updateTxtures)
		for (int i = 0; i < texturesLength; i++)
			textures[i]->ApplyTexture(i);

	for (int i = 3; i < cBuffersVertexLength; i++)
		shader->SetVertexCBuffer(i, cBufferVertexValues[i]);

	for (int i = 0; i < cBuffersGeometryLength; i++)
		shader->SetGeometryCBuffer(i, cBufferGeometryValues[i]);

	for (int i = 0; i < cBuffersPixelLength; i++)
		shader->SetPixelCBuffer(i, cBufferPixelValues[i]);

	MaterialsManager::setActive(this);
}

void Material::SetInputLayout(const D3D11_INPUT_ELEMENT_DESC* ied3d, int numElements)
{
	if (shader)
		shader->SetInputLayout(ied3d, numElements);
}

//vertex

void Material::SetVertexCBuffer(int index, const D3DXVECTOR2& value)
{
	assert(index >= 3 && index < cBuffersVertexLength);
	SetCBuffer(index, value, cBufferVertexValues, cBuffersVertexLength);
	if (isActive)
		shader->SetVertexCBuffer(index, cBufferVertexValues[index]);
}

void Material::SetVertexCBuffer(int index, const D3DXVECTOR3& value)
{
	assert(index >= 3 && index < cBuffersVertexLength);
	SetCBuffer(index, value, cBufferVertexValues, cBuffersVertexLength);
	if (isActive)
		shader->SetVertexCBuffer(index, cBufferVertexValues[index]);
}

void Material::SetVertexCBuffer(int index, const D3DXVECTOR4& value)
{
	assert(index >= 3 && index < cBuffersVertexLength);
	SetCBuffer(index, value, cBufferVertexValues, cBuffersVertexLength);
	if (isActive)
		shader->SetVertexCBuffer(index, cBufferVertexValues[index]);
}

void Material::SetVertexCBuffer(int index, const D3DMATRIX& value)
{
	assert(index >= 3 && index < cBuffersVertexLength);
	SetCBuffer(index, value, cBufferVertexValues, cBuffersVertexLength);
	if (isActive)
		shader->SetVertexCBuffer(index, cBufferVertexValues[index]);
}

void Material::SetVertexCBuffer(int index, const float* value, int arrayLength)
{
	assert(index >= 3 && index < cBuffersVertexLength);
	SetCBuffer(index, value, arrayLength, cBufferVertexValues, cBuffersVertexLength);
	if (isActive)
		shader->SetVertexCBuffer(index, cBufferVertexValues[index]);
}

//geometry

void Material::SetGeometryCBuffer(int index, const D3DXVECTOR2& value)
{
	SetCBuffer(index, value, cBufferGeometryValues, cBuffersGeometryLength);
	if (isActive)
		shader->SetGeometryCBuffer(index, cBufferGeometryValues[index]);
}

void Material::SetGeometryCBuffer(int index, const D3DXVECTOR3& value)
{
	SetCBuffer(index, value, cBufferGeometryValues, cBuffersGeometryLength);
	if (isActive)
		shader->SetGeometryCBuffer(index, cBufferGeometryValues[index]);
}

void Material::SetGeometryCBuffer(int index, const D3DXVECTOR4& value)
{
	SetCBuffer(index, value, cBufferGeometryValues, cBuffersGeometryLength);
	if (isActive)
		shader->SetGeometryCBuffer(index, cBufferGeometryValues[index]);
}

void Material::SetGeometryCBuffer(int index, const D3DMATRIX& value)
{
	SetCBuffer(index, value, cBufferGeometryValues, cBuffersGeometryLength);
	if (isActive)
		shader->SetGeometryCBuffer(index, cBufferGeometryValues[index]);
}

void Material::SetGeometryCBuffer(int index, const float* value, int arrayLength)
{
	SetCBuffer(index, value, arrayLength, cBufferGeometryValues, cBuffersGeometryLength);
	if (isActive)
		shader->SetGeometryCBuffer(index, cBufferGeometryValues[index]);
}

//pixel

void Material::SetPixelCBuffer(int index, const D3DXVECTOR2& value)
{
	SetCBuffer(index, value, cBufferPixelValues, cBuffersPixelLength);
	if (isActive)
		shader->SetPixelCBuffer(index, cBufferPixelValues[index]);
}

void Material::SetPixelCBuffer(int index, const D3DXVECTOR3& value)
{
	SetCBuffer(index, value, cBufferPixelValues, cBuffersPixelLength);
	if (isActive)
		shader->SetPixelCBuffer(index, cBufferPixelValues[index]);
}

void Material::SetPixelCBuffer(int index, const D3DXVECTOR4& value)
{
	SetCBuffer(index, value, cBufferPixelValues, cBuffersPixelLength);
	if (isActive)
		shader->SetPixelCBuffer(index, cBufferPixelValues[index]);
}

void Material::SetPixelCBuffer(int index, const D3DMATRIX& value)
{
	SetCBuffer(index, value, cBufferPixelValues, cBuffersPixelLength);
	if (isActive)
		shader->SetPixelCBuffer(index, cBufferPixelValues[index]);
}

void Material::SetPixelCBuffer(int index, const float* value, int arrayLength)
{
	SetCBuffer(index, value, arrayLength, cBufferPixelValues, cBuffersPixelLength);
	if (isActive)
		shader->SetPixelCBuffer(index, cBufferPixelValues[index]);
}



//private

void Material::SetCBuffer(int index, const D3DXVECTOR2& value, void** buffer, int bufferLength)
{
	assert(index >= 0 && index < bufferLength);
	*((D3DXVECTOR2*)buffer[index]) = D3DXVECTOR2(value);
}

void Material::SetCBuffer(int index, const D3DXVECTOR3& value, void** buffer, int bufferLength)
{
	assert(index >= 0 && index < bufferLength);
	*((D3DXVECTOR3*)buffer[index]) = D3DXVECTOR3(value);
}

void Material::SetCBuffer(int index, const D3DXVECTOR4& value, void** buffer, int bufferLength)
{
	assert(index >= 0 && index < bufferLength);
	*((D3DXVECTOR4*)buffer[index]) = D3DXVECTOR4(value);
}

void Material::SetCBuffer(int index, const D3DMATRIX& value, void** buffer, int bufferLength)
{
	assert(index >= 0 && index < bufferLength);
	*((D3DMATRIX*)buffer[index]) = D3DMATRIX(value);
}

void Material::SetCBuffer(int index, const float* value, int arrayLength, void** buffer, int bufferLength)
{
	assert(index >= 0 && index < bufferLength);
	//assert(arrayLength * 4 <= shader->cBuffersSizes[index]);
	for (int i = 0; i < arrayLength; i++)
		((float*)buffer[index])[i] = value[i];
}

void Material::SetTexture(Texture* t, int index)
{
	assert(index >= 0 && index < texturesLength);

	textures[index] = t;
}