#pragma once

#include "../../stdafx.h"

class Camera
{
	friend class CameraManager;

public:
	D3DXMATRIX GetViewMatrix();
	D3DXMATRIX GetProjectionMatrix();
	virtual D3DXVECTOR3 ScreenPointToWorldDirection(D3DXVECTOR2& point);
	virtual D3DXVECTOR3 ScreenPointToWorldDirection(const float& x, const float& y) { return ScreenPointToWorldDirection(D3DXVECTOR2(x, y)); }
	virtual void OnActivated() { };
	virtual void OnDeactivated() { };
	virtual void Update() { };

protected:
	virtual void updateViewMatrix() = 0;
	virtual void updateProjectionMatrix() = 0;
	virtual void OnResolutionChanged() { recalculateProjectionMatrix = true; }

protected:
	bool recalculateViewMatrix, recalculateProjectionMatrix;
	D3DXMATRIX viewMatrix;
	D3DXMATRIX projectionMatrix;
};