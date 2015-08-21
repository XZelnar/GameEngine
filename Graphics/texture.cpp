#include "texture.h"
#include "graphicsEngine.h"
#include "shaderManager.h"

Texture::Texture()
{
	texture = NULL;
}

Texture::Texture(ID3D11ShaderResourceView* _texture)
{
	texture = _texture;
}

void Texture::Dispose()
{
	if (texture)
		texture->Release();
	delete[] filename;
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return texture;
}

void Texture::ApplyTexture(int index)
{
	ShaderManager::SetTexture((ID3D11ShaderResourceView**)&texture, index);
}

//======================================================================================================================================
//======================================================================================================================================
//======================================================================================================================================

Texture* Texture::LoadFromFile(LPCWSTR filename)
{
	static ID3D11ShaderResourceView* t;
	static HRESULT r;
	r = D3DX11CreateShaderResourceViewFromFile(GraphicsEngine::GetDevice(), filename, NULL, NULL, &t, NULL);
	if (FAILED(r))
		return NULL;
	Texture* rr = new Texture(t);
	rr->filename = new wchar_t[wcslen(filename) + 1];
	memcpy(rr->filename, filename, (wcslen(filename) + 1) * 2);
	return rr;
}