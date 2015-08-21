#include "physicsEngine.h"

#ifndef NO_HAVOK_PHYSICS
#include <Common/Base/Memory/System/Util/hkMemoryInitUtil.h>
#include <Physics2012/Collide/Dispatch/hkpAgentRegisterUtil.h>

#include <Physics2012\Collide\BroadPhase\hkpBroadPhase.h>

hkpWorld* PhysicsEngine::world;
hkMallocAllocator PhysicsEngine::baseMalloc;

hkVisualDebugger* PhysicsEngine::_vdb;
hkpPhysicsContext* PhysicsEngine::_physicsContext;

static void HK_CALL errorReport(const char* msg, void* userContext)
{
	//std::cerr << msg << std::endl;
}

void PhysicsEngine::Initialize()
{
	hkMemoryRouter* memoryRouter = hkMemoryInitUtil::initChecking(&baseMalloc, hkMemorySystem::FrameInfo(1024 * 1024));
	hkBaseSystem::init(memoryRouter, errorReport);

	hkpWorldCinfo worldInfo;
	worldInfo.m_gravity = hkVector4(0, -29.8f, 0);

	worldInfo.m_fireCollisionCallbacks = true;
	worldInfo.m_broadPhaseBorderBehaviour = hkpWorldCinfo::BROADPHASE_BORDER_FIX_ENTITY;
	//worldInfo.setBroadPhaseWorldSize(1000);

	world = new hkpWorld(worldInfo);

	hkpAgentRegisterUtil::registerAllAgents(world->getCollisionDispatcher());

	_physicsContext = new hkpPhysicsContext();
	hkpPhysicsContext::registerAllPhysicsProcesses(); // all the physics viewers
	_physicsContext->addWorld(world); // add the physics world so the viewers can see it
	hkArray<hkProcessContext*> contexts;
	contexts.pushBack(_physicsContext);
	_vdb = new hkVisualDebugger(contexts);
	_vdb->serve();
}

void PhysicsEngine::Dispose()
{
	_vdb->removeReference();
	_physicsContext->removeReference();

	world->removeReference();

	hkBaseSystem::quit();
	hkMemoryInitUtil::quit();

	delete world;
	delete _vdb;
}

void PhysicsEngine::SimulationStep(float deltaTime)
{
	world->stepDeltaTime(deltaTime);

	_vdb->step();
}

hkpAllRayHitCollector PhysicsEngine::RayCast(hkVector4& from, hkVector4& to)
{
	hkpAllRayHitCollector c;
	hkpWorldRayCastInput i;
	i.m_enableShapeCollectionFilter = false;
	i.m_filterInfo = 0;
	i.m_from = from;
	i.m_to = to;
	world->castRay(i, c);
	return c;
}



#include <Common/Base/keycode.cxx>

// This excludes libraries that are not going to be linked from the project configuration, even if the keycodes are
// present.
#undef HK_FEATURE_PRODUCT_AI
#undef HK_FEATURE_PRODUCT_ANIMATION
#undef HK_FEATURE_PRODUCT_CLOTH
#undef HK_FEATURE_PRODUCT_DESTRUCTION
#undef HK_FEATURE_PRODUCT_DESTRUCTION_2012
#undef HK_FEATURE_PRODUCT_BEHAVIOUR
#undef HK_FEATURE_PRODUCT_MILSIM
#undef HK_FEATURE_PRODUCT_PHYSICS

#define HK_EXCLUDE_LIBRARY_hkpVehicle
#define HK_EXCLUDE_LIBRARY_hkCompat
#define HK_EXCLUDE_LIBRARY_hkSceneData
#define HK_EXCLUDE_LIBRARY_hkcdCollide

//
// Common
//
#define HK_EXCLUDE_FEATURE_SerializeDeprecatedPre700
#define HK_EXCLUDE_FEATURE_RegisterVersionPatches 

//
// Physics
//
#define HK_EXCLUDE_FEATURE_hkpHeightField

#define HK_EXCLUDE_FEATURE_hkpAccurateInertiaTensorComputer

//#define HK_EXCLUDE_FEATURE_hkpUtilities
#define HK_EXCLUDE_FEATURE_hkpVehicle
#define HK_EXCLUDE_FEATURE_hkpCompressedMeshShape
#define HK_EXCLUDE_FEATURE_hkpConvexPieceMeshShape
#define HK_EXCLUDE_FEATURE_hkpExtendedMeshShape
#define HK_EXCLUDE_FEATURE_hkpMeshShape
#define HK_EXCLUDE_FEATURE_hkpSimpleMeshShape
#define HK_EXCLUDE_FEATURE_hkpPoweredChainData
#define HK_EXCLUDE_FEATURE_hkMonitorStream

#include <Common/Base/Config/hkProductFeatures.cxx>
#endif