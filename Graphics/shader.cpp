#include "shader.h"

#include "graphicsEngine.h"
#include "shaderManager.h"
#include "../GameObject/Component/transformation.h"

ID3D11DeviceContext* Shader::devcon = null;
ID3D11Buffer* Shader::bPerFrame = NULL;
ID3D11Buffer* Shader::bPerCamera = NULL;
ID3D11Buffer* Shader::bPerObject = NULL;
ID3D11Buffer* Shader::bPerObjectColor = NULL;

Shader::Shader()
{
	if (!devcon)
		devcon = GraphicsEngine::GetContext();
	pLayout = null;
	cBuffersGeometryLength = 0;
	bGeometryCBuffers = null;
	useColor = false;
	cbuffers.clear();
}

void Shader::Dispose()
{
	for (auto i = cbuffers.begin(); i != cbuffers.end(); i++)
		i->second->Release();
	cbuffers.clear();
	delete[] bVertexCBuffers;
	delete[] bGeometryCBuffers;
	delete[] bPixelCBuffers;

	for (int i = 0; i < sTexturesCount; i++)
		sTextures[i]->Release();
	delete[] sTextures;

	if (pLayout)
		pLayout->Release();

	pVS->Release();
	pPS->Release();
	if (pGS)
		pGS->Release();

	blobVS->Release();
	blobPS->Release();
	if (blobGS)
		blobGS->Release();
}

void Shader::Activate() const
{
    devcon->VSSetShader(pVS, 0, 0);
    devcon->PSSetShader(pPS, 0, 0);
	devcon->GSSetShader(pGS, 0, 0);

	if (cBuffersVertexLength)
		devcon->VSSetConstantBuffers(0, cBuffersVertexLength, bVertexCBuffers);
	if (cBuffersGeometryLength)
		devcon->GSSetConstantBuffers(0, cBuffersGeometryLength, bGeometryCBuffers);
	if (cBuffersPixelLength)
		devcon->PSSetConstantBuffers(0, cBuffersPixelLength, bPixelCBuffers);

	devcon->PSSetSamplers(0, sTexturesCount, sTextures);
	devcon->IASetInputLayout(pLayout);
}

void Shader::Deactivate() const
{
}

void Shader::SetCBuffer(std::string name, void* value)
{
	assert(cbuffers[name] != NULL);
	devcon->UpdateSubresource(cbuffers[name], 0, nullptr, value, 0, 0);
}

void Shader::SetTexture(ID3D11ShaderResourceView** texture, int index)
{
	devcon->PSSetShaderResources(index, 1, texture);
}

void Shader::SetInputLayout(const D3D11_INPUT_ELEMENT_DESC* ied3d, int numElements)
{
	if (pLayout)
		pLayout->Release();
	HRESULT res = GraphicsEngine::GetDevice()->CreateInputLayout(ied3d, numElements, blobVS->GetBufferPointer(), blobVS->GetBufferSize(), &pLayout);
	assert(!FAILED(res));
}



void Shader::SetCBufferPerFrame(void* value)
{
	if (!bPerFrame)
		return;
	devcon->UpdateSubresource(bPerFrame, 0, nullptr, value, 0, 0);
}

void Shader::SetCBufferPerCamera(void* value)
{
	if (!bPerCamera)
		return;
	devcon->UpdateSubresource(bPerCamera, 0, nullptr, value, 0, 0);
}

void Shader::SetCBufferPerObject(void* value)
{
	if (!bPerObject)
		return;
	devcon->UpdateSubresource(bPerObject, 0, nullptr, value, 0, 0);
}

void Shader::SetCBufferPerObjectColor(void* value)
{
	if (!bPerObjectColor || !useColor)
		return;
	devcon->UpdateSubresource(bPerObjectColor, 0, nullptr, value, 0, 0);
}



Shader* Shader::CompileFromFile(LPCWSTR filename)
{
	static HRESULT res;
	static ID3D10Blob *VS, *PS, *GS, *err;
	static char buf[200];

	wcstombs(buf, filename, 200);
	if (FILE *file = fopen(buf, "r"))
		fclose(file);
	else
		throw new std::exception("File not found.");

	Shader::ShaderInclude si;

	res = D3DX11CompileFromFile(filename, 0, &si, "VShader", "vs_5_0", 0, 0, 0, &VS, &err, 0);
	if (FAILED(res))
	{
		char* strerr = (char*)err->GetBufferPointer();
		int asfd = 0;
		goto failed;
	}
	res = D3DX11CompileFromFile(filename, 0, &si, "PShader", "ps_5_0", 0, 0, 0, &PS, &err, 0);
	if (FAILED(res))
	{
		char* strerr = (char*)err->GetBufferPointer();
		int asfd = 0;
		goto failed;
	}
	bool geom = false;
	GS = null;
	res = D3DX11CompileFromFile(filename, 0, &si, "GShader", "gs_5_0", 0, 0, 0, &GS, &err, 0);
	if (FAILED(res))
	{
		geom = false;
		char* strerr = (char*)err->GetBufferPointer();
		int asfd = 0;
	}
	else
		geom = true;

	return generateShader(VS, PS, GS);

failed:
	return null;
}

Shader* Shader::CompileFromMemory(LPCSTR data)
{
	static HRESULT res;
	static ID3D10Blob *VS, *PS, *GS , *err;

	Shader::ShaderInclude si;

	res = D3DX11CompileFromMemory(data, strlen(data), 0, 0, &si, "VShader", "vs_5_0", 0, 0, 0, &VS, &err, 0);
	if (FAILED(res))
	{
		char* strerr = (char*)err->GetBufferPointer();
		int asfd = 0;
		goto failed;
	}
	res = D3DX11CompileFromMemory(data, strlen(data), 0, 0, &si, "PShader", "ps_5_0", 0, 0, 0, &PS, &err, 0);
	if (FAILED(res))
	{
		char* strerr = (char*)err->GetBufferPointer();
		int asfd = 0;
		goto failed;
	}
	bool geom = false;
	GS = null;
	res = D3DX11CompileFromMemory(data, strlen(data), 0, 0, &si, "GShader", "gs_5_0", 0, 0, 0, &GS, &err, 0);
	if (FAILED(res))
	{
		geom = false;
		char* strerr = (char*)err->GetBufferPointer();
		int asfd = 0;
	}
	else
		geom = true;

	return generateShader(VS, PS, GS);

failed:
	return null;
}

Shader* Shader::generateShader(ID3D10Blob* VS, ID3D10Blob* PS, ID3D10Blob* GS)
{
	static HRESULT res;
	Shader* s = new Shader();

	bool geom = GS;

	s->blobVS = VS;
	s->blobGS = GS;
	s->blobPS = PS;

	res = GraphicsEngine::GetDevice()->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &(s->pVS));
	res = GraphicsEngine::GetDevice()->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &(s->pPS));
	if (geom)
		GraphicsEngine::GetDevice()->CreateGeometryShader(GS->GetBufferPointer(), GS->GetBufferSize(), NULL, &(s->pGS));
	else
		s->pGS = NULL;

	static D3D11_INPUT_ELEMENT_DESC ied3d[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	res = GraphicsEngine::GetDevice()->CreateInputLayout(ied3d, _countof(ied3d), VS->GetBufferPointer(), VS->GetBufferSize(), &s->pLayout);

	D3D11_BUFFER_DESC cBufferDesc;
	cBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	cBufferDesc.ByteWidth = sizeof(D3DXMATRIX);
	cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags = 0;
	cBufferDesc.MiscFlags = 0;
	cBufferDesc.StructureByteStride = 0;

	//reflection
	ID3D11ShaderReflection* reflection = null;
	ID3D11ShaderReflectionConstantBuffer* cb;
	D3D11_SHADER_DESC desc;
	D3D11_SHADER_BUFFER_DESC cbdesc;
	ID3D11Buffer* buf;

	//vertex reflection
	res = D3DReflect(VS->GetBufferPointer(), VS->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&reflection);
	reflection->GetDesc(&desc);
	ID3D11Buffer** bufs = new ID3D11Buffer*[desc.ConstantBuffers];
	s->cBuffersVertexSizes = new int[desc.ConstantBuffers];
	for (int i = 0; i < desc.ConstantBuffers; i++)
	{
		cb = reflection->GetConstantBufferByIndex(i);
		cb->GetDesc(&cbdesc);
		buf = s->getCBuffer(cbdesc);
		bufs[i] = buf;
	}
	s->bVertexCBuffers = bufs;
	s->cBuffersVertexLength = desc.ConstantBuffers;

	//geometry reflection
	if (geom)
	{
		res = D3DReflect(GS->GetBufferPointer(), GS->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&reflection);
		reflection->GetDesc(&desc);
		bufs = new ID3D11Buffer*[desc.ConstantBuffers];
		s->cBuffersGeometrySizes = new int[desc.ConstantBuffers];
		for (int i = 0; i < desc.ConstantBuffers; i++)
		{
			cb = reflection->GetConstantBufferByIndex(i);
			cb->GetDesc(&cbdesc);
			buf = s->getCBuffer(cbdesc);
			bufs[i] = buf;
		}
		s->bGeometryCBuffers = bufs;
		s->cBuffersGeometryLength = desc.ConstantBuffers;
	}

	//pixel reflection
	res = D3DReflect(PS->GetBufferPointer(), PS->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&reflection);
	reflection->GetDesc(&desc);
	bufs = new ID3D11Buffer*[desc.ConstantBuffers];
	s->cBuffersPixelSizes = new int[desc.ConstantBuffers];
	for (int i = 0; i < desc.ConstantBuffers; i++)
	{
		cb = reflection->GetConstantBufferByIndex(i);
		cb->GetDesc(&cbdesc);
		buf = s->getCBuffer(cbdesc);
		bufs[i] = buf;
	}
	s->bPixelCBuffers = bufs;
	s->cBuffersPixelLength = desc.ConstantBuffers;



	static D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	D3D11_SHADER_INPUT_BIND_DESC sbd;
	s->sTexturesCount = 0;
	for (int i = 0; i < desc.BoundResources; i++)
	{
		reflection->GetResourceBindingDesc(i, &sbd);
		if (sbd.Type == D3D_SIT_TEXTURE)
			s->sTexturesCount++;
	}

	s->sTextures = new ID3D11SamplerState*[s->sTexturesCount];
	for (int i = 0; i < s->sTexturesCount; i++)
		res = GraphicsEngine::GetDevice()->CreateSamplerState(&samplerDesc, &s->sTextures[i]);

	return s;

failed:
	delete s;
	return NULL;
}

ID3D11Buffer* Shader::getCBuffer(D3D11_SHADER_BUFFER_DESC& cbdesc)
{
	D3D11_BUFFER_DESC cBufferDesc;
	cBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	cBufferDesc.ByteWidth = cbdesc.Size;
	cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags = 0;
	cBufferDesc.MiscFlags = 0;
	cBufferDesc.StructureByteStride = 0;

#define __processBuffer(a, b) if (!strcmp(cbdesc.Name, a)) { if (!b) assert(!GraphicsEngine::GetDevice()->CreateBuffer(&cBufferDesc, NULL, &b)); return b; }

	__processBuffer("_PerFrame", bPerFrame);
	__processBuffer("_PerCamera", bPerCamera);
	__processBuffer("_PerObject", bPerObject);
	//__processBuffer("_PerObjectColor", bPerObjectColor);
	if (!strcmp(cbdesc.Name, "_PerObjectColor"))
	{
		useColor = true;
		if (!bPerObjectColor)
		{
			assert(!GraphicsEngine::GetDevice()->CreateBuffer(&cBufferDesc, NULL, &bPerObjectColor));
			D3DXVECTOR4 t(1, 1, 1, 1);
			SetCBufferPerObjectColor(&t);
		}
		return bPerObjectColor;
	}

	//if (!strcmp(cbdesc.Name, "_PerFrame"))
	//{
	//	if (!bPerFrame)
	//		assert(!GraphicsEngine::GetDevice()->CreateBuffer(&cBufferDesc, NULL, &bPerFrame));
	//	return bPerFrame;
	//}

	if (cbuffers[cbdesc.Name] != null)
		return cbuffers[cbdesc.Name];

	ID3D11Buffer* buf;
	assert(!GraphicsEngine::GetDevice()->CreateBuffer(&cBufferDesc, NULL, &buf));
	cbuffers[std::string(cbdesc.Name)] = buf;
	return buf;
}











HRESULT Shader::ShaderInclude::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
{
	if (strcmp(pFileName, "variables.fx"))
	{
		throw new exception("???");
	}

	static char* varInclude = "cbuffer _PerFrame\n\
			{\n\
			// Time since current scene load\n\
			uniform float4 _Time; // (t/20, t, t*2, t*3)\n\
			uniform float4 _GameTime; // (t/20, t, t*2, t*3)\n\
			uniform float4 _SinTime; // sin(t/8), sin(t/4), sin(t/2), sin(t)\n\
			uniform float4 _CosTime; // cos(t/8), cos(t/4), cos(t/2), cos(t)\n\
			uniform float4 _SinGameTime; // sin(t/8), sin(t/4), sin(t/2), sin(t)\n\
			uniform float4 _CosGameTime; // cos(t/8), cos(t/4), cos(t/2), cos(t)\n\
			uniform float4 _DeltaTime; // dt, 1/dt, gamedt, 1/gamedt\n\
			}\n\
			\n\
			cbuffer _PerCamera\n\
			{\n\
			matrix _MatrixV;\n\
			matrix _MatrixP;\n\
			matrix _MatrixVP;\n\
			float3 _WorldSpaceCameraPos;\n\
			float __unused;\n\
			}\n\
			\n\
			cbuffer _PerObject\n\
			{\n\
			matrix _MatrixMVP;\n\
			matrix _Object2World;\n\
			matrix _World2Object;\n\
			}\n\
			\n\
			cbuffer _PerObjectColor\n\
			{\n\
			float4 _Color;\n\
			}";

	*ppData = varInclude;
	*pBytes = strlen(varInclude);
	return 0;
}

HRESULT Shader::ShaderInclude::Close(LPCVOID pData)
{
	return 0;
}