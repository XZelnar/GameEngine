#include "componentContactListener.h"

#include "../gameObject.h"

ComponentContactListener::ComponentContactListener()
{
}

void ComponentContactListener::Initialize(GameObject* _parent)
{
	parent = _parent;
}

void ComponentContactListener::Dispose()
{
}

void ComponentContactListener::contactPointCallback(const hkpContactPointEvent& collision)
{
	parent->OnCollisionDetected(collision);
}

void ComponentContactListener::collisionAddedCallback(const hkpCollisionEvent &collision)
{
	//hkpContactListener::registerForEndOfStepContactPointCallbacks(collision);
}

void ComponentContactListener::collisionRemovedCallback(const hkpCollisionEvent &collision)
{
	//hkpContactListener::unregisterForEndOfStepContactPointCallbacks(collision);
}