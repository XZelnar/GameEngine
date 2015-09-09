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

	color = D3DXVECTOR4(1, 1, 1, 1);

	for (int i = 0; i < texturesLength; i++)
		textures[i] = null;
}

Material::Material(Material& m)
{
	updateTxtures = m.updateTxtures;
	isActive = false;

	shader = m.shader;
	textures = new Texture*[m.texturesLength];
	texturesLength = m.texturesLength;

	color = m.color;

	for (int i = 0; i < texturesLength; i++)
		textures[i] = null;
	memcpy(textures, m.textures, sizeof(Texture*) * texturesLength);

}

Material::~Material()
{
	if (isActive)
	{
		MaterialsManager::setActive(null);
		shader->Deactivate();
	}

	delete[] textures;
}

void Material::Activate()
{
	if (MaterialsManager::isActive(this))
		return;
	ShaderManager::SetActiveShader(shader);
	shader->SetCBufferPerObjectColor(&color);

	if (updateTxtures)
		for (int i = 0; i < texturesLength; i++)
			textures[i]->ApplyTexture(i);

	MaterialsManager::setActive(this);
}

void Material::SetInputLayout(const D3D11_INPUT_ELEMENT_DESC* ied3d, int numElements)
{
	if (shader)
		shader->SetInputLayout(ied3d, numElements);
}



void Material::SetCBuffer(std::string name, const D3DXVECTOR2& value)
{
	shader->SetCBuffer(name, (void*)&value);
}

void Material::SetCBuffer(std::string name, const D3DXVECTOR3& value)
{
	shader->SetCBuffer(name, (void*)&value);
}

void Material::SetCBuffer(std::string name, const D3DXVECTOR4& value)
{
	shader->SetCBuffer(name, (void*)&value);
}

void Material::SetCBuffer(std::string name, const D3DMATRIX& value)
{
	shader->SetCBuffer(name, (void*)&value);
}

void Material::SetCBuffer(std::string name, const void* value)
{
	shader->SetCBuffer(name, (void*)&value);
}

void Material::SetColor(const D3DXVECTOR4& value)
{
	color = value;
	if (isActive)
		shader->SetCBufferPerObjectColor((void*)&value);
}

void Material::SetTexture(Texture* t, int index)
{
	assert(index >= 0 && index < texturesLength);

	textures[index] = t;
}