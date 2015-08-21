#pragma once

#include "../../stdafx.h"
#include "iComponent.h"
#ifndef NO_HAVOK_PHYSICS
#include <Common/Base/hkBase.h>
#include <Physics2012/Collide/Shape/hkpShape.h>
#include <Physics2012/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>
#endif

class ComponentCollider : public IGameObjectComponent
{
public:
	ComponentCollider() : IGameObjectComponent() { }
	virtual void Update();
	virtual void Render();

	virtual hkpShape* GetCollider() = 0;
	virtual hkMassProperties& GetMassProperties(hkReal mass) = 0;
};