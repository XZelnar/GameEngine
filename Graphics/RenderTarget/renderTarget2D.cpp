#include "renderTarget2D.h"

#include "../graphicsEngine.h"
#include "../../window.h"
#include "renderTargetsStack.h"

RenderTarget2D::RenderTarget2D()
{
	keepBackBufferSize = false;
	texture = null;
	view = null;
	shaderResourceView = null;

	fbos.push_back(this);
	createRenderTarget();
}

RenderTarget2D::RenderTarget2D(bool _keepBackBufferSize, float scale)
{
	keepBackBufferSize = _keepBackBufferSize;
	texture = null;
	view = null;
	shaderResourceView = null;

	this->scale = scale;
	
	fbos.push_back(this);
	createRenderTarget();
}

void RenderTarget2D::Dispose()
{
	releaseResources();

	fbos.erase(remove(fbos.begin(), fbos.end(), this), fbos.end());
}

ID3D11ShaderResourceView** RenderTarget2D::GetShaderResourceView()
{
	return &shaderResourceView;
}

void RenderTarget2D::SetActiveRenderTarget()
{
	RenderTarget::SetActiveRenderTarget();
	GraphicsEngine::GetContext()->OMSetRenderTargets(1, &view, null);
	RenderTargetsStack::viewport.Width =  GetWidth();
	RenderTargetsStack::viewport.Height = GetHeight();
	GraphicsEngine::GetContext()->RSSetViewports(1, &RenderTargetsStack::viewport);
}


void RenderTarget2D::ClearRenderTarget(D3DXVECTOR4 color)
{
	if (!RenderTargetsStack::IsTop(this))
		return;
	GraphicsEngine::GetContext()->ClearRenderTargetView(view, color);
}

float RenderTarget2D::GetWidth()
{
	return Window::GetWidth() * scale;
}

float RenderTarget2D::GetHeight()
{
	return Window::GetHeight() * scale;
}



void RenderTarget2D::releaseResources()
{
	view->Release();
	shaderResourceView->Release();
	texture->Release();

	view = 0;
	shaderResourceView = 0;
	texture = 0;
}

void RenderTarget2D::createRenderTarget()
{
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	ZeroMemory(&renderTargetViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	textureDesc.Width = Window::GetWidth() * scale;
	textureDesc.Height = Window::GetHeight() * scale;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	result = GraphicsEngine::GetDevice()->CreateTexture2D(&textureDesc, NULL, &texture);

	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;
	result = GraphicsEngine::GetDevice()->CreateRenderTargetView(texture, &renderTargetViewDesc, &view);

	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	result = GraphicsEngine::GetDevice()->CreateShaderResourceView(texture, &shaderResourceViewDesc, &shaderResourceView);
}