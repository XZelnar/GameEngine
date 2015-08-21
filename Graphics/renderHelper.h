#pragma once

#include "../stdafx.h"

class Texture;

class RenderHelper
{
	friend class GraphicsEngine;

public:
	static void RenderSquare(const float& x1, const float& y1, const float& x2, const float& y2, Texture* texture);
	static void RenderSquare(const float& x1, const float& y1, const float& x2, const float& y2, Texture* texture, D3DXMATRIX* postTransform);
	static void RenderSquare(const float& x1, const float& y1, const float& x2, const float& y2, Texture* texture, D3DXMATRIX* preTransform, D3DXMATRIX* postTransform);

private:
	static void StaticInit();
	static void StaticDispose();

private:
	static ID3D11DeviceContext* devcon;

	static D3DXMATRIX mTransform;
	static ID3D11Buffer* vBufferGuiSquare;
	static ID3D11RasterizerState* rast;

private:
	struct VERTEX_GUI
	{
		D3DXVECTOR2 POSITION;
		float U, V;
	};
};