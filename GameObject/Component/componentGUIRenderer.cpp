#include "componentGUIRenderer.h"

#include "../../Graphics/graphicsEngine.h"
#include "../../Graphics/texture.h"
#include "../../Graphics/shaderManager.h"
#include "../../Graphics/material.h"

ComponentGUIRenderer::ComponentGUIRenderer()
{
	border = 0.1f;
	vBuffer = null;
	IsVisible = true;
}

void ComponentGUIRenderer::Dispose()
{
	if (vBuffer)
		vBuffer->Release();
}

void ComponentGUIRenderer::SetBorder(float b, float bPix)
{
	border = b;
	borderPix = bPix;
	updateMatrix();
}

void ComponentGUIRenderer::GUI(D3DXMATRIX* world)
{
	assert(texture);
	assert(material);
	if (!IsVisible)
		return;

	static UINT stride = sizeof(VERTEX);
	static UINT offset = 0;
	
	material->Activate();
	texture->ApplyTexture();
	ShaderManager::SetMatrixWorld(world);
	devcon->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->Draw(54, 0);
}



void ComponentGUIRenderer::updateMatrix()
{
	if (vBuffer)
		vBuffer->Release();
	createVBO();
}

void ComponentGUIRenderer::createVBO()
{
	VERTEX v[54];
	float nborder = 1.0f - border;

	//top left -> top left
	v[0].POSITION = position;
	v[0].U = v[0].V = 0;

	//top left -> top right
	v[1].POSITION = v[4].POSITION = position + D3DXVECTOR2(borderPix, 0);
	v[1].U = v[4].U = border;
	v[1].V = v[4].V = 0;

	//top left -> bottom left
	v[2].POSITION = v[3].POSITION = position + D3DXVECTOR2(0, borderPix);
	v[2].U = v[3].U = 0;
	v[2].V = v[3].V = border;

	//top left -> bottom right
	v[5].POSITION = position + D3DXVECTOR2(borderPix, borderPix);
	v[5].U = v[5].V = border;

	//6-11===================================================================================

	//top middle -> top left
	v[6] = v[1];

	//top middle -> top right
	v[7].POSITION = v[10].POSITION = position + D3DXVECTOR2(size.x - borderPix, 0);
	v[7].U = v[10].U = nborder;
	v[7].V = v[10].V = 0;

	//top middle -> bottom left
	v[8] = v[9] = v[5];

	//top middle -> bottom right
	v[11].POSITION = position + D3DXVECTOR2(size.x - borderPix, borderPix);
	v[11].U = nborder;
	v[11].V = border;
	
	//12-17==================================================================================

	//top right -> top left
	v[12] = v[7];

	//top right -> top right
	v[13].POSITION = v[16].POSITION = position + D3DXVECTOR2(size.x, 0);
	v[13].U = v[16].U = 1;
	v[13].V = v[16].V = 0;

	//top right -> bottom left
	v[14] = v[15] = v[11];

	//top right -> bottom right
	v[17].POSITION = position + D3DXVECTOR2(size.x, borderPix);
	v[17].U = 1;
	v[17].V = border;
	
	//=======================================================================================
	//middle=================================================================================
	//=======================================================================================

	//18-23==================================================================================
	
	//middle left -> top left
	v[18] = v[2];

	//middle left -> top right
	v[19] = v[22] = v[5];

	//middle left -> bottom left
	v[20].POSITION = v[21].POSITION = position + D3DXVECTOR2(0, size.y - borderPix);
	v[20].U = v[21].U = 0;
	v[20].V = v[21].V = nborder;

	//middle left -> bottom right
	v[23].POSITION = position + D3DXVECTOR2(borderPix, size.y - borderPix);
	v[23].U = border;
	v[23].V = nborder;

	//24-29==================================================================================

	//middle middle -> top left
	v[24] = v[19];

	//middle middle -> top right
	v[25] = v[28] = v[11];

	//middle middle -> bottom left
	v[26] = v[27] = v[23];

	//middle middle -> bottom right
	v[29].POSITION = position + D3DXVECTOR2(size.x - borderPix, size.y - borderPix);
	v[29].U = v[29].V = nborder;

	//30-35==================================================================================

	//middle right -> top left
	v[30] = v[25];

	//middle right -> top right
	v[31] = v[34] = v[17];

	//middle right -> bottom left
	v[32] = v[33] = v[29];

	//middle right -> bottom right
	v[35].POSITION = position + D3DXVECTOR2(size.x, size.y - borderPix);
	v[35].U = 1;
	v[35].V = nborder;
	
	//=======================================================================================
	//right==================================================================================
	//=======================================================================================

	//36-41==================================================================================
	
	//bottom left -> top left
	v[36] = v[20];

	//bottom left -> top right
	v[37] = v[40] = v[23];

	//bottom left -> bottom left
	v[38].POSITION = v[39].POSITION = position + D3DXVECTOR2(0, size.y);
	v[38].U = v[39].U = 0;
	v[38].V = v[39].V = 1;

	//middle left -> bottom right
	v[41].POSITION = position + D3DXVECTOR2(borderPix, size.y);
	v[41].U = border;
	v[41].V = 1;

	//42-47==================================================================================

	//bottom middle -> top left
	v[42] = v[37];

	//bottom middle -> top right
	v[43] = v[46] = v[29];

	//bottom middle -> bottom left
	v[44] = v[45] = v[41];

	//bottom middle -> bottom right
	v[47].POSITION = position + D3DXVECTOR2(size.x - borderPix, size.y);
	v[47].U = nborder;
	v[47].V = 1;

	//48-53==================================================================================

	//bottom right -> top left
	v[48] = v[43];

	//bottom right -> top right
	v[49] = v[52] = v[35];

	//bottom right -> bottom left
	v[50] = v[51] = v[47];

	//bottom right -> bottom right
	v[53].POSITION = position + size;
	v[53].U = 1;
	v[53].V = 1;
	


	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX) * 54;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GraphicsEngine::GetDevice()->CreateBuffer(&bd, NULL, &vBuffer);

	D3D11_MAPPED_SUBRESOURCE ms;
	GraphicsEngine::GetContext()->Map(vBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, v, 54 * sizeof(VERTEX));
	GraphicsEngine::GetContext()->Unmap(vBuffer, NULL);
}