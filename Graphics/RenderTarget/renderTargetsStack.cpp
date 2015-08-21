#include "renderTargetsStack.h"

#include "../graphicsEngine.h"
#include "../shaderManager.h"
#include "../../window.h"

D3D11_VIEWPORT RenderTargetsStack::viewport;

stack<RenderTarget*> RenderTargetsStack::targetStack;
ID3D11RenderTargetView** RenderTargetsStack::baseTarget;
ID3D11DepthStencilView** RenderTargetsStack::baseDepth;

void RenderTargetsStack::PushRenderTarget(RenderTarget* t)
{
	targetStack.push(t);
}

void RenderTargetsStack::PopRenderTarget(RenderTarget* t)
{
	if (t != targetStack.top())
		return;
	targetStack.pop();
	if (targetStack.empty())
	{
		//GraphicsEngine::GetContext()->OMSetRenderTargets(1, baseTarget, *baseDepth);
		GraphicsEngine::SetBackBufferRenderTarget();
	}
	else
		targetStack.top()->SetActiveRenderTarget();

	//ShaderManager::ReActivateShader();//TODO rm
}

bool RenderTargetsStack::IsTop(RenderTarget* t)
{
	return targetStack.top() == t;
}



void RenderTargetsStack::Initialize(ID3D11RenderTargetView** target, ID3D11DepthStencilView** depth)
{
	baseTarget = target;
	baseDepth = depth;

    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
    viewport.Width = Window::GetWidth();
	viewport.Height = Window::GetHeight();
}