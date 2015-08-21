#include "cachedText.h"

#include "font.h"
#include "../graphicsEngine.h"

void CachedText::Dispose()
{
	if (vBuffer)
	{
		vBuffer->Release();
		vBuffer = null;
	}
	delete[] text;
}

bool CachedText::IsTextEqual(const char* txt)
{
	return !strcmp(txt, text);
}

void CachedText::Render()
{
	static UINT stride = sizeof(VERTEX);
	static UINT offset = 0;
    GraphicsEngine::GetContext()->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
    GraphicsEngine::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GraphicsEngine::GetContext()->Draw(verticesCount, 0);
}



void CachedText::genBuffer(Font* font)
{
	if (alignment == ALIGN_Left)
		genArrayLeft(font);
	else if (alignment == ALIGN_Middle)
		genArrayMiddle(font);
	else if (alignment == ALIGN_Right)
		genArrayRight(font);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX) * verticesCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GraphicsEngine::GetDevice()->CreateBuffer(&bd, NULL, &vBuffer);

	D3D11_MAPPED_SUBRESOURCE ms;
	GraphicsEngine::GetContext()->Map(vBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, vertices, verticesCount * sizeof(VERTEX));
	GraphicsEngine::GetContext()->Unmap(vBuffer, NULL);
	
	delete[] vertices;
}

void CachedText::genArrayLeft(Font* font)
{
	verticesCount = strlen(text) * 6;
	vertices = new VERTEX[verticesCount];

	unsigned short c;
	int fonty = font->GetHeight();
	int curx = 0, cury = 0;
	int adv;
	FONT_LETTER l;
	for(int i = 0; i < verticesCount; i += 6)
	{
		c = text[i / 6];
		if (c == '\n')
		{
			vertices[i].X = vertices[i].Y = 0;
			vertices[i].TEXCOORD = D3DXVECTOR2(0, 0);
			vertices[i + 1] = vertices[i];
			vertices[i + 2] = vertices[i];
			vertices[i + 3] = vertices[i];
			vertices[i + 4] = vertices[i];
			vertices[i + 5] = vertices[i];

			size.x = max(size.x, curx);
			curx = 0;
			cury += fonty;
			continue;
		}

		l = font->GetCharacter(c);
		curx += l.offset.x;
		cury += l.offset.y;
		fonty -= l.offset.y * 2;
		adv = l.xAdvance - l.offset.x * 2;
		
		//left down
		vertices[i + 1].X = curx;
		vertices[i + 1].Y = cury;
		vertices[i + 1].TEXCOORD = l.texCoordMin;
		vertices[i + 4] = vertices[i + 1];

		//left up
		vertices[i + 2].X = curx;
		vertices[i + 2].Y = cury + fonty;
		vertices[i + 2].TEXCOORD = D3DXVECTOR2(l.texCoordMin.x, l.texCoordMax.y);

		//right up
		vertices[i].X = curx + adv;
		vertices[i].Y = cury + fonty;
		vertices[i].TEXCOORD = l.texCoordMax;
		vertices[i + 5] = vertices[i];

		//right down
		vertices[i + 3].X = curx + adv;
		vertices[i + 3].Y = cury;
		vertices[i + 3].TEXCOORD = D3DXVECTOR2(l.texCoordMax.x, l.texCoordMin.y);

		curx += l.xAdvance - l.offset.x;
		cury -= l.offset.y;
		fonty += l.offset.y * 2;
	}

	size.x = max(size.x, curx);
	size.y = cury + fonty;
}

void CachedText::genArrayMiddle(Font* font)
{
	verticesCount = strlen(text) * 6;
	vertices = new VERTEX[verticesCount];

	unsigned short c;
	int fonty = font->GetHeight();
	int curx = 0, cury = 0;
	int adv;
	int lineStart = 0;
	int i;
	FONT_LETTER l;
	for(i = 0; i < verticesCount; i += 6)
	{
		c = text[i / 6];
		if (c == '\n')
		{
			vertices[i].X = vertices[i].Y = 0;
			vertices[i].TEXCOORD = D3DXVECTOR2(0, 0);
			vertices[i + 1] = vertices[i];
			vertices[i + 2] = vertices[i];
			vertices[i + 3] = vertices[i];
			vertices[i + 4] = vertices[i];
			vertices[i + 5] = vertices[i];

			float delta = (areaWidth - curx) / 2;
			for(int ti = lineStart; ti < i; ti++)
				vertices[ti].X += delta;
			lineStart = i + 1;

			size.x = max(size.x, curx);
			curx = 0;
			cury += fonty;
			continue;
		}

		l = font->GetCharacter(c);
		curx += l.offset.x;
		cury += l.offset.y;
		fonty -= l.offset.y * 2;
		adv = l.xAdvance - l.offset.x * 2;
		
		//left down
		vertices[i + 2].X = curx;
		vertices[i + 2].Y = cury;
		vertices[i + 2].TEXCOORD = l.texCoordMin;
		vertices[i + 5] = vertices[i + 2];

		//left up
		vertices[i + 1].X = curx;
		vertices[i + 1].Y = cury + fonty;
		vertices[i + 1].TEXCOORD = D3DXVECTOR2(l.texCoordMin.x, l.texCoordMax.y);

		//right up
		vertices[i].X = curx + adv;
		vertices[i].Y = cury + fonty;
		vertices[i].TEXCOORD = l.texCoordMax;
		vertices[i + 4] = vertices[i];

		//right down
		vertices[i + 3].X = curx + adv;
		vertices[i + 3].Y = cury;
		vertices[i + 3].TEXCOORD = D3DXVECTOR2(l.texCoordMax.x, l.texCoordMin.y);

		curx += l.xAdvance - l.offset.x;
		cury -= l.offset.y;
		fonty += l.offset.y * 2;
	}

	float delta = (areaWidth - curx) / 2;
	for(int ti = lineStart; ti < i; ti++)
		vertices[ti].X += delta;

	size.x = max(size.x, curx);
	size.y = cury + fonty;
}

void CachedText::genArrayRight(Font* font)
{
	verticesCount = strlen(text) * 6;
	vertices = new VERTEX[verticesCount];

	unsigned short c;
	int fonty = font->GetHeight();
	int curx = 0, cury = 0;
	int adv;
	int lineStart = 0;
	int i;
	FONT_LETTER l;
	for(i = 0; i < verticesCount; i += 6)
	{
		c = text[i / 6];
		if (c == '\n')
		{
			vertices[i].X = vertices[i].Y = 0;
			vertices[i].TEXCOORD = D3DXVECTOR2(0, 0);
			vertices[i + 1] = vertices[i];
			vertices[i + 2] = vertices[i];
			vertices[i + 3] = vertices[i];
			vertices[i + 4] = vertices[i];
			vertices[i + 5] = vertices[i];

			float delta = areaWidth - curx;
			for(int ti = lineStart; ti < i; ti++)
				vertices[ti].X += delta;
			lineStart = i + 1;

			size.x = max(size.x, curx);
			curx = 0;
			cury += fonty;
			continue;
		}

		l = font->GetCharacter(c);
		curx += l.offset.x;
		cury += l.offset.y;
		fonty -= l.offset.y * 2;
		adv = l.xAdvance - l.offset.x * 2;
		
		//left down
		vertices[i].X = curx;
		vertices[i].Y = cury;
		vertices[i].TEXCOORD = l.texCoordMin;
		vertices[i + 4] = vertices[i];

		//left up
		vertices[i + 1].X = curx;
		vertices[i + 1].Y = cury + fonty;
		vertices[i + 1].TEXCOORD = D3DXVECTOR2(l.texCoordMin.x, l.texCoordMax.y);

		//right up
		vertices[i + 2].X = curx + adv;
		vertices[i + 2].Y = cury + fonty;
		vertices[i + 2].TEXCOORD = l.texCoordMax;
		vertices[i + 5] = vertices[i + 2];

		//right down
		vertices[i + 3].X = curx + adv;
		vertices[i + 3].Y = cury;
		vertices[i + 3].TEXCOORD = D3DXVECTOR2(l.texCoordMax.x, l.texCoordMin.y);

		curx += l.xAdvance - l.offset.x;
		cury -= l.offset.y;
		fonty += l.offset.y * 2;
	}

	float delta = areaWidth - curx;
	for(int ti = lineStart; ti < i; ti++)
		vertices[ti].X += delta;

	size.x = max(size.x, curx);
	size.y = cury + fonty;
}



CachedText::CachedText()
{
	vertices = null;
	vBuffer = null;
	verticesCount = 0;
	text = null;
	alignment = ALIGN_Left;
	size = D3DXVECTOR2(0, 0);
}

CachedText* CachedText::CacheText(char* txt, Font* font, TEXT_ALIGNMENT al, float width)
{
	CachedText* t = new CachedText();
	t->alignment = al;
	t->text = new char[strlen(txt)+1];
	strcpy(t->text, txt);
	t->areaWidth = width;

	t->genBuffer(font);

	return t;
}