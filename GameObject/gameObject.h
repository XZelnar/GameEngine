#pragma once

#include "../stdafx.h"
#ifndef NO_HAVOK_PHYSICS
#include <Physics2012/Dynamics/Collide/ContactListener/hkpContactPointEvent.h>
#endif

class ComponentCollider;
class ComponentRigidBody;
class ComponentContactListener;
class ComponentRenderer;
class ComponentParticleSystem;
class Transformation;

class GameObject
{
	friend class ComponentRigidBody;
	friend class Level;

public:
	GameObject();
	virtual void Initialize();
	virtual void Dispose();
	
	virtual void PreInput() { }
	virtual void PostInput() { }
	virtual void PreUpdate() { }
	virtual void Update();
	virtual void PostUpdate() { }
	virtual void PrePhysicsStep() { }
	virtual void PostPhysicsStep();
	virtual void PreRender() { }
	virtual void Render();
	virtual void PostRender() { }
	virtual void PreGUI() { }
	virtual void GUI();
	virtual void PostGUI() { }

	virtual void OnResolutionChanged(int neww, int newh, int oldw, int oldh) { }
	
	virtual void SetPosition(float x, float y, float z);
	virtual void SetPosition(D3DXVECTOR3 pos);
	virtual void SetRotation(D3DXVECTOR3 rot);
	virtual void Move(D3DXVECTOR3 delta);
	virtual void AddLinearVelocity(D3DXVECTOR3 delta);
	virtual void AddRotation(float x, float y, float z);
	virtual void AddRotation(D3DXVECTOR3& rot);
	virtual void SetScale(D3DXVECTOR3 scale);
	virtual void SetLinearVelocity(D3DXVECTOR3 vel);

	virtual void OnCollisionDetected(const hkpContactPointEvent& collision) { }
	
	inline GameObject* GetParent() { return parent; }
	inline Transformation* GetTransformation() { return transformation; }
	inline ComponentCollider* GetCollider() { return collider; }
	inline ComponentRigidBody* GetRigidBody() { return rigidBody; }
	inline ComponentContactListener* GetContactListener() { return contactListener; }
	inline ComponentRenderer* GetRenderer() { return renderer; }
	inline ComponentParticleSystem* GetParticleSystem() { return particleSystem; }

	bool HasParent() { return parent; }
	bool HasCollider() { return collider; }
	bool HasRigidBody() { return rigidBody; }
	bool HasContactListener() { return contactListener; }
	bool HasRenderer() { return renderer; }
	bool HasParticleSystem() { return particleSystem; }

	void SetParent(GameObject* _parent);
	void SetCollider(ComponentCollider* _collider);
	void SetRigidBody(ComponentRigidBody* _rigidBody);
	void SetContactListener(ComponentContactListener* _contactListener);
	void SetRenderer(ComponentRenderer* _renderer);
	void SetParticleSystem(ComponentParticleSystem* _particleSystem);

	void AddChild(GameObject* o);
	void RemoveChild(GameObject* o);
	
protected:
	Transformation* transformation;
	ComponentCollider* collider;
	ComponentRigidBody* rigidBody;
	ComponentContactListener* contactListener;
	ComponentRenderer* renderer;
	ComponentParticleSystem* particleSystem;
	unsigned int ID, loadedID;
	GameObject* parent;
	vector<GameObject*> children;

protected://static
	static unsigned int CurID;
};