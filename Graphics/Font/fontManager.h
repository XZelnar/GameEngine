#pragma once

#include "../../stdafx.h"
#include "fontTypes.h"

using namespace std;

class Font;

class FontManager
{
private:
	FontManager() { }
	FontManager(const FontManager& f) { }

public:
	static FontManager& GetInstance()
	{
		static FontManager i;
		return i;
	}

public:
	void Initialize();
	void Dispose();
	Font* GetFont(char* name, int size, FONT_STYLE modifiers);
	bool FontExists(char* name, int size, FONT_STYLE modifiers);

private:
	Font* getFont(char* name, int size, FONT_STYLE modifiers);
	Font* loadFont(char* name, int size, FONT_STYLE modifiers);

private:
	vector<Font*> fonts;
};