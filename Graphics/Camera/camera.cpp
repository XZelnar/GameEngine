#include "camera.h"

#include "../../window.h"

D3DXMATRIX Camera::GetViewMatrix()
{
	if (recalculateViewMatrix)
		updateViewMatrix();
	return viewMatrix;
}

D3DXMATRIX Camera::GetProjectionMatrix()
{
	if (recalculateProjectionMatrix)
		updateProjectionMatrix();
	return projectionMatrix;
}

D3DXMATRIX Camera::GetViewProjectionMatrix()
{
	if (recalculateViewMatrix)
		updateViewMatrix();
	if (recalculateProjectionMatrix)
		updateProjectionMatrix();
	return viewProjectionMatrix;
}

D3DXVECTOR3 Camera::ScreenPointToWorldDirection(D3DXVECTOR2& point)
{
	if (recalculateProjectionMatrix)
		updateProjectionMatrix();
	if (recalculateViewMatrix)
		updateViewMatrix();

	float px = point.x, py = point.y;
	D3DXMATRIX proj = GetProjectionMatrix();

	px = 2.0f * px / Window::GetWidth() - 1.0f;
	py = -2.0f * py / Window::GetHeight() + 1.0f;
	px /= proj._11;//proj matrix actually
	py /= proj._22;
	D3DXMatrixInverse(&proj, null, &GetViewMatrix());

	D3DXVECTOR3 dir;
	dir.x = (px * proj._11) + (py * proj._21) + proj._31;
	dir.y = (px * proj._12) + (py * proj._22) + proj._32;
	dir.z = (px * proj._13) + (py * proj._23) + proj._33;
	D3DXVec3Normalize(&dir, &dir);
	return dir;
}