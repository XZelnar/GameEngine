#include "shaderManager.h"

#include "shader.h"
#include "Camera/cameraManager.h"
#include "Camera/camera.h"
#include "graphicsEngine.h"
#include "../Util/time.h"

Shader* ShaderManager::activeShader = null;
Shader* ShaderManager::defaultShader = null;
D3DXMATRIX ShaderManager::vp;

void ShaderManager::Initialize()
{
}

void ShaderManager::Dispose()
{
}

void ShaderManager::Update()
{
	static float* perFrameShaderData = new float[28];
	static float t;
	int i = 0;

	t = Time::GetTimeSinceStart();
	//_Time
	perFrameShaderData[i++] = t / 20;
	perFrameShaderData[i++] = t;
	perFrameShaderData[i++] = t * 2;
	perFrameShaderData[i++] = t * 3;
	//_SinTime
	perFrameShaderData[i++] = sin(t / 8);
	perFrameShaderData[i++] = sin(t / 4);
	perFrameShaderData[i++] = sin(t / 2);
	perFrameShaderData[i++] = sin(t);
	//_CosTime
	perFrameShaderData[i++] = cos(t / 8);
	perFrameShaderData[i++] = cos(t / 4);
	perFrameShaderData[i++] = cos(t / 2);
	perFrameShaderData[i++] = cos(t);

	t = Time::GetGameTimeSinceStart();
	//_Time
	perFrameShaderData[i++] = t / 20;
	perFrameShaderData[i++] = t;
	perFrameShaderData[i++] = t * 2;
	perFrameShaderData[i++] = t * 3;
	//_SinTime
	perFrameShaderData[i++] = sin(t / 8);
	perFrameShaderData[i++] = sin(t / 4);
	perFrameShaderData[i++] = sin(t / 2);
	perFrameShaderData[i++] = sin(t);
	//_CosTime
	perFrameShaderData[i++] = cos(t / 8);
	perFrameShaderData[i++] = cos(t / 4);
	perFrameShaderData[i++] = cos(t / 2);
	perFrameShaderData[i++] = cos(t);

	//_DeltaTime
	t = Time::GetDeltaTime();
	perFrameShaderData[i++] = t;
	perFrameShaderData[i++] = 1.0f / t;
	t = Time::GetGameDeltaTime();
	perFrameShaderData[i++] = t;
	perFrameShaderData[i++] = 1.0f / t;

	Shader::SetCBufferPerFrame(perFrameShaderData);
}



void ShaderManager::SetDefaultShader(Shader* s)
{
	defaultShader = s;
}



void ShaderManager::SetMatrixWorld(D3DXMATRIX* world)
{
	static float* perObjecrShaderData = new float[48];

	D3DXMatrixMultiply((D3DXMATRIX*)(perObjecrShaderData), world, &vp);
	memcpy(perObjecrShaderData + 16, world, sizeof(D3DXMATRIX));
	D3DXMatrixInverse((D3DXMATRIX*)(perObjecrShaderData + 32), 0, world);

	Shader::SetCBufferPerObject(perObjecrShaderData);
}

void ShaderManager::SetTexture(ID3D11ShaderResourceView** texture, int index)
{
	if (!activeShader)
		return;
	activeShader->SetTexture(texture, index);
}



void ShaderManager::SetActiveShader(Shader* s)
{
	if ((s && activeShader == s) || (!s && activeShader == defaultShader))
		return;

	if (activeShader)
		activeShader->Deactivate();

	if (s)
		activeShader = s;
	else
		activeShader = defaultShader;

	if (activeShader)
		activeShader->Activate();
}

void ShaderManager::UpdateCamera()
{
	static Camera* c;
	c = CameraManager::GetCurrentActiveCamera();
	if (c)
	{
		static float* perCameraShaderData = new float[52];
		memcpy(perCameraShaderData, &c->GetViewMatrix(), sizeof(D3DXMATRIX));
		memcpy(perCameraShaderData + 16, &c->GetProjectionMatrix(), sizeof(D3DXMATRIX));
		memcpy(perCameraShaderData + 32, &(vp = c->GetViewProjectionMatrix()), sizeof(D3DXMATRIX));
		memcpy(perCameraShaderData + 48, &c->GetPosition(), sizeof(D3DXVECTOR3));
		Shader::SetCBufferPerCamera(perCameraShaderData);
		//SetMatrixProjection(&c->GetProjectionMatrix());
		//SetMatrixView(&c->GetViewMatrix());
	}
}

void ShaderManager::FrameEnd()
{
	if (activeShader)
	{
		activeShader->Deactivate();
		activeShader = null;
	}
}