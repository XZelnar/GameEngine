#pragma once

#include "camera.h"

class CameraOrtho : public Camera
{
public:
	CameraOrtho();
	virtual void OnActivated();
	virtual void OnResolutionChanged();
	virtual D3DXMATRIX GetViewMatrix();
	virtual D3DXMATRIX GetProjectionMatrix();
	virtual D3DXVECTOR3 GetPosition() { return D3DXVECTOR3(0, 0, 0); }

protected:
	void updateViewMatrix();
	void updateProjectionMatrix();
};