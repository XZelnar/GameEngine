#pragma once

#include "../../stdafx.h"

enum TEXT_ALIGNMENT
{
	ALIGN_Left = 1,
	ALIGN_Middle = 2,
	ALIGN_Right = 3
};

enum FONT_STYLE
{
	FS_None = 0,
	FS_Bold = 1,
	FS_Italic = 2,
	FS_ItalicBold = 3,
	FS_BoldItalic = 3
};

struct FONT_LETTER
{
	D3DXVECTOR2 texCoordMin;
	D3DXVECTOR2 texCoordMax;
	D3DXVECTOR2 offset;
	int xAdvance;
	unsigned short _char;
};