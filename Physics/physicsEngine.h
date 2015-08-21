#pragma once

#include "../stdafx.h"

#ifndef NO_HAVOK_PHYSICS
#include <Common/Visualize/hkVisualDebugger.h>
#include <Physics2012/Utilities/VisualDebugger/hkpPhysicsContext.h>
#endif

class PhysicsEngine
{
#ifndef NO_HAVOK_PHYSICS
public:
	static void Initialize();
	static void Dispose();
	static void SimulationStep(float deltaTime);
	static hkpAllRayHitCollector RayCast(hkVector4& from, hkVector4& to);

private:
	static hkMallocAllocator baseMalloc;
	static hkpWorld* world;
	
public:
	static hkVisualDebugger* _vdb;
	static hkpPhysicsContext* _physicsContext;

public:
	static hkpWorld* GetWorld() { return world; }
	static void AddEntity(hkpEntity* entity) { world->addEntity(entity); }
	static void RemoveEntity(hkpEntity* entity) { world->removeEntity(entity); }
#else
public:
	static void Initialize() { }
	static void Dispose() { }
	static void SimulationStep(float deltaTime) { }
	static hkpAllRayHitCollector RayCast(hkVector4& from, hkVector4& to) { }

	static hkpWorld* GetWorld() { return null; }
	static void AddEntity(hkpEntity* entity) { }
	static void RemoveEntity(hkpEntity* entity) { }
#endif
};