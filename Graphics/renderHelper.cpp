#include "renderHelper.h"

#include "graphicsEngine.h"
#include "texture.h"
#include "shader.h"
#include "shaderManager.h"

ID3D11DeviceContext* RenderHelper::devcon;

D3DXMATRIX RenderHelper::mTransform;
ID3D11Buffer* RenderHelper::vBufferGuiSquare;
ID3D11RasterizerState* RenderHelper::rast;

void RenderHelper::StaticInit()
{
	devcon = GraphicsEngine::GetContext();

	//===========================================================================guiSquare
	VERTEX_GUI vertices[] = {
			{ D3DXVECTOR2(0, 0), 0, 1 },
			{ D3DXVECTOR2(1, 0), 1, 1 },
			{ D3DXVECTOR2(0, 1), 0, 0 },
			{ D3DXVECTOR2(1, 1), 1, 0 }
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_GUI) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GraphicsEngine::GetDevice()->CreateBuffer(&bd, NULL, &vBufferGuiSquare);

	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(vBufferGuiSquare, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, vertices, 4 * sizeof(VERTEX_GUI));
	devcon->Unmap(vBufferGuiSquare, NULL);

	D3D11_RASTERIZER_DESC rtr;
	ZeroMemory(&rtr, sizeof(D3D11_RASTERIZER_DESC));
	rtr.FillMode = D3D11_FILL_SOLID;
	rtr.CullMode = D3D11_CULL_NONE;
	rast = GraphicsEngine::CreateRasterizer(rtr);
}

void RenderHelper::StaticDispose()
{
	vBufferGuiSquare->Release();
	rast->Release();
}



void RenderHelper::RenderSquare(const float& x1, const float& y1, const float& x2, const float& y2, Texture* texture)
{
	static D3DXMATRIX m = *D3DXMatrixIdentity(&m);
	RenderSquare(x1, y1, x2, y2, texture, &m, &m);
}

void RenderHelper::RenderSquare(const float& x1, const float& y1, const float& x2, const float& y2, Texture* texture, D3DXMATRIX* postTransform)
{
	static D3DXMATRIX m = *D3DXMatrixIdentity(&m);
	RenderSquare(x1, y1, x2, y2, texture, &m, postTransform);
}

void RenderHelper::RenderSquare(const float& x1, const float& y1, const float& x2, const float& y2, Texture* texture, D3DXMATRIX* preTransform, D3DXMATRIX* postTransform)
{
	static float _x1, _x2, _y1, _y2;
	_x1 = min(x1, x2);
	_x2 = max(x1, x2);
	_y1 = min(y1, y2);
	_y2 = max(y1, y2);

	static D3DXMATRIX mtp;
	D3DXMatrixTranslation(&mtp, _x1, _y1, 0);
	D3DXMatrixScaling(&mTransform, _x2 - _x1, _y2 - _y1, 1);

	GraphicsEngine::SetRasterizer(rast);
	if (texture)
		texture->ApplyTexture();
	ShaderManager::SetMatrixWorld(&(*preTransform * mTransform * mtp * *postTransform));

	static UINT STRIDE = sizeof(VERTEX_GUI);
	static UINT OFFSET = 0;

	devcon->IASetVertexBuffers(0, 1, &vBufferGuiSquare, &STRIDE, &OFFSET);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	devcon->Draw(4, 0);
	GraphicsEngine::SetRasterizer(null);
}