#include "textPrinter.h"

#include "fontManager.h"
#include "font.h"
#include "../shaderManager.h"
#include "../graphicsResourceManager.h"
#include "../material.h"
#include "../shader.h"

Material* TextPrinter::m = null;
void TextPrinter::Print(const std::string& text, const char* fontName, int fontSize, FONT_STYLE fontStyle, D3DXMATRIX* mWorld, D3DXVECTOR4 color, 
		TEXT_ALIGNMENT alignment, float width, bool cache)
{
	static Font* font;
	font = FontManager::GetInstance().GetFont((char*)fontName, fontSize, fontStyle);
	if (!font)
		return;
	m->SetPixelCBuffer(0, color);
	m->Activate();
	ShaderManager::SetMatrixWorld(mWorld);
	font->Print(text, alignment, width, cache);
}

void TextPrinter::Print(const std::string& text, const char* fontName, int fontSize, FONT_STYLE fontStyle, const float& x, const float& y, D3DXVECTOR4 color, 
		TEXT_ALIGNMENT alignment, float width, bool cache)
{
	static D3DXMATRIX m;
	D3DXMatrixTranslation(&m, x, y, 0);
	Print(text, fontName, fontSize, fontStyle, &m, color, alignment, width, cache);
}

D3DXVECTOR2 TextPrinter::MeasureString(const std::string& text, const char* fontName, int fontSize, FONT_STYLE fontStyle, bool cache)
{
	static Font* font;
	font = FontManager::GetInstance().GetFont((char*)fontName, fontSize, fontStyle);
	if (!font)
		throw exception("Font not found");
	return font->MeasureString(text, cache);
}

void TextPrinter::Uncache(const std::string& text, const char* fontName, int fontSize, FONT_STYLE fontStyle, TEXT_ALIGNMENT alignment, float width)
{
	static Font* font;
	font = FontManager::GetInstance().GetFont((char*)fontName, fontSize, fontStyle);
	if (!font)
		throw exception("Font not found");
	font->Uncache(text, alignment, width);
}

void TextPrinter::StaticInit()
{
	m = new Material(GraphicsResourceManager::GetInstance().GetShader(L"Resources/Shaders/text.fx"));
	m->updateTxtures = false;
	static D3D11_INPUT_ELEMENT_DESC ied3d[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	m->SetInputLayout(ied3d, _countof(ied3d));
}

void TextPrinter::StaticDispose()
{
	delete m;
}