#pragma once

#include "../../stdafx.h"
#include "fontTypes.h"

using namespace std;

class Texture;
class CachedText;

class Font
{
	friend class FontManager;

public:
	Font();
	void Initialize();
	void Dispose();
	FONT_LETTER GetCharacter(char c);
	int GetHeight() { return size; }
	void Print(std::string text, TEXT_ALIGNMENT alignment, float width, bool _cache);
	D3DXVECTOR2 MeasureString(std::string text, bool _cache);
	void Uncache(std::string text, TEXT_ALIGNMENT alignment, float width);

private:
	CachedText* getCachedText(std::string text, TEXT_ALIGNMENT alignment, float width, bool& isCached);

private:
	FONT_LETTER* fontMap;
	int fontMapSize;
	Texture* texture;
	char* name;
	int size;
	byte style;
	int tWidth, tHeight;
	vector<CachedText*> cache;

public:
	static Font* LoadFont(const char* folder, const char* filename);
};