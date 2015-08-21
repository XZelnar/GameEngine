#pragma once

#include "../stdafx.h"
#include "texture.h"

class TextureDynamic : public Texture
{
public:
	TextureDynamic(int width, int height);
	virtual void Dispose();
	void SetPixels(void* data);
	
	inline int GetWidth() { return w; }
	inline int GetHeight() { return h; }

private:
	TextureDynamic() { }

private:
	ID3D11Texture2D* pTexture;
	ID3D11DeviceContext* devcon;
	int w, h;
};