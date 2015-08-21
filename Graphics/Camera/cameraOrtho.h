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

protected:
	void updateViewMatrix();
	void updateProjectionMatrix();
};