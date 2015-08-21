#include "phantomAABB.h"

#ifndef NO_HAVOK_PHYSICS
#include "../physicsEngine.h"

PhantomAABB::PhantomAABB()
{
	aabb.m_min = hkVector4(0, 0, 0);
	aabb.m_max = hkVector4(0, 0, 0);
	phantom = null;
}

void PhantomAABB::Initialize()
{
	phantom = new hkpAabbPhantom(aabb);

	PhysicsEngine::GetWorld()->addPhantom(phantom);
}

void PhantomAABB::Dispose()
{
	if (phantom)
	{
		PhysicsEngine::GetWorld()->removePhantom(phantom);
		phantom->removeReference();
		delete phantom;
	}
}

void PhantomAABB::SetAABB(hkVector4& posMin, hkVector4& posMax)
{
	aabb.m_min = posMin;
	aabb.m_max = posMax;
	if (phantom)
		phantom->setAabb(aabb);
}

void PhantomAABB::Move(hkVector4& delta)
{
	aabb.m_min = hkVector4(aabb.m_min(0) + delta(0), aabb.m_min(1) + delta(1), aabb.m_min(2) + delta(2));
	aabb.m_max = hkVector4(aabb.m_max(0) + delta(0), aabb.m_max(1) + delta(1), aabb.m_max(2) + delta(2));
	if (phantom)
		phantom->setAabb(aabb);
}

const hkArray<hkpCollidable*, hkContainerHeapAllocator>& PhantomAABB::GetOverlappingCollidables()
{
	return phantom->getOverlappingCollidables();
}
#endif