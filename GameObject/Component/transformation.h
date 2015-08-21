#pragma once

#include "../../stdafx.h"

#ifndef NO_HAVOK_PHYSICS
#include <Common/Base/hkBase.h>
#endif

class Transformation
{
	friend class GameObject;
	friend class ComponentRigidBody;

public:
	Transformation(GameObject* _parent);

	D3DXVECTOR3 GetPosition();
	hkVector4 GetPositionHK() { return hkVector4(position.x, position.y, position.z); }
	D3DXVECTOR3 GetRotation();
	hkQuaternion GetRotationHK();
	D3DXVECTOR3 GetScale();
	D3DXMATRIX* GetTransformationMatrix();
	D3DXMATRIX* GetRotationMatrix();
	D3DXVECTOR3* LocalToGlobalDirection(D3DXVECTOR3* local);
#ifndef NO_HAVOK_PHYSICS
	void hkGetTransformation(hkTransform& transformation);
	void hkGetTransformedLocalPoint(D3DXVECTOR3& point, hkTransform& transformation);
#endif
	
	void SetPreMatrix(D3DXMATRIX m);
	void SetPostMatrix(D3DXMATRIX m);

	void setRotation(D3DXVECTOR3 _rotation);

private:
	void setPosition(D3DXVECTOR3 _position);
	void addRotation(D3DXVECTOR3 _rotation);
	void setScale(D3DXVECTOR3 _scale);
	void normalizeRotation();
	void setPositionHK(const hkVector4& _position);
	void setRotationHK(const hkQuaternion& _rotation);
	void recalculateMatrix();

private:
	GameObject* parent;
	D3DXVECTOR3 position;
	D3DXVECTOR3 rotation;
	D3DXVECTOR3 scale;
	D3DXMATRIX preMatrix, postMatrix;
	D3DXMATRIX mWorld, mPos, mRot, mRotX, mRotY, mRotZ, mScale;
	bool recalculate;
	bool parentHasRigidBody, parentHasCollider;
};