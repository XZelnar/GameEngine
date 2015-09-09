#include "mesh.h"
#include "graphicsEngine.h"

using namespace std;

Mesh::Mesh()
{
	vBuffer = NULL;
	verticesCount = 0;
	trianglesCount = 0;
	rawData = null;
	sizeOfRawData = 0;
	filename = L"";
	topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	stride = sizeof(VERTEX);
}

void Mesh::Dispose()
{
	if (vBuffer)
		vBuffer->Release();
	if (vertices != null)
		delete[] vertices;
}

void Mesh::Render()
{
	static UINT offset = 0;
	GraphicsEngine::GetContext()->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
	GraphicsEngine::GetContext()->IASetPrimitiveTopology(topology);
	GraphicsEngine::GetContext()->Draw(verticesCount, 0);
}

void Mesh::SetVertices(D3DXVECTOR3* points, D3DXVECTOR3* normals, D3DXVECTOR4* colors, D3DXVECTOR2* texcoords, int length, bool _updateVBO)
{
	if (length != verticesCount)
	{
		delete[] vertices;
		vertices = null;
		if (vBuffer)
		{
			vBuffer->Release();
			vBuffer = null;
		}
	}
	if (vertices == null)
	{
		vertices = new VERTEX[length];
		rawData = vertices;
		sizeOfRawData = length * sizeof(VERTEX);
	}


	trianglesCount = length / 3;
	verticesCount = length;

	for (int i = 0; i < length; i++)
	{
		vertices[i].POSITION = points[i];
		vertices[i].TEXCOORD = texcoords[i];
		vertices[i].NORMAL = normals[i];
	}

	if (_updateVBO)
		updateVBO();
}

void Mesh::SetRawData(void* _data, int _vertexCount, int _sizeOfData, UINT _stride)
{
	if (verticesCount != _vertexCount)
	{
		if (vBuffer)
		{
			vBuffer->Release();
			vBuffer = null;
		}
		verticesCount = _vertexCount;
		if (topology == D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
			trianglesCount = _vertexCount / 3;
		else if (topology == D3D11_PRIMITIVE_TOPOLOGY_LINELIST)
			trianglesCount = _vertexCount / 2;
		else
			trianglesCount = _vertexCount;
	}
	stride = _stride;
	rawData = _data;
	sizeOfRawData = _sizeOfData;
	updateVBO();
}

Mesh::VERTEX Mesh::GetVertex(int index)
{
	if (index < 0) index = 0;
	if (index >= GetVertexCount()) index = GetVertexCount() - 1;
	if (vertices == null) throw exception("Attempting to get vertices from Mesh, that was created from raw data");
	return vertices[index];
}

#if DEBUG
void Mesh::SetName(char* name)
{
	if (vBuffer == null)
		return;
	vBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(name) - 1, name);
}
#endif

//======================================================================================================================================
//======================================================================================================================================
//======================================================================================================================================

Mesh* Mesh::LoadFromFile(LPCWSTR filename)
{
	int v, vt, vn, f;
	getMeshInfoFromFile(filename, v, vt, vn, f);
	if (v == 0)
		return NULL;
	Mesh* m = getMeshFromFile(filename, v, vt, vn, f);
	m->rawData = m->vertices;
	m->sizeOfRawData = m->verticesCount * sizeof(VERTEX);
	m->createVBO();
	m->filename = new wchar_t[wcslen(filename) + 1];
	memcpy(m->filename, filename, (wcslen(filename) + 1) * 2);
	return m;
}

Mesh* Mesh::CreateFromVertices(D3DXVECTOR3* vertices, D3DXVECTOR3* normals, D3DXVECTOR2* texcoords, int l, D3D11_PRIMITIVE_TOPOLOGY topology_, bool _createVBO)
{
	Mesh* m = new Mesh();
	m->topology = topology_;
	if (topology_ == D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
		m->trianglesCount = l / 3;
	else if (topology_ == D3D11_PRIMITIVE_TOPOLOGY_LINELIST)
		m->trianglesCount = l / 2;
	else
		m->trianglesCount = l;
	m->verticesCount = l;

	VERTEX* v = new VERTEX[l];
	for (int i = 0; i < l; i++)
	{
		v[i].POSITION = vertices[i];
		v[i].TEXCOORD = texcoords[i];
		v[i].NORMAL = normals[i];
	}

	m->vertices = v;
	m->rawData = v;
	m->sizeOfRawData = l * sizeof(VERTEX);
	if (_createVBO)
		m->createVBO();
	return m;
}

Mesh* Mesh::CreateFromRawData(void* _data, int vertexCount, int _sizeOfData, D3D11_PRIMITIVE_TOPOLOGY topology_, UINT _stride)
{
	Mesh* m = new Mesh();
	m->topology = topology_;
	if (topology_ == D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
		m->trianglesCount = vertexCount / 3;
	else if (topology_ == D3D11_PRIMITIVE_TOPOLOGY_LINELIST)
		m->trianglesCount = vertexCount / 2;
	else
		m->trianglesCount = vertexCount;
	m->verticesCount = vertexCount;
	m->stride = _stride;

	m->rawData = _data;
	m->sizeOfRawData = _sizeOfData;
	m->createVBO();
	return m;
}

//======================================================================================================================================
//======================================================================================================================================
//======================================================================================================================================

bool Mesh::getMeshInfoFromFile(LPCWSTR filename, int& v, int& vt, int& vn, int& f)
{
	v = 0;
	vt = 0;
	vn = 0;
	f = 0;

	ifstream ifs;
	ifs.open(filename);
	if (ifs.fail())
		return false;

	char c;

	while (!ifs.eof())
	{
		ifs.get(c);
		if (c == 'v')
		{
			ifs.get(c);
			if (c == ' ')
				v++;
			else if (c == 't')
				vt++;
			else if (c == 'n')
				vn++;
		}
		else if (c == 'f')
			f++;

		while (c != '\n')
			ifs.get(c);
	}

	ifs.close();

	return true;
}

Mesh* Mesh::getMeshFromFile(LPCWSTR filename, const int v, const int vt, const int vn, const int f)
{
	Mesh* result = new Mesh();
	result->vertices = new VERTEX[f * 3];

	D3DVECTOR* av;
	D3DXVECTOR2* avt;
	D3DVECTOR* avn;

	av = new D3DVECTOR[v];
	avt = new D3DXVECTOR2[vt];
	avn = new D3DVECTOR[vn];

	int curv = 0, curvt = 0, curvn = 0, curf = 0;
	int i1, i2, i3;

	char c;

	ifstream ifs;
	ifs.open(filename);
	if (ifs.fail())
		return result;

	while (!ifs.eof())
	{
		ifs.get(c);

		if (c == 'v')
		{
			ifs.get(c);
			if (c == ' ')
			{
				ifs >> av[curv].x >> av[curv].y >> av[curv].z;
				curv++;
			}
			else if (c == 'n')
			{
				ifs >> avn[curvn].x >> avn[curvn].y >> avn[curvn].z;
				curvn++;
			}
			else if (c == 't')
			{
				ifs >> avt[curvt].x >> avt[curvt].y;
				curvt++;
			}
		}
		else if (c == 'f')
			for (int i = 0; i < 3; i++)
			{
				ifs >> i1 >> c >> i2 >> c >> i3;
				result->vertices[curf].POSITION = av[i1 - 1];
				result->vertices[curf].TEXCOORD = avt[i2 - 1];
				result->vertices[curf++].NORMAL = avn[i3 - 1];
			}

		while (c != '\n')
			ifs.get(c);
	}

	delete[] av;
	delete[] avt;
	delete[] avn;

	result->verticesCount = f * 3;
	result->trianglesCount = f;
	return result;
}

//======================================================================================================================================
//======================================================================================================================================
//======================================================================================================================================

void Mesh::createVBO()
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeOfRawData;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GraphicsEngine::GetDevice()->CreateBuffer(&bd, NULL, &vBuffer);

	D3D11_MAPPED_SUBRESOURCE ms;
	GraphicsEngine::GetContext()->Map(vBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, rawData, sizeOfRawData);
	GraphicsEngine::GetContext()->Unmap(vBuffer, NULL);
}

void Mesh::updateVBO()
{
	if (rawData == null)
		return;
	if (vBuffer == null)
	{
		createVBO();
		return;
	}

	D3D11_MAPPED_SUBRESOURCE ms;
	HRESULT res = GraphicsEngine::GetContext()->Map(vBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, rawData, sizeOfRawData);
	GraphicsEngine::GetContext()->Unmap(vBuffer, NULL);
}