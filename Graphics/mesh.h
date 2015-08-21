#pragma once

#include "../stdafx.h"

class Shader;

class Mesh
{
	friend class ComponentColliderConvexVertices;

public://static
	struct VERTEX
	{
		D3DXVECTOR3 POSITION;
		D3DVECTOR NORMAL;
		D3DXVECTOR2 TEXCOORD;
	};

public:
	Mesh();
	void Dispose();
	void Render();
	void SetVertices(D3DXVECTOR3* vertices, D3DXVECTOR3* normals, D3DXVECTOR4* colors, D3DXVECTOR2* texcoords, int length, bool _updateVBO);
	void SetRawData(void* _data, int vertexCount, int _sizeOfData, UINT _stride);
	void updateVBO();
	int GetVertexCount() { return verticesCount; }
#if DEBUG
	void SetName(char* name);
#endif

	const wchar_t* GetFilename() { return filename; }

private:
	void createVBO();
	const VERTEX* GetVertices() { return vertices; }

public://static
	static Mesh* LoadFromFile(LPCWSTR filename);
	static Mesh* CreateFromVertices(D3DXVECTOR3* vertices, D3DXVECTOR3* normals, D3DXVECTOR2* texcoords, int length, D3D11_PRIMITIVE_TOPOLOGY topology_, bool _createVBO);
	static Mesh* CreateFromRawData(void* _data, int vertexCount, int _sizeOfData, D3D11_PRIMITIVE_TOPOLOGY topology_, UINT _stride);

private://static
	static bool getMeshInfoFromFile(LPCWSTR filename, int& v, int& vt, int& vn, int& f);
	static Mesh* getMeshFromFile(LPCWSTR filename, int v, int vt, int vn, int f);

private:
	VERTEX* vertices;
	void* rawData;
	int sizeOfRawData;
	ID3D11Buffer* vBuffer;
	int verticesCount;
	int trianglesCount;
	wchar_t* filename;
	D3D11_PRIMITIVE_TOPOLOGY topology;
	UINT stride;
};