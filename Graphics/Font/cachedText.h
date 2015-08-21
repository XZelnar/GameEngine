#pragma once

#include "../../stdafx.h"
#include "fontTypes.h"

class Font;

class CachedText
{
public:
	void Dispose();
	void Render();
	bool IsTextEqual(const char* txt);
	inline TEXT_ALIGNMENT GetAlignment() { return alignment; }
	inline float GetAreaWidth() { return areaWidth; }
	inline D3DXVECTOR2 GetSize() { return size; }

private:
	CachedText();
	void genBuffer(Font* font);
	void genArrayLeft(Font* font);
	void genArrayMiddle(Font* font);
	void genArrayRight(Font* font);

public://static
	static CachedText* CacheText(char* txt, Font* font, TEXT_ALIGNMENT al, float width = -1);

private://static
	struct VERTEX
	{
		float X, Y;
		D3DXVECTOR2 TEXCOORD;
	};

private:
	VERTEX* vertices;
	ID3D11Buffer* vBuffer;
	int verticesCount;
	char* text;
	TEXT_ALIGNMENT alignment;
	float areaWidth;//used in alignment
	D3DXVECTOR2 size;
};