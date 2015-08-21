#include "font.h"

#include "../texture.h"
#include "cachedText.h"
#include "../graphicsResourceManager.h"

Font::Font()
{
	fontMap = null;
	fontMapSize = 0;
	name = null;
	texture = null;
	style = FONT_STYLE::FS_None;
	tWidth = 0;
	tHeight = 0;
}

void Font::Initialize()
{
}

void Font::Dispose()
{
	for	(int i = 0; i < cache.size(); i++)
		if (cache[i])
		{
			cache[i]->Dispose();
			delete cache[i];
		}
	cache.clear();

	delete[] fontMap;
	fontMap = null;
	texture = null;
	if (name)
		delete[] name;
	name = null;
}

FONT_LETTER Font::GetCharacter(char c)
{
	for(int i = 0; i < fontMapSize; i++)
		if (fontMap[i]._char == c)
			return fontMap[i];
	return GetCharacter(' ');
}

void Font::Print(std::string text, TEXT_ALIGNMENT alignment, float width, bool _cache)
{
	bool isCached;
	texture->ApplyTexture();
	CachedText* t = getCachedText(text, alignment, width, isCached);
	t->Render();
	if (_cache)
	{
		if (!isCached)
			cache.push_back(t);
	}
	else
	{
		t->Dispose();
		delete t;
	}
}

D3DXVECTOR2 Font::MeasureString(std::string text, bool _cache)
{
	bool isCached;
	CachedText* t = getCachedText(text, TEXT_ALIGNMENT::ALIGN_Left, 100, isCached);
	auto size = t->GetSize();
	if (_cache)
	{
		if (!isCached)
			cache.push_back(t);
	}
	else
	{
		t->Dispose();
		delete t;
	}
	return size;
}

void Font::Uncache(std::string text, TEXT_ALIGNMENT alignment, float width)
{
	CachedText* c;
	for (auto i = cache.begin(); i != cache.end(); i++)
	{
		c = *i;
		if (c && c->IsTextEqual((char*)text.c_str()) && c->GetAlignment() == alignment && (alignment == ALIGN_Left || c->GetAreaWidth() == width))
		{
			c->Dispose();
			delete c;
			auto old = i;
			i++;
			cache.erase(old, i);
			return;
		}
	}
}


CachedText* Font::getCachedText(std::string text, TEXT_ALIGNMENT alignment, float width, bool& isCached)
{
	for (int i = 0; i < cache.size(); i++)
		if (cache[i] && cache[i]->IsTextEqual((char*)text.c_str()) && cache[i]->GetAlignment() == alignment && (alignment == ALIGN_Left || cache[i]->GetAreaWidth() == width))
		{
			isCached = true;
			return cache[i];
		}

	isCached = false;
	return CachedText::CacheText((char*)text.c_str(), this, alignment, width);
}



inline void readLine(ifstream& ifs)
{
	static char c;
	while(!ifs.eof() && (c = ifs.get()) != '\n') ;
}

Font* Font::LoadFont(const char* folder, const char* filename)
{
	char tpbuf[80];
	strcpy_s(tpbuf, folder);
	strcat_s(tpbuf, filename);
	ifstream ifs;
	ifs.open(tpbuf);
	if (ifs.fail())
		return null;

	Font* f = new Font();

	char buf[80];
	wchar_t fn[80];
	char c;
	int i, t;
	//====================================================================================
	//================================================================================info
	//====================================================================================
	//read start tag
	ifs.get(buf, 6);//info 
	//read font type
	ifs.get(buf, 7);//face="
	for(i = 0; i < 32; i++)
	{
		c = ifs.get();
		if (c == '"')
		{
			buf[i] = 0;
			break;
		}
		else
			buf[i] = c;
	}
	f->name = new char[i + 1];
	strcpy(f->name, buf);
	//read font size
	ifs.get(buf, 7);// size=
	ifs >> f->size;
	//read bold
	ifs.get(buf, 7);// bold=
	ifs >> i;
	if (i)
		f->style |= FONT_STYLE::FS_Bold;
	//read italic
	ifs.get(buf, 9);// italic=
	ifs >> i;
	if (i)
		f->style |= FONT_STYLE::FS_Italic;
	readLine(ifs);//trash
	//====================================================================================
	//==============================================================================common
	//====================================================================================
	ifs.get(buf, 19);//common lineHeight=
	ifs >> i;
	ifs.get(buf, 7);// base=
	ifs >> i;
	//width
	ifs.get(buf, 9);// scaleW=
	ifs >> f->tWidth;
	//height
	ifs.get(buf, 9);// scaleH=
	ifs >> f->tHeight;
	readLine(ifs);//trash
	//====================================================================================
	//================================================================================page
	//====================================================================================
	ifs.get(buf, 17);//page id=0 file="
	for(i = 0; i < 32; i++)
	{
		c = ifs.get();
		if (c == '"')
		{
			fn[i] = 0;
			break;
		}
		else
			fn[i] = c;
	}
	wchar_t tpbuf2[80];
	mbstowcs(tpbuf2, folder, strlen(folder) + 1);
	wcscat(tpbuf2, fn);
	f->texture = GraphicsResourceManager::GetInstance().GetTexture(tpbuf2);
	readLine(ifs);//trash
	//====================================================================================
	//===============================================================================chars
	//====================================================================================
	ifs.get(buf, 13);//chars count=
	ifs >> f->fontMapSize;
	f->fontMap = new FONT_LETTER[f->fontMapSize];
	readLine(ifs);//trash
	//====================================================================================
	//================================================================================char
	//====================================================================================
	for (i = 0; i < f->fontMapSize; i++)
	{
		ifs.get(buf, 9);//char id=
		ifs >> f->fontMap[i]._char;
		while (ifs.get() == ' ') ;
		//x
		ifs.get();
		ifs >> t;
		f->fontMap[i].texCoordMin.x = (float)t / f->tWidth;
		while (ifs.get() == ' ') ;
		//y
		ifs.get();
		ifs >> t;
		f->fontMap[i].texCoordMin.y = (float)t / f->tHeight;
		while (ifs.get() == ' ') ;
		//x2
		ifs.get(buf, 6);
		ifs >> t;
		f->fontMap[i].texCoordMax.x = (float)t / f->tWidth + f->fontMap[i].texCoordMin.x;
		while (ifs.get() == ' ') ;
		//y2
		ifs.get(buf, 7);
		ifs >> t;
		f->fontMap[i].texCoordMax.y = (float)t / f->tHeight + f->fontMap[i].texCoordMin.y;
		while (ifs.get() == ' ') ;
		//xoff
		ifs.get(buf, 8);
		ifs >> f->fontMap[i].offset.x;
		while (ifs.get() == ' ') ;
		//yoff
		ifs.get(buf, 8);
		ifs >> f->fontMap[i].offset.y;
		while (ifs.get() == ' ') ;
		//xadv
		ifs.get(buf, 9);
		ifs >> f->fontMap[i].xAdvance;
		readLine(ifs);//trash
	}

	ifs.close();
	f->Initialize();
	return f;
}