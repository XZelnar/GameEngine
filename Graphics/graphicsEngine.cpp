#include "graphicsEngine.h"

#include "../window.h"
#include "Camera/camera.h"
#include "shaderManager.h"
#include "Font/fontManager.h"
#include "graphicsResourceManager.h"
#include "Camera/cameraManager.h"
#include "renderHelper.h"
#include "Font/textPrinter.h"
#include "materialsManager.h"

IDXGISwapChain* GraphicsEngine::swapchain;
DXGI_SWAP_CHAIN_DESC GraphicsEngine::scd;
ID3D11Device* GraphicsEngine::d3ddev;
ID3D11DeviceContext* GraphicsEngine::devcon;
ID3D11Texture2D* GraphicsEngine::pBackBuffer;
ID3D11Texture2D* GraphicsEngine::g_d3dDepthStencilBuffer;
ID3D11DepthStencilView* GraphicsEngine::g_d3dDepthStencilView;
ID3D11RenderTargetView* GraphicsEngine::backbuffer;
D3D11_VIEWPORT GraphicsEngine::viewport;
ID3D11RasterizerState* GraphicsEngine::rasterizer, *GraphicsEngine::rasterizerCurrent;
ID3D11DepthStencilState* GraphicsEngine::pDSState;
ID3D11DepthStencilState* GraphicsEngine::pNoDSState;
ID3D11BlendState* GraphicsEngine::blendNormal;
ID3D11BlendState* GraphicsEngine::blendOff;

D3DXCOLOR GraphicsEngine::clearColor = D3DXCOLOR(0, 0, 0, 1);
RENDER_PASS_TYPE GraphicsEngine::curRenderPass = RENDER_PASS_TYPE::RP_NONE;
bool GraphicsEngine::VSync = true;
int GraphicsEngine::fps_ = 0;
bool GraphicsEngine::depth = true;
bool GraphicsEngine::skipFrame = false;

void GraphicsEngine::Initialize(HWND hWnd)
{
	HRESULT res;

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferCount = 2;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hWnd;
	scd.SampleDesc.Count = 1;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scd.Windowed = !Window::IsFullscreen();
	scd.BufferDesc.Width = Window::GetWidth();
	scd.BufferDesc.Height = Window::GetHeight();

	D3D_FEATURE_LEVEL fl = D3D_FEATURE_LEVEL_11_0;

#if DEBUG
	res = D3D11CreateDeviceAndSwapChain(null, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, &fl, 1, D3D11_SDK_VERSION, &scd, &swapchain, &d3ddev, NULL, &devcon);
#else
	res = D3D11CreateDeviceAndSwapChain(null, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &fl, 1, D3D11_SDK_VERSION, &scd, &swapchain, &d3ddev, NULL, &devcon);
#endif

	res = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	res = d3ddev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	IDXGIFactory* f;
	res = swapchain->GetParent(__uuidof(IDXGIFactory), (void**)&f);
	res = f->MakeWindowAssociation(Window::GetHWND(), DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);

	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	ZeroMemory(&depthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
	depthStencilBufferDesc.ArraySize = 1;
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.CPUAccessFlags = 0;
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBufferDesc.Width = Window::GetWidth();
	depthStencilBufferDesc.Height = Window::GetHeight();
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.SampleDesc.Count = 1;
	depthStencilBufferDesc.SampleDesc.Quality = 0;
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	res = d3ddev->CreateTexture2D(&depthStencilBufferDesc, nullptr, &g_d3dDepthStencilBuffer);
	res = d3ddev->CreateDepthStencilView(g_d3dDepthStencilBuffer, NULL, &g_d3dDepthStencilView);

	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dsDesc.StencilEnable = false;
	res = d3ddev->CreateDepthStencilState(&dsDesc, &pDSState);
	//dsDesc.DepthEnable = false;
	//dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	res = d3ddev->CreateDepthStencilState(&dsDesc, &pNoDSState);

	devcon->OMSetRenderTargets(1, &backbuffer, g_d3dDepthStencilView);
	devcon->OMSetDepthStencilState(pDSState, 1);

	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.Width = Window::GetWidth();
	viewport.Height = Window::GetHeight();
	devcon->RSSetViewports(1, &viewport);

	D3D11_RASTERIZER_DESC rtr;
	ZeroMemory(&rtr, sizeof(D3D11_RASTERIZER_DESC));
	rtr.FillMode = D3D11_FILL_SOLID;
	//rtr.CullMode = D3D11_CULL_FRONT;
	//rtr.FillMode = D3D11_FILL_WIREFRAME;
	rtr.CullMode = D3D11_CULL_NONE;
	res = d3ddev->CreateRasterizerState(&rtr, &rasterizer);
	devcon->RSSetState(rasterizer);

	D3D11_BLEND_DESC blendStateDescription;
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	d3ddev->CreateBlendState(&blendStateDescription, &blendNormal);
	blendStateDescription.RenderTarget[0].BlendEnable = false;
	d3ddev->CreateBlendState(&blendStateDescription, &blendOff);
	devcon->OMSetBlendState(blendNormal, NULL, 0xffffffff);

	RenderHelper::StaticInit();
	ShaderManager::Initialize();
	FontManager::GetInstance().Initialize();
	TextPrinter::StaticInit();
	MaterialsManager::Initialize();
}

void GraphicsEngine::Dispose()
{
	FontManager::GetInstance().Dispose();
	ShaderManager::Dispose();
	RenderHelper::StaticDispose();
	TextPrinter::StaticDispose();
	MaterialsManager::Dispose();

	blendNormal->Release();
	blendOff->Release();
	rasterizer->Release();
	backbuffer->Release();
	pBackBuffer->Release();
	g_d3dDepthStencilView->Release();
	g_d3dDepthStencilBuffer->Release();
	pDSState->Release();
	pNoDSState->Release();
	swapchain->SetFullscreenState(FALSE, NULL);
	swapchain->Release();
	d3ddev->Release();
	devcon->Release();
}
ID3D11RasterizerState* GraphicsEngine::CreateRasterizer(const D3D11_RASTERIZER_DESC& rtr)
{
	ID3D11RasterizerState* r;
	HRESULT res = d3ddev->CreateRasterizerState(&rtr, &r);
	assert(!FAILED(res));
	return r;
}

void GraphicsEngine::SetRasterizer(ID3D11RasterizerState* rast)
{
	if (rast == null)
		rast = rasterizer;
	if (rasterizerCurrent == rast)
		return;
	devcon->RSSetState(rast);
	rasterizerCurrent = rast;
}

void GraphicsEngine::DisableDepth()
{
	if (!depth)
		return;
	devcon->OMSetDepthStencilState(pNoDSState, 1);
	depth = false;
}

void GraphicsEngine::EnableDepth()
{
	if (depth)
		return;
	devcon->OMSetDepthStencilState(pDSState, 1);
	depth = true;
}

void GraphicsEngine::SetBackBufferRenderTarget()
{
	devcon->OMSetRenderTargets(1, &backbuffer, g_d3dDepthStencilView);
	devcon->OMSetDepthStencilState(pDSState, 1);
	devcon->RSSetViewports(1, &GraphicsEngine::viewport);
	devcon->OMSetBlendState(blendNormal, NULL, 0xffffffff);
}

void GraphicsEngine::SetClearColor(D3DCOLOR color)
{
	clearColor = color;
}

void GraphicsEngine::OnWM_SIZE()
{
	if (!swapchain)
		return;
	devcon->OMSetRenderTargets(0, 0, 0);
	backbuffer->Release();
	backbuffer = null;
	pBackBuffer->Release();
	bool oldw = scd.Windowed;
	scd.BufferDesc.Width = Window::GetWidth();
	scd.BufferDesc.Height = Window::GetHeight();
	scd.Windowed = !Window::IsFullscreen();

	if (oldw != scd.Windowed)
	{
		if (scd.Windowed)
		{
			swapchain->SetFullscreenState(false, null);
			swapchain->ResizeTarget(&scd.BufferDesc);
		}
		else
		{
			swapchain->SetFullscreenState(true, null);
			swapchain->ResizeTarget(&scd.BufferDesc);
		}
	}
	else
		swapchain->ResizeTarget(&scd.BufferDesc);

	swapchain->ResizeBuffers(0, Window::GetWidth(), Window::GetHeight(), DXGI_FORMAT_UNKNOWN, 0);

	//DXGI_SWAP_CHAIN_DESC d2;
	//swapchain->GetDesc(&d2);

	if (backbuffer)
		backbuffer->Release();
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	d3ddev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);

	g_d3dDepthStencilBuffer->Release();
	g_d3dDepthStencilView->Release();
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	ZeroMemory(&depthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
	depthStencilBufferDesc.ArraySize = 1;
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.CPUAccessFlags = 0;
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBufferDesc.Width = Window::GetWidth();
	depthStencilBufferDesc.Height = Window::GetHeight();
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.SampleDesc.Count = 1;
	depthStencilBufferDesc.SampleDesc.Quality = 0;
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3ddev->CreateTexture2D(&depthStencilBufferDesc, nullptr, &g_d3dDepthStencilBuffer);
	d3ddev->CreateDepthStencilView(g_d3dDepthStencilBuffer, NULL, &g_d3dDepthStencilView);

	pDSState->Release();
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dsDesc.StencilEnable = false;
	d3ddev->CreateDepthStencilState(&dsDesc, &pDSState);
	dsDesc.DepthEnable = false;
	d3ddev->CreateDepthStencilState(&dsDesc, &pNoDSState);

	devcon->OMSetRenderTargets(1, &backbuffer, g_d3dDepthStencilView);
	devcon->OMSetDepthStencilState(pDSState, 1);

	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.Width = Window::GetWidth();
	viewport.Height = Window::GetHeight();
	devcon->RSSetViewports(1, &viewport);

	//UINT t = 1;
	//D3D11_VIEWPORT v2;
	//devcon->RSGetViewports(&t, &v2);
	//DXGI_SWAP_CHAIN_DESC d;
	//swapchain->GetDesc(&d);

	CameraManager::OnResolutionChanged();
}

void GraphicsEngine::Update()
{
	CameraManager::Update();
}

RENDER_PASS_TYPE GraphicsEngine::GetRenderPass()
{
	return curRenderPass;
}

void GraphicsEngine::BeginFrame()
{
	devcon->ClearRenderTargetView(backbuffer, clearColor);
	devcon->ClearDepthStencilView(g_d3dDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	if (rasterizerCurrent != rasterizer)
		SetRasterizer(rasterizer);
	curRenderPass = RENDER_PASS_TYPE::RP_REGULAR;
	CameraManager::UpdateShader();
}

void GraphicsEngine::BeginGUI()
{
	if (rasterizerCurrent != rasterizer)
		SetRasterizer(rasterizer);
	EnableDepth();
	devcon->OMSetDepthStencilState(pNoDSState, 1);
	curRenderPass = RENDER_PASS_TYPE::RP_GUI;
	CameraManager::UpdateShader();
}

void GraphicsEngine::EndFrame()
{
	static int _buf = 0, _sec = 0;
	static char buf[10] = "";
	_buf++;
	time_t t;
	time(&t);
	if (t != _sec)
	{
		_sec = t;
		fps_ = _buf;
		_buf = 0;
	}

	devcon->OMSetDepthStencilState(pDSState, 1);

	ShaderManager::FrameEnd();
	MaterialsManager::setActive(null);
	curRenderPass = RENDER_PASS_TYPE::RP_NONE;
	swapchain->Present(VSync, 0);
	//swapchain->Present(0, 0);
}