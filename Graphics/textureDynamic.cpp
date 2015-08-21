#include "textureDynamic.h"

#include "graphicsEngine.h"

TextureDynamic::TextureDynamic(int width, int height)
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = w = width;
	desc.Height = h = height;
	desc.MipLevels = desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	auto res = GraphicsEngine::GetDevice()->CreateTexture2D(&desc, NULL, &pTexture);
	assert(!FAILED(res));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	res = GraphicsEngine::GetDevice()->CreateShaderResourceView(pTexture, &srvDesc, &texture);
	assert(!FAILED(res));

	devcon = GraphicsEngine::GetContext();
}

void TextureDynamic::Dispose()
{
	texture->Release();
	pTexture->Release();
}

void TextureDynamic::SetPixels(void* data)
{
	D3D11_MAPPED_SUBRESOURCE s;
	devcon->Map(pTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &s);
	memcpy(s.pData, data, w * h * 4);
	devcon->Unmap(pTexture, 0);
}