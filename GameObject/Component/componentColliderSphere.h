#pragma once

#include "../../stdafx.h"
#include "componentCollider.h"

class GameObject;
class ComponentRigidBody;

#ifndef NO_HAVOK_PHYSICS
#include <Common/Base/hkBase.h>
#include <Physics2012/Collide/Shape/hkpShape.h>
#include <Physics2012/Collide/Shape/Convex/Sphere/hkpSphereShape.h>

class ComponentColliderSphere : public ComponentCollider
{
public:
	ComponentColliderSphere();
	ComponentColliderSphere(hkReal& _radius);

public:
	virtual void Initialize(GameObject* _parent);
	virtual void Dispose();

	virtual hkpShape* GetCollider() { return collider; }
	virtual hkMassProperties& GetMassProperties(hkReal mass);

public:
	hkReal GetRadius() { return radius; }
	void SetRadius(hkReal _radius);

private:
	hkMassProperties r;
	hkpSphereShape* collider;
	hkReal radius, radiusUnscaled;
};

#else

class ComponentColliderSphere : public ComponentCollider
{
public:
	ComponentColliderSphere() { }
	ComponentColliderSphere(hkReal& _radius) { }

public:
	virtual void Initialize(GameObject* _parent) { }
	virtual void Dispose() { }

	virtual hkpShape* GetCollider() { return null; }
	virtual hkMassProperties& GetMassProperties(hkReal mass) { return _mass; }

public:
	hkReal GetRadius() { return 0; }
	void SetRadius(hkReal _radius) { }

private:
	hkMassProperties _mass;
};

#endif