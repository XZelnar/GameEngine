#pragma once

#include "../stdafx.h"

class Texture
{
public:
	Texture();
	Texture(ID3D11ShaderResourceView* _texture);
	void Dispose();
	ID3D11ShaderResourceView* GetTexture();
	void ApplyTexture(int index = 0);
	const wchar_t* GetFilename() { return filename; }

public://static
	static Texture* LoadFromFile(LPCWSTR filename);

protected:
	ID3D11ShaderResourceView* texture;

private:
	wchar_t* filename;
};