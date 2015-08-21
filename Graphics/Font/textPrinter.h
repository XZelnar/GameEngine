#pragma once

#include "../../stdafx.h"
#include "fontTypes.h"

class Material;

class TextPrinter
{
	friend class GraphicsEngine;

public:
	static void Print(const std::string& text, const char* fontName, int fontSize, FONT_STYLE fontStyle, D3DXMATRIX* mWorld, D3DXVECTOR4 color = D3DXVECTOR4(1, 1, 1, 1), 
		TEXT_ALIGNMENT alignment = ALIGN_Left, float width = 100, bool cache = false);
	static void Print(const std::string& text, const char* fontName, int fontSize, FONT_STYLE fontStyle, const float& x, const float& y, D3DXVECTOR4 color = D3DXVECTOR4(1, 1, 1, 1), 
		TEXT_ALIGNMENT alignment = ALIGN_Left, float width = 100, bool cache = false);
	static D3DXVECTOR2 MeasureString(const std::string& text, const char* fontName, int fontSize, FONT_STYLE fontStyle, bool cache = false);
	static void Uncache(const std::string& text, const char* fontName, int fontSize, FONT_STYLE fontStyle, TEXT_ALIGNMENT alignment = ALIGN_Left, float width = 100);

private:
	static void StaticInit();
	static void StaticDispose();

private:
	static Material* m;
};