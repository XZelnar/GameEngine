#pragma once

#include "../../stdafx.h"

class Camera
{
	friend class CameraManager;

public:
	D3DXMATRIX GetViewMatrix();
	D3DXMATRIX GetProjectionMatrix();
	D3DXMATRIX GetViewProjectionMatrix();
	virtual D3DXVECTOR3 ScreenPointToWorldDirection(D3DXVECTOR2& point);
	virtual D3DXVECTOR3 ScreenPointToWorldDirection(const float& x, const float& y) { return ScreenPointToWorldDirection(D3DXVECTOR2(x, y)); }
	virtual void OnActivated() { };
	virtual void OnDeactivated() { };
	virtual void Update() { };
	virtual D3DXVECTOR3 GetPosition() = 0;

protected:
	virtual void updateViewMatrix() = 0;
	virtual void updateProjectionMatrix() = 0;
	virtual void updateViewProjectionMatrix() { viewProjectionMatrix = viewMatrix * projectionMatrix; }
	virtual void OnResolutionChanged() { recalculateProjectionMatrix = true; }

protected:
	bool recalculateViewMatrix, recalculateProjectionMatrix;
	D3DXVECTOR3 position;
	D3DXMATRIX viewMatrix;
	D3DXMATRIX projectionMatrix;
	D3DXMATRIX viewProjectionMatrix;
};