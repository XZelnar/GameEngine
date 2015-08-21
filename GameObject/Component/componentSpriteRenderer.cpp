#include "componentSpriteRenderer.h"

#include "../../Graphics/graphicsEngine.h"
#include "../../Graphics/shaderManager.h"
#include "../../Graphics/texture.h"
#include "../../Graphics/material.h"

ID3D11Buffer* ComponentSpriteRenderer::vBuffer;

ComponentSpriteRenderer::ComponentSpriteRenderer()
{
	if (!vBuffer)
		StaticInit();
	IsVisible = true;
	devcon = GraphicsEngine::GetContext();
	position.x = 0;
	position.y = 0;
	rotation = 0;
	size.x = 0;
	size.y = 0;
	material = null;
	D3DXMatrixIdentity(&mWorld);
}

void ComponentSpriteRenderer::Dispose()
{
}

void ComponentSpriteRenderer::SetTexture(Texture* t)
{
	texture = t;
	if (size.x == 0 && size.y == 0)
		SetSize(D3DXVECTOR2(16, 16));
}

void ComponentSpriteRenderer::SetPosition(D3DXVECTOR2 pos)
{
	position = pos;
	updateMatrix();
}

void ComponentSpriteRenderer::SetRotation(float rot)
{
	rotation = rot;
	updateMatrix();
}

void ComponentSpriteRenderer::SetSize(D3DXVECTOR2 s)
{
	size = s;
	updateMatrix();
}

void ComponentSpriteRenderer::updateMatrix()
{
	D3DXMatrixTranslation(&tpPos, position.x, position.y, 0);
	D3DXMatrixRotationZ(&tpRot, rotation);
	D3DXMatrixScaling(&tpScale, size.x, size.y, 1);
	mWorld = tpScale * tpPos;
}

void ComponentSpriteRenderer::GUI()
{
	if (!texture || !IsVisible)
		return;
	if (!material)
		;// mat = (Effect*)EffectsManager::GetGUIEffect();//TODO

	static UINT stride = sizeof(VERTEX);
	static UINT offset = 0;
	
	material->Activate();
	texture->ApplyTexture();
	ShaderManager::SetMatrixWorld(&mWorld);
	devcon->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	devcon->Draw(4, 0);
}


void ComponentSpriteRenderer::StaticDispose()
{
	if (vBuffer)
		vBuffer->Release();
}

void ComponentSpriteRenderer::StaticInit()
{
	VERTEX vertices[] = {
		{ D3DXVECTOR2(0, 0), 0, 0 },
		{ D3DXVECTOR2(1, 0), 1, 0 },
		{ D3DXVECTOR2(0, 1), 0, 1 },
		{ D3DXVECTOR2(1, 1), 1, 1 }
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GraphicsEngine::GetDevice()->CreateBuffer(&bd, NULL, &vBuffer);

	D3D11_MAPPED_SUBRESOURCE ms;
	GraphicsEngine::GetContext()->Map(vBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, vertices, 4 * sizeof(VERTEX));
	GraphicsEngine::GetContext()->Unmap(vBuffer, NULL);
}