#include "graphicsResourceManager.h"

#include "mesh.h"
#include "shader.h"
#include "shaderManager.h"
#include "texture.h"

Mesh* GraphicsResourceManager::GetMesh(LPCWSTR filename)
{
	Mesh* r;
	r = meshes[filename];
	if (r == NULL)
		r = loadMesh(filename);
	return r;
}

Texture* GraphicsResourceManager::GetTexture(LPCWSTR filename)
{
	Texture* r;
	r = textures[filename];
	if (r == NULL)
		r = loadTexture(filename);
	return r;
}

Shader* GraphicsResourceManager::GetShader(LPCWSTR filename)
{
	Shader* r;
	r = shaders[filename];
	if (r == NULL)
		r = loadShader(filename);
	return r;
}



void GraphicsResourceManager::LoadMesh(LPCWSTR filename)
{
	if (!ExistsMesh(filename))
		loadMesh(filename);
}

void GraphicsResourceManager::LoadTexture(LPCWSTR filename)
{
	if (!ExistsTexture(filename))
		loadTexture(filename);
}

void GraphicsResourceManager::LoadShader(LPCWSTR filename)
{
	if (!ExistsShader(filename))
		loadShader(filename);
}



bool GraphicsResourceManager::Exists(LPCWSTR filename)
{
	return ExistsTexture(filename) || ExistsMesh(filename) || ExistsShader(filename);
}

bool GraphicsResourceManager::ExistsTexture(LPCWSTR filename)
{
	return textures[filename] != NULL;
}

bool GraphicsResourceManager::ExistsMesh(LPCWSTR filename)
{
	return meshes[filename] != NULL;
}

bool GraphicsResourceManager::ExistsShader(LPCWSTR filename)
{
	return shaders[filename] != NULL;
}



void GraphicsResourceManager::Dispose()
{
	map<LPCWSTR, Mesh*, cmp_LPCWSTR>::iterator im = meshes.begin();
	for (int i = 0; i < meshCount; i++)
	{
		im->second->Dispose();
		delete im->second;
		im++;
	}
	meshes.clear();
	meshCount = 0;
	
	map<LPCWSTR, Texture*, cmp_LPCWSTR>::iterator it = textures.begin();
	for (int i = 0; i < textureCount; i++)
	{
		it->second->Dispose();
		delete it->second;
		it++;
	}
	textures.clear();
	textureCount = 0;
	
	map<LPCWSTR, Shader*, cmp_LPCWSTR>::iterator is = shaders.begin();
	for (int i = 0; i < shaderCount; i++)
	{
		if (is->second)
			is->second->Dispose();
		is++;
	}
	shaders.clear();
	shaderCount = 0;
}



Mesh* GraphicsResourceManager::loadMesh(LPCWSTR filename)
{
	Mesh* m = Mesh::LoadFromFile(filename);
	if (m != NULL)
	{
		meshes[filename] = m;
		meshCount++;
	}
	return m;
}

Texture* GraphicsResourceManager::loadTexture(LPCWSTR filename)
{
	Texture* t = Texture::LoadFromFile(filename);
	if (t != NULL)
	{
		textures[filename] = t;
		textureCount++;
	}
	return t;
}

Shader* GraphicsResourceManager::loadShader(LPCWSTR filename)
{
	Shader* s = Shader::LoadFromFile(filename);
	if (s != NULL)
	{
		shaders[filename] = s;
		shaderCount++;
	}
	return s;
}