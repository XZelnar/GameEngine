#include "componentColliderBox.h"

#ifndef NO_HAVOK_PHYSICS
#include "componentRigidBody.h"
#include "../gameObject.h"
#include "transformation.h"

ComponentColliderBox::ComponentColliderBox() : ComponentCollider()
{
	size = hkVector4(1, 1, 1);
	sizeUnscaled = size;
	collider = null;
}

ComponentColliderBox::ComponentColliderBox(hkVector4& _size) : ComponentCollider()
{
	size = hkVector4(_size);
	size.div4(hkVector4(2, 2, 2, 2));
	sizeUnscaled = size;
	collider = null;
}

void ComponentColliderBox::Initialize(GameObject* _parent)
{
	parent = _parent;
	D3DXVECTOR3 s = parent->GetTransformation()->GetScale();
	double x = sizeUnscaled.getComponent(0);
	x *= s.x;
	double y = sizeUnscaled.getComponent(1);
	y *= s.y;
	double z = sizeUnscaled.getComponent(2);
	z *= s.z;
	size = hkVector4(x, y, z);
	collider = new hkpBoxShape(size);
}

void ComponentColliderBox::Dispose()
{
	if (collider)
	{
		collider->removeReference();
		delete collider;
	}
}

void ComponentColliderBox::SetSize(hkVector4& _size)
{
	sizeUnscaled = _size;
	D3DXVECTOR3 s = parent->GetTransformation()->GetScale();
	double x = sizeUnscaled.getComponent(0);
	x *= s.x;
	double y = sizeUnscaled.getComponent(1);
	y *= s.y;
	double z = sizeUnscaled.getComponent(2);
	z *= s.z;
	size = hkVector4(x, y, z);
	collider->setHalfExtents(size);
	IGameObjectComponent* rb = parent->GetRigidBody();
	if (rb)
		((ComponentRigidBody*)rb)->OnSizeChanged();
}

hkMassProperties& ComponentColliderBox::GetMassProperties(hkReal mass)
{
	hkInertiaTensorComputer::computeBoxVolumeMassProperties(size, mass, r);
	return r;
}
#endif