#pragma once

#include "../stdafx.h"

class Texture;
class Transformation;

class Shader
{
	friend class Material;
	friend class ShaderManager;

public:
	Shader();
	void Dispose();
	void Activate() const;
	void Deactivate() const;

private:
	void SetCBuffer(std::string name, void* value);
	void SetTexture(ID3D11ShaderResourceView** texture, int index = 0);
	void SetInputLayout(const D3D11_INPUT_ELEMENT_DESC* ied3d, int numElements);
	ID3D11Buffer* getCBuffer(D3D11_SHADER_BUFFER_DESC& cbdesc);

protected:
	ID3D10Blob* blobVS, *blobGS, *blobPS;
	ID3D11VertexShader* pVS;
	ID3D11PixelShader* pPS;
	ID3D11GeometryShader* pGS;
	ID3D11InputLayout* pLayout;
	ID3D11Buffer** bVertexCBuffers;
	ID3D11Buffer** bGeometryCBuffers;
	ID3D11Buffer** bPixelCBuffers;
	int* cBuffersVertexSizes;
	int* cBuffersGeometrySizes;
	int* cBuffersPixelSizes;
	int  cBuffersVertexLength;
	int  cBuffersGeometryLength;
	int  cBuffersPixelLength;
	ID3D11SamplerState** sTextures;
	int sTexturesCount;

	map<std::string, ID3D11Buffer*> cbuffers;
	bool useColor;

private:
	static ID3D11DeviceContext* devcon;
	static ID3D11Buffer* bPerFrame;
	static ID3D11Buffer* bPerCamera;
	static ID3D11Buffer* bPerObject;
	static ID3D11Buffer* bPerObjectColor;

public://static
	static Shader* CompileFromMemory(LPCSTR filename);
	static Shader* CompileFromFile(LPCWSTR filename);

private://static
	static Shader* generateShader(ID3D10Blob* VS, ID3D10Blob* PS, ID3D10Blob* GS);

	static void SetCBufferPerFrame(void* value);
	static void SetCBufferPerCamera(void* value);
	static void SetCBufferPerObject(void* value);
	void SetCBufferPerObjectColor(void* value);

protected:
	class ShaderInclude : public ID3D10Include
	{
	public:
		STDMETHOD(Open)(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes);
		STDMETHOD(Close)(LPCVOID pData);
	};
};