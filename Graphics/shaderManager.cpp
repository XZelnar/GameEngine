#include "shaderManager.h"

#include "shader.h"
#include "Camera/cameraManager.h"
#include "Camera/camera.h"
#include "graphicsEngine.h"

Shader* ShaderManager::activeShader = null;
Shader* ShaderManager::defaultShader = null;

void ShaderManager::Initialize()
{
}

void ShaderManager::Dispose()
{
}



void ShaderManager::SetDefaultShader(Shader* s)
{
	defaultShader = s;
}



void ShaderManager::SetMatrixProjection(D3DXMATRIX* proj)
{
	if (!activeShader)
		return;
	activeShader->SetProjectionMatrix(proj);
}

void ShaderManager::SetMatrixView(D3DXMATRIX* view)
{
	if (!activeShader)
		return;
	activeShader->SetViewMatrix(view);
}

void ShaderManager::SetMatrixWorld(D3DXMATRIX* world)
{
	if (!activeShader)
		return;
	activeShader->SetWorldMatrix(world);
}

void ShaderManager::SetObjectTransformation(Transformation* trans)
{
	if (!activeShader)
		return;
	activeShader->SetObjectTransformation(trans);
}

void ShaderManager::SetTexture(ID3D11ShaderResourceView** texture, int index)
{
	if (!activeShader)
		return;
	activeShader->SetTexture(texture, index);
}



Shader* ShaderManager::LoadFromMemory(LPCSTR filename)
{
	return Shader::LoadFromMemory(filename);
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
	{
		activeShader->Activate();
		UpdateCamera();
	}
}

void ShaderManager::UpdateCamera()
{
	static Camera* c;
	c = CameraManager::GetCurrentActiveCamera();
	if (c)
	{
		SetMatrixProjection(&c->GetProjectionMatrix());
		SetMatrixView(&c->GetViewMatrix());
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