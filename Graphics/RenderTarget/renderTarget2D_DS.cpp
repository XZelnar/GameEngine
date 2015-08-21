#include "renderTarget2D_DS.h"

#include "../graphicsEngine.h"
#include "../../window.h"
#include "renderTargetsStack.h"

RenderTarget2D_DS::RenderTarget2D_DS()
{
	keepBackBufferSize = false;
	texture = null;
	view = null;
	shaderResourceView = null;

	fbos.push_back(this);
	createRenderTarget();
}

RenderTarget2D_DS::RenderTarget2D_DS(bool _keepBackBufferSize, float scale)
{
	keepBackBufferSize = _keepBackBufferSize;
	texture = null;
	view = null;
	shaderResourceView = null;

	this->scale = scale;
	
	fbos.push_back(this);
	createRenderTarget();
}

void RenderTarget2D_DS::Dispose()
{
	releaseResources();

	fbos.erase(remove(fbos.begin(), fbos.end(), this), fbos.end());
}

ID3D11ShaderResourceView** RenderTarget2D_DS::GetShaderResourceView()
{
	return &shaderResourceView;
}

void RenderTarget2D_DS::SetActiveRenderTarget()
{
	RenderTarget::SetActiveRenderTarget();
	GraphicsEngine::GetContext()->OMSetRenderTargets(1, &view, dsView);
	GraphicsEngine::GetContext()->OMSetDepthStencilState(pDSState, 1);
	RenderTargetsStack::viewport.Width =  GetWidth();
	RenderTargetsStack::viewport.Height = GetHeight();
	GraphicsEngine::GetContext()->RSSetViewports(1, &RenderTargetsStack::viewport);
}


void RenderTarget2D_DS::ClearRenderTarget(D3DXVECTOR4 color)
{
	if (!RenderTargetsStack::IsTop(this))
		return;
	GraphicsEngine::GetContext()->ClearRenderTargetView(view, color);
	GraphicsEngine::GetContext()->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

float RenderTarget2D_DS::GetWidth()
{
	return Window::GetWidth() * scale;
}

float RenderTarget2D_DS::GetHeight()
{
	return Window::GetHeight() * scale;
}



void RenderTarget2D_DS::releaseResources()
{
	view->Release();
	dsView->Release();
	shaderResourceView->Release();
	texture->Release();
	dsTexture->Release();

	view = 0;
	dsView = 0;
	shaderResourceView = 0;
	texture = 0;
	dsTexture = 0;
}

void RenderTarget2D_DS::createRenderTarget()
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


	//DS

	

	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	ZeroMemory( &depthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC) );
	depthStencilBufferDesc.ArraySize = 1;
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.CPUAccessFlags = 0;
	depthStencilBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilBufferDesc.Width = Window::GetWidth();
	depthStencilBufferDesc.Height = Window::GetHeight();
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.SampleDesc.Count = 1;
	depthStencilBufferDesc.SampleDesc.Quality = 0;
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	result = GraphicsEngine::GetDevice()->CreateTexture2D(&depthStencilBufferDesc, nullptr, &dsTexture);
	result = GraphicsEngine::GetDevice()->CreateDepthStencilView(dsTexture, NULL, &dsView);

	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsDesc.StencilEnable = false;
	result = GraphicsEngine::GetDevice()->CreateDepthStencilState(&dsDesc, &pDSState);
}