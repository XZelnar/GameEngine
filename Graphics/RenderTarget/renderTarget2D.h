#pragma once

#include "../../stdafx.h"
#include "renderTarget.h"

using namespace std;

class RenderTarget2D : public RenderTarget
{
public:
	RenderTarget2D();
	RenderTarget2D(bool _keepBackBufferSize, float scale = 1);

	virtual void Dispose();
	virtual ID3D11ShaderResourceView** GetShaderResourceView();
	virtual void SetActiveRenderTarget();
	virtual void ClearRenderTarget(D3DXVECTOR4 color);
	virtual float GetWidth();
	virtual float GetHeight();

protected:
	virtual void releaseResources();
	virtual void createRenderTarget();

private:
	ID3D11Texture2D* texture;
	ID3D11RenderTargetView* view;
	ID3D11ShaderResourceView* shaderResourceView;
	float scale;
};