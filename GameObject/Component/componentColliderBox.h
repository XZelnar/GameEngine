#pragma once

#include "../../stdafx.h"
#include "componentCollider.h"

class GameObject;
class ComponentRigidBody;

#ifndef NO_HAVOK_PHYSICS
#include <Common/Base/hkBase.h>
#include <Physics2012/Collide/Shape/hkpShape.h>
#include <Physics2012/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics2012/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>

class ComponentColliderBox : public ComponentCollider
{
public:
	ComponentColliderBox();
	ComponentColliderBox(hkVector4& _size);

public:
	virtual void Initialize(GameObject* _parent);
	virtual void Dispose();

	virtual hkpShape* GetCollider() { return collider; }
	virtual hkMassProperties& GetMassProperties(hkReal mass);

public:
	hkVector4 GetSize() { return size; }
	void SetSize(hkVector4& _size);

private:
	hkMassProperties r;
	hkVector4 size, sizeUnscaled;
	hkpBoxShape* collider;
};

#else

class ComponentColliderBox : public ComponentCollider
{
public:
	ComponentColliderBox() { }
	ComponentColliderBox(hkVector4& _size) { }

public:
	virtual void Initialize(GameObject* _parent) { }
	virtual void Dispose() { }

	virtual hkpShape* GetCollider() { return null; }
	virtual hkMassProperties& GetMassProperties(hkReal mass) { return _mass; }

public:
	hkVector4 GetSize() { return hkVector4(); }
	void SetSize(hkVector4& _size) { }

private:
	int _mass;
};

#endif