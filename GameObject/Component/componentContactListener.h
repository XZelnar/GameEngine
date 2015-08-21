#pragma once

#include "../../stdafx.h"

class GameObject;

#ifndef NO_HAVOK_PHYSICS
#include <Common/Base/hkBase.h>
#include <Physics2012/Dynamics/Entity/hkpRigidBody.h>
#include <Physics2012/Dynamics/Collide/ContactListener/hkpContactListener.h>
#include <Physics2012/Dynamics/Collide/ContactListener/hkpContactPointEvent.h>

class ComponentContactListener : public hkpContactListener
{
public:
	ComponentContactListener();
	void Initialize(GameObject* _parent);
	void Dispose();

	virtual void contactPointCallback(const hkpContactPointEvent& collision);
	virtual void collisionAddedCallback(const hkpCollisionEvent &event);
	virtual void collisionRemovedCallback(const hkpCollisionEvent &event);

private:
	GameObject* parent;
};

#else

class ComponentContactListener
{
public:
	ComponentContactListener();
	void Initialize(GameObject* _parent);
	void Dispose();

	virtual void contactPointCallback(const hkpContactPointEvent& collision);
	virtual void collisionAddedCallback(const hkpCollisionEvent &event);
	virtual void collisionRemovedCallback(const hkpCollisionEvent &event);

private:
	GameObject* parent;
};

#endif