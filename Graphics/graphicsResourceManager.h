#pragma once

#include "../stdafx.h"

using namespace std;

class Mesh;
class Texture;
class Shader;

struct cmp_LPCWSTR
{
   bool operator()(LPCWSTR a, LPCWSTR b)
   {
	   return std::wcscmp(a, b) < 0;
   }
};

class GraphicsResourceManager
{
private:
	GraphicsResourceManager() { }
	GraphicsResourceManager(const GraphicsResourceManager& g) { }

public:
	static GraphicsResourceManager& GetInstance()
	{
		static GraphicsResourceManager i;
		return i;
	}

public:
	Mesh* GetMesh(LPCWSTR filename);
	Texture* GetTexture(LPCWSTR filename);
	Shader* GetShader(LPCWSTR filename);

	void LoadMesh(LPCWSTR filename);
	void LoadTexture(LPCWSTR filename);
	void LoadShader(LPCWSTR filename);

	bool Exists(LPCWSTR filename);
	bool ExistsTexture(LPCWSTR filename);
	bool ExistsMesh(LPCWSTR filename);
	bool ExistsShader(LPCWSTR filename);

	void Dispose();

private:
	Mesh* loadMesh(LPCWSTR filename);
	Texture* loadTexture(LPCWSTR filename);
	Shader* loadShader(LPCWSTR filename);

private:
	map<LPCWSTR, Mesh*, cmp_LPCWSTR> meshes;
	map<LPCWSTR, Texture*, cmp_LPCWSTR> textures;
	map<LPCWSTR, Shader*, cmp_LPCWSTR> shaders;

	int meshCount;
	int textureCount;
	int shaderCount;
};