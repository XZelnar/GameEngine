#pragma once

#include "../stdafx.h"

enum RENDER_PASS_TYPE
{
	RP_NONE,
	RP_REGULAR,
	RP_GUI
};

class GraphicsEngine
{
	friend class Window;
	friend int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
	friend LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	static void Initialize(HWND hWnd);
	static void Dispose();
	static inline ID3D11Device* GetDevice() { return d3ddev; }
	static inline ID3D11DeviceContext* GetContext() { return devcon; }
	static ID3D11RasterizerState* CreateRasterizer(const D3D11_RASTERIZER_DESC& rtr);
	static void SetRasterizer(ID3D11RasterizerState* rast);
	static void DisableDepth();
	static void EnableDepth();
	static void SetBackBufferRenderTarget();
	static void SetClearColor(D3DCOLOR color);
	static void Update();
	static RENDER_PASS_TYPE GetRenderPass();
	static void BeginFrame();
	static void BeginGUI();
	static void EndFrame();
	static int GetFPS() { return fps_; }

	static inline const D3D11_VIEWPORT& GetViewport() { return viewport; }

private:
	static void OnWM_SIZE();

private:
	static IDXGISwapChain* swapchain;
	static ID3D11Device* d3ddev;
	static ID3D11DeviceContext* devcon;
	static ID3D11Texture2D *pBackBuffer;
	static ID3D11Texture2D* g_d3dDepthStencilBuffer;
	static ID3D11DepthStencilView* g_d3dDepthStencilView;
	static ID3D11RenderTargetView* backbuffer;
	static ID3D11RasterizerState* rasterizer, *rasterizerCurrent;
	static ID3D11DepthStencilState * pDSState;
	static ID3D11DepthStencilState * pNoDSState;
	static ID3D11BlendState* blendNormal;
	static ID3D11BlendState* blendOff;

	static D3D11_VIEWPORT viewport;
	static DXGI_SWAP_CHAIN_DESC scd;

	static D3DXCOLOR clearColor;
	static RENDER_PASS_TYPE curRenderPass;
	static bool VSync;
	static int fps_;
	static bool depth;
	static bool skipFrame;
};