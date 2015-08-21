#pragma once

#include "../../stdafx.h"

using namespace std;

class RenderTarget
{
	friend class GraphicsEngine;

public:
	virtual void Dispose() = 0;
	virtual ID3D11ShaderResourceView** GetShaderResourceView() = 0;
	virtual void SetActiveRenderTarget();
	virtual void DisableActiveRenderTarget();
	virtual void ClearRenderTarget(D3DXVECTOR4 color) = 0;
	virtual float GetWidth() = 0;//TODO rm
	virtual float GetHeight() = 0;

protected:
	virtual void releaseResources() = 0;
	virtual void createRenderTarget() = 0;

protected:
	bool keepBackBufferSize;

private://static
	static void OnResolutionChanged(int w, int h);

protected://static
	static vector<RenderTarget*> fbos;
};