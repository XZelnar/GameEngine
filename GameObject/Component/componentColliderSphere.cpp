#include "componentColliderSphere.h"

#ifndef NO_HAVOK_PHYSICS
#include "componentRigidBody.h"
#include "../gameObject.h"
#include "transformation.h"

ComponentColliderSphere::ComponentColliderSphere() : ComponentCollider()
{
	radius = 1;
	radiusUnscaled = radius;
}

ComponentColliderSphere::ComponentColliderSphere(hkReal& _radius) : ComponentCollider()
{
	radius = _radius;
	radiusUnscaled = radius;
}

void ComponentColliderSphere::Initialize(GameObject* _parent)
{
	parent = _parent;
	D3DXVECTOR3 s = parent->GetTransformation()->GetScale();
	radius *= pow(s.x * s.y * s.z, 1.0f / 3.0f);
	collider = new hkpSphereShape(radius);
}

void ComponentColliderSphere::Dispose()
{
	if (collider)
	{
		collider->removeReference();
		delete collider;
	}
}

void ComponentColliderSphere::SetRadius(hkReal _radius)
{
	radiusUnscaled = _radius;
	radius = _radius;
	D3DXVECTOR3 s = parent->GetTransformation()->GetScale();
	radius *= pow(s.x * s.y * s.z, 1.0f / 3.0f);
	collider->setRadius(radius);
	IGameObjectComponent* rb = parent->GetRigidBody();
	if (rb)
		((ComponentRigidBody*)rb)->OnSizeChanged();
}

hkMassProperties& ComponentColliderSphere::GetMassProperties(hkReal mass)
{
	hkInertiaTensorComputer::computeSphereVolumeMassProperties(radius, mass, r);
	return r;
}
#endif