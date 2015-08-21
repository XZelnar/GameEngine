#include "fontManager.h"

#include "font.h"

void FontManager::Initialize()
{
}

void FontManager::Dispose()
{
	for (int i = 0; i < fonts.size(); i++)
		if (fonts[i] != null)
		{
			fonts[i]->Dispose();
			delete fonts[i];
		}
	fonts.clear();
}

Font* FontManager::GetFont(char* name, int size, FONT_STYLE modifiers)
{
	Font* f = getFont(name, size, modifiers);
	if (f)
		return f;
	return loadFont(name, size, modifiers);
}

bool FontManager::FontExists(char* name, int size, FONT_STYLE modifiers)
{
	return getFont(name, size, modifiers);
}



Font* FontManager::getFont(char* name, int size, FONT_STYLE modifiers)
{
	static Font* t;
	for (int i = 0; i < fonts.size(); i++)
		if ((t = fonts[i]) != null && !strcmp(name, t->name) && t->size == size && t->style == modifiers)
			return fonts[i];
	return null;
}

Font* FontManager::loadFont(char* name, int size, FONT_STYLE modifiers)
{
	char buf[10];
	string s;
	s.append(name);
	s.append("_");
	_itoa_s(size, buf, 10);
	s.append(buf);
	s.append(modifiers == FS_Bold ? "_b" : modifiers == FS_Italic ? "_i" : modifiers == FS_ItalicBold ? "_ib" : "");
	s.append(".fnt");

	Font* f = Font::LoadFont("Resources/Fonts/", s.c_str());
	fonts.push_back(f);
	return f;
}