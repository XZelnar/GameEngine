#include "cameraOrtho.h"

#include "../../window.h"

CameraOrtho::CameraOrtho()
{
	D3DXMatrixTranslation(&viewMatrix, -Window::GetWidth() / 2, -Window::GetHeight() / 2, 0);
	D3DXMatrixOrthoLH(&projectionMatrix, Window::GetWidth(), Window::GetHeight(), 0, 1);
}

void CameraOrtho::OnActivated()
{
	OnResolutionChanged();
}

void CameraOrtho::OnResolutionChanged()
{
	D3DXMatrixTranslation(&viewMatrix, -Window::GetWidth() / 2, -Window::GetHeight() / 2, 0);
	D3DXMatrixOrthoLH(&projectionMatrix, Window::GetWidth(), -Window::GetHeight(), 0, 1);
}

D3DXMATRIX CameraOrtho::GetViewMatrix()
{
	return viewMatrix;
}

D3DXMATRIX CameraOrtho::GetProjectionMatrix()
{
	return projectionMatrix;
}

void CameraOrtho::updateViewMatrix()
{
	recalculateViewMatrix = false;
	D3DXMatrixTranslation(&viewMatrix, -Window::GetWidth() / 2, -Window::GetHeight() / 2, 0);
}

void CameraOrtho::updateProjectionMatrix()
{
	recalculateProjectionMatrix = false;
	D3DXMatrixOrthoLH(&projectionMatrix, Window::GetWidth(), -Window::GetHeight(), 0, 1);
}