#pragma once

#include "../../stdafx.h"
#include "renderTarget.h"

using namespace std;

class RenderTargetsStack
{
	friend class GraphicsEngine;

public:
	static void PushRenderTarget(RenderTarget* t);
	static void PopRenderTarget(RenderTarget* t);
	static bool IsTop(RenderTarget* t);

private:
	static void Initialize(ID3D11RenderTargetView** target, ID3D11DepthStencilView** depth);

public:
	static D3D11_VIEWPORT viewport;
	
private:
	static stack<RenderTarget*> targetStack;
	static ID3D11RenderTargetView** baseTarget;
	static ID3D11DepthStencilView** baseDepth;
};