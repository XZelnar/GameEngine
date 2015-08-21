#pragma once

#include "../../stdafx.h"

class Camera;

class CameraManager
{
public:
	static Camera* GetActiveCamera();
	static Camera* GetActive2DCamera();
	static Camera* GetCurrentActiveCamera();
	static void SetActiveCamera(Camera* camera);
	static void SetActive2DCamera(Camera* camera);
	static void UpdateShader();
	static void Update();
	static void OnResolutionChanged();

private:
	static Camera* activeCamera;
	static Camera* activeCamera2D;
};