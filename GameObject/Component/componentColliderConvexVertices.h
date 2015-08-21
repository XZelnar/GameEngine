#pragma once

#include "componentCollider.h"

class Mesh;

#ifndef NO_HAVOK_PHYSICS
#include <Common/Base/hkBase.h>
#include <Physics2012/Collide/Shape/hkpShape.h>
#include <Physics2012/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesShape.h>
#include <Physics2012/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>
#include <Common/Base/Types/Geometry/hkStridedVertices.h>

class ComponentColliderConvexVertices : public ComponentCollider
{
public:
	ComponentColliderConvexVertices(Mesh* m);

public:
	virtual void Initialize(GameObject* _parent);
	virtual void Dispose();

	virtual hkpShape* GetCollider() { return collider; }
	virtual hkMassProperties& GetMassProperties(hkReal mass);

private:
	hkMassProperties r;
	Mesh* mesh;
	hkStridedVertices vertices;
	hkpConvexVerticesShape* collider;
};

#else

class ComponentColliderConvexVertices : public ComponentCollider
{
public:
	ComponentColliderConvexVertices(Mesh* m) { }

public:
	virtual void Initialize(GameObject* _parent) { parent = _parent; }
	virtual void Dispose() { }

	virtual hkpShape* GetCollider() { return null; }
	virtual hkMassProperties& GetMassProperties(hkReal mass) { return t; }

private:
	int t;
};

#endif