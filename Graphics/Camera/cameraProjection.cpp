#include "cameraProjection.h"

#include "../../window.h"

CameraProjection::CameraProjection()
{
	position = D3DXVECTOR3(0, 0, -10);
	lookAt = D3DXVECTOR3(0, 0, 0);
	upVector = D3DXVECTOR3(0, 1, 0);
	fov = D3DX_PI / 4;
	nearClipPlane = 0.1f;
	farClipPlane = 1000;
	recalculateProjectionMatrix = true;
	recalculateViewMatrix = true;
}

CameraProjection::CameraProjection(float fov, float nearPlane, float farPlane)
{
	position = D3DXVECTOR3(0, 0, -4);
	lookAt = D3DXVECTOR3(0, 0, 0);
	upVector = D3DXVECTOR3(0, 1, 0);
	SetFOV(fov);
	SetClipPlanes(nearPlane, farPlane);
	recalculateProjectionMatrix = true;
	recalculateViewMatrix = true;
}

D3DXVECTOR3 CameraProjection::GetPosition()
{
	return position;
}

void CameraProjection::MoveCamera(D3DXVECTOR3 delta, bool moveLookAt)
{
	position += delta;
	if (moveLookAt)
		lookAt += delta;
	recalculateViewMatrix = true;
}

void CameraProjection::SetPosition(D3DXVECTOR3 pos, bool moveLookAt)
{
	if (moveLookAt)
		lookAt += pos - position;
	position = pos;
	recalculateViewMatrix = true;
}

void CameraProjection::SetPosition(D3DXVECTOR3 pos, D3DXVECTOR3 _lookAt)
{
	position = pos;
	lookAt = _lookAt;
	recalculateViewMatrix = true;
}

D3DXVECTOR3 CameraProjection::GetLookAt()
{
	return lookAt;
}

void CameraProjection::SetLookAt(D3DXVECTOR3 _lookAt)
{
	lookAt = _lookAt;
	recalculateViewMatrix = true;
}

void CameraProjection::GetClipPlanes(float& nearPlane, float& farPlane)
{
	nearPlane = nearClipPlane;
	farPlane = farClipPlane;
}

void CameraProjection::SetClipPlanes(float nearPlane, float farPlane)
{
	nearClipPlane = nearPlane;
	farClipPlane = farPlane;
	recalculateProjectionMatrix = true;
}

float CameraProjection::GetFOV()
{
	return fov;
}

void CameraProjection::SetFOV(float _fov)
{
	if (fov == _fov)
		return;
	fov = _fov;
	recalculateProjectionMatrix = true;
}


//======================================================================================================================================
//======================================================================================================================================
//======================================================================================================================================

void CameraProjection::updateViewMatrix()
{
	recalculateViewMatrix = false;
	D3DXMatrixLookAtLH(&viewMatrix, &position, &lookAt, &upVector);
}

void CameraProjection::updateProjectionMatrix()
{
	recalculateProjectionMatrix = false;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, fov, Window::GetWidth() / Window::GetHeight(), nearClipPlane, farClipPlane);
}