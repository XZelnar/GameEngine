#pragma once

#include "camera.h"

class CameraProjection : public Camera
{
public:
	CameraProjection();
	CameraProjection(float fov, float nearPlane, float farPlane);
	D3DXVECTOR3 GetPosition();
	void SetPosition(D3DXVECTOR3 pos, bool moveLookAt = false);
	void SetPosition(D3DXVECTOR3 pos, D3DXVECTOR3 _lookAt);
	void MoveCamera(D3DXVECTOR3 delta, bool moveLookAt = true);
	D3DXVECTOR3 GetLookAt();
	void SetLookAt(D3DXVECTOR3 _lookAt);
	void GetClipPlanes(float& near, float& far);
	void SetClipPlanes(float near, float far);
	float GetFOV();
	void SetFOV(float _fov);

protected:
	virtual void updateViewMatrix();
	virtual void updateProjectionMatrix();

protected:
	D3DXVECTOR3 position;
	D3DXVECTOR3 lookAt;
	D3DXVECTOR3 upVector;
	float nearClipPlane;
	float farClipPlane;
	float fov;
};