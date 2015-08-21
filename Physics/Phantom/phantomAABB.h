#pragma once

#include "../../stdafx.h"

#ifndef NO_HAVOK_PHYSICS
#include <Common/Base/hkBase.h>
#include <Physics2012/Dynamics/Phantom/hkpAabbPhantom.h>


class PhantomAABB
{
public:
	PhantomAABB();
	void Initialize();
	void Dispose();

	void SetAABB(hkVector4& posMin, hkVector4& posMax);
	void Move(hkVector4& delta);
	const hkArray<hkpCollidable*, hkContainerHeapAllocator>& GetOverlappingCollidables();

private:
	hkpAabbPhantom* phantom;
	hkAabb aabb;
};

#else

class PhantomAABB
{
public:
	PhantomAABB() { }
	void Initialize() { }
	void Dispose() { }

	void SetAABB(hkVector4& posMin, hkVector4& posMax) { }
	void Move(hkVector4& delta) { }
	//const hkArray<hkpCollidable*, hkContainerHeapAllocator>& GetOverlappingCollidables() { }
};

#endif