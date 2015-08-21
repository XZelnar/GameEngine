#pragma once

#include "../stdafx.h"

class Transformation;

struct LightSource
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 color;
};

class Shader;

class ShaderManager
{
	friend class GraphicsEngine;
	friend class LightManager;
	friend class CameraManager;
	friend class Effect;
	friend class EffectsManager;

public:
	static void Initialize();
	static void Dispose();

	static const Shader* GetActiveShader() { return activeShader; }
	static void SetDefaultShader(Shader* s);

	static void SetMatrixProjection(D3DXMATRIX* proj);
	static void SetMatrixView(D3DXMATRIX* view);
	static void SetMatrixWorld(D3DXMATRIX* world);
	static void SetObjectTransformation(Transformation* trans);
	static void SetTexture(ID3D11ShaderResourceView** texture, int index = 0);
	//static void SetColor(D3DXVECTOR4 color);

	static void SetActiveShader(Shader* s);
	static Shader* LoadFromMemory(LPCSTR filename);

private:
	static void UpdateLights(D3DXVECTOR3 ambient, bool activeLights[], LightSource* lights[]);
	static void UpdateCamera();
	static void FrameEnd();

private:
	static Shader* activeShader;
	static Shader* defaultShader;
};