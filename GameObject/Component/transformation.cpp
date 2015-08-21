#include "transformation.h"

#include "../../Util/mathUtils.h"
#include "../gameObject.h"

Transformation::Transformation(GameObject* _parent)
{
	parent = _parent;
	position = D3DXVECTOR3(0, 0, 0);
	rotation = D3DXVECTOR3(0, 0, 0);
	scale = D3DXVECTOR3(1, 1, 1);
	//positionOffset = D3DXVECTOR3(0, 0, 0);
	D3DXMatrixIdentity(&preMatrix);
	D3DXMatrixIdentity(&postMatrix);
	recalculate = false;
	parentHasCollider = false;
	parentHasRigidBody = false;
}

D3DXVECTOR3 Transformation::GetPosition() 
{ 
	return position; 
}

D3DXVECTOR3 Transformation::GetRotation() 
{ 
	return rotation; 
}

hkQuaternion Transformation::GetRotationHK()
{
	static hkQuaternion quat;
	return MathUtils::EulerToQuaternion(rotation, quat);
}

D3DXVECTOR3 Transformation::GetScale() 
{ 
	return scale; 
}

D3DXMATRIX* Transformation::GetTransformationMatrix()
{
	if (recalculate)
		recalculateMatrix();
	if (parent->HasParent())
	{
		static D3DXMATRIX t;
		t = *parent->GetParent()->GetTransformation()->GetRotationMatrix();
		D3DXMatrixInverse(&t, null, &t);
		t = mWorld * *parent->GetParent()->GetTransformation()->GetTransformationMatrix();
		return &t;
	}
	else
		return &mWorld;
}

D3DXMATRIX* Transformation::GetRotationMatrix()
{
	if (recalculate)
		recalculateMatrix();
	return &mRot;
}

/// RETURNS SAME POINTER
D3DXVECTOR3* Transformation::LocalToGlobalDirection(D3DXVECTOR3* local)
{
	static D3DXMATRIX tpLocal;
	D3DXMatrixTranslation(&tpLocal, local->x, local->y, local->z);
	if (recalculate)
		recalculateMatrix();
	tpLocal = tpLocal * mRotX * mRotY * mRotZ;
	local->x = tpLocal._41;
	local->y = tpLocal._42;
	local->z = tpLocal._43;
	return local;
}

#ifndef NO_HAVOK_PHYSICS
void Transformation::hkGetTransformation(hkTransform& transformation)
{
	transformation.setIdentity();
	if (recalculate)
		recalculateMatrix();
	transformation.setTranslation(hkVector4(mWorld._41, mWorld._42, mWorld._43));
}

void Transformation::hkGetTransformedLocalPoint(D3DXVECTOR3& point, hkTransform& transformation)
{
	transformation.setIdentity();
	static D3DXMATRIX tpLocal2;
	D3DXMatrixTranslation(&tpLocal2, point.x, point.y, point.z);
	if (recalculate)
		recalculateMatrix();
	tpLocal2 = tpLocal2 * mWorld;
	transformation.setTranslation(hkVector4(tpLocal2._41, tpLocal2._42, tpLocal2._43));
}
#endif
	
void Transformation::SetPreMatrix(D3DXMATRIX m)
{
	preMatrix = m;
	recalculate = true;
}

void Transformation::SetPostMatrix(D3DXMATRIX m)
{
	postMatrix = m;
	recalculate = true;
}

//======================================================================================================================================
//======================================================================================================================================
//======================================================================================================================================

void Transformation::setPosition(D3DXVECTOR3 _position) 
{ 
	position = D3DXVECTOR3(_position);
	recalculate = true;
}

void Transformation::addRotation(D3DXVECTOR3 _rotation)
{
	rotation += _rotation;
	normalizeRotation();
	recalculate = true;
}

void Transformation::setRotation(D3DXVECTOR3 _rotation) 
{ 
	rotation = D3DXVECTOR3(_rotation);
	normalizeRotation();
	recalculate = true;
}

void Transformation::normalizeRotation()
{
	while (rotation.x < -D3DX_PI)
		rotation.x += 2 * D3DX_PI;
	while (rotation.x > D3DX_PI)
		rotation.x -= 2 * D3DX_PI;
	
	while (rotation.y < -D3DX_PI)
		rotation.y += 2 * D3DX_PI;
	while (rotation.y > D3DX_PI)
		rotation.y -= 2 * D3DX_PI;
	
	while (rotation.z < -D3DX_PI)
		rotation.z += 2 * D3DX_PI;
	while (rotation.z > D3DX_PI)
		rotation.z -= 2 * D3DX_PI;
}

void Transformation::setScale(D3DXVECTOR3 _scale) 
{ 
	scale = _scale;
	recalculate = true;
}

void Transformation::setPositionHK(const hkVector4& _position) 
{ 
	position = D3DXVECTOR3(_position.getComponent(0), _position.getComponent(1), _position.getComponent(2));
	recalculate = true;
}

void Transformation::setRotationHK(const hkQuaternion& _rotation) 
{
	MathUtils::QuaternionToEuler(hkQuaternion(_rotation), rotation);
	recalculate = true;
}

void Transformation::recalculateMatrix()
{
	recalculate = false;
	D3DXMatrixTranslation(&mPos, position.x, position.y, position.z);
	D3DXMatrixRotationX(&mRotX, rotation.x);
	D3DXMatrixRotationY(&mRotY, rotation.y);
	D3DXMatrixRotationZ(&mRotZ, rotation.z);
	D3DXMatrixScaling(&mScale, scale.x, scale.y, scale.z);
	mRot = mRotX * mRotY * mRotZ * preMatrix;
	mWorld = mScale * mRotX * mRotY * mRotZ * preMatrix * postMatrix * mPos;
}