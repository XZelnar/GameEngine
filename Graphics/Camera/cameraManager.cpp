#include "cameraManager.h"

#include "camera.h"
#include "../graphicsEngine.h"
#include "../shaderManager.h"

Camera* CameraManager::activeCamera = null;
Camera* CameraManager::activeCamera2D = null;

Camera* CameraManager::GetActiveCamera()
{
	return activeCamera;
}

void CameraManager::SetActiveCamera(Camera* camera)
{
	if (activeCamera)
		activeCamera->OnDeactivated();
	activeCamera = camera;
	if (activeCamera)
		camera->OnActivated();
	if (GraphicsEngine::GetRenderPass() == RENDER_PASS_TYPE::RP_REGULAR)
		UpdateShader();
}

Camera* CameraManager::GetActive2DCamera()
{
	return activeCamera2D;
}

Camera* CameraManager::GetCurrentActiveCamera()
{
	return GraphicsEngine::GetRenderPass() == RENDER_PASS_TYPE::RP_GUI ? activeCamera2D : activeCamera;
}

void CameraManager::SetActive2DCamera(Camera* camera)
{
	if (activeCamera2D)
		activeCamera2D->OnDeactivated();
	activeCamera2D = camera;
	if (activeCamera2D)
		camera->OnActivated();
	if (GraphicsEngine::GetRenderPass() == RENDER_PASS_TYPE::RP_GUI)
		UpdateShader();
}

void CameraManager::UpdateShader()
{
	ShaderManager::UpdateCamera();
}

void CameraManager::Update()
{
	if (activeCamera)
		activeCamera->Update();
	if (activeCamera2D)
		activeCamera2D->Update();
}

void CameraManager::OnResolutionChanged()
{
	if (activeCamera)
		activeCamera->OnResolutionChanged();
	if (activeCamera2D)
		activeCamera2D->OnResolutionChanged();
}