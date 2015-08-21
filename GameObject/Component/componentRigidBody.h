#pragma once

#include "../../stdafx.h"
#include "iComponent.h"

class GameObject;
class ComponentCollider;

#ifndef NO_HAVOK_PHYSICS
#include <Common/Base/hkBase.h>
#include <Physics2012/Dynamics/Entity/hkpRigidBody.h>

class ComponentRigidBody : public IGameObjectComponent
{
public:
	ComponentRigidBody();
	ComponentRigidBody(hkReal _mass);

public:
	virtual void Initialize(GameObject* parent);
	virtual void Dispose();
	virtual void Update() { }
	virtual void Render();

public:
	void AddObjectToWorld();
	void RemoveObjectFromWorld();

	hkReal GetMass() { return mass; }
	bool IsStatic() { return isStatic; }
	void AddContactListener(hkpContactListener* l);
	void RemoveContactListener(hkpContactListener* l);
	hkVector4 GetPosition();
	const hkpRigidBody* GetRigidBody();
	void SetPosition(const hkVector4& pos);
	void SetRotation(hkQuaternion& rot);
	void SetMass(hkReal _mass);
	void SetRestitution(float res);
	void SetFriction(float fr);
	void SetLinearDamping(float d);
	void SetLinearVelocity(const hkVector4& vel);
	void SetGravityFactor(float f);
	void SetIsStatic(bool _isStatic);
	void AddLinearVelocity(float x, float y, float z);
	void OnSizeChanged();
	void UpdateGameObjectTransformation();

public:
	bool forceGameObjectRotation;
	bool SyncRotation;

protected:
	bool isInWorld;
	bool isStatic;
	hkReal mass;
	float restitution, friction, linearDamping, gravityFactor;
	hkpRigidBody* rigidBody;
	hkpShape* shape;
};

#else

class ComponentRigidBody : public IGameObjectComponent
{
public:
	ComponentRigidBody() { }
	ComponentRigidBody(hkReal _mass) { }

public:
	void AddObjectToWorld() { }
	void RemoveObjectFromWorld() { }

	virtual void Initialize(GameObject* parent) { this->parent = parent; }
	virtual void Dispose() { }
	virtual void Update() { }
	virtual void Render() { }

	hkReal GetMass() { return 0; }
	bool IsStatic() { return true; }
	void AddContactListener(hkpContactListener* l) { }
	void RemoveContactListener(hkpContactListener* l) { }
	hkVector4 GetPosition() { }
	const hkpRigidBody* GetRigidBody() { }
	void SetPosition(const hkVector4& pos) { }
	void SetRotation(hkQuaternion& rot) { }
	void SetMass(hkReal _mass) { }
	void SetRestitution(float res) { }
	void SetFriction(float fr) { }
	void SetLinearDamping(float d) { }
	void SetLinearVelocity(const hkVector4& vel) { }
	void SetGravityFactor(float f) { }
	void SetIsStatic(bool _isStatic) { }
	void AddLinearVelocity(float x, float y, float z) { }
	void OnSizeChanged() { }
	void UpdateGameObjectTransformation() { }

public:
	bool forceGameObjectRotation;
	bool SyncRotation;
};

#endif