#define NO_HAVOK_PHYSICS

#pragma once

#include <Windows.h>
#include <WindowsX.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcommon.h>
#include <d3dx10math.h>
#include <dinput.h>
#include <dinputd.h>
#include <iostream>
#include <fstream>
#include <MMSystem.h>
#include <map>
#include <vector>
#include <stack>
#include <utility>
#include <algorithm>
#include <random>
#include <string.h>
#include <stdlib.h>
#include <functional>
#include <assert.h>
#include <ctime>
#include <DXGI.h>
#include <D3D11Shader.h>
#include <D3Dcompiler.h>

#include "Util/mathInline.h"
#include "Util/converter.h"
#include "engineValue.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "dxgi.lib")



#ifndef NO_HAVOK_PHYSICS
#pragma comment(lib, "hkImageUtilities.lib")
#include <Common/Base/hkBase.h>
#include <Physics2012/Dynamics/World/hkpWorld.h>
#include <Physics2012/Collide/Dispatch/hkpAgentRegisterUtil.h>
#include <Common/Base/Memory/Allocator/Malloc/hkMallocAllocator.h>
#include <Physics2012/Collide/Query/Collector/RayCollector/hkpAllRayHitCollector.h>
#include <Physics2012/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#endif



#define null NULL

#ifdef NO_HAVOK_PHYSICS

#pragma message ("Warning: Engine is compiled with NO_HAVOK_PHYSICS flag. Some parts of the engine will be disabled.")

#define hkReal float
#define hkpShape int
#define hkMassProperties int
#define hkpBoxShape int
#define hkpSphereShape int
#define hkpConvexVerticesShape int
#define hkpContactPointEvent int
#define hkpCollisionEvent int
#define hkpContactListener int
#define hkpRigidBody int
#define hkpAllCdBodyPairCollector int
#define hkpAllCdPointCollector int
#define hkpSimpleShapePhantom int
#define hkpAllRayHitCollector int
#define hkpWorld int
#define hkpEntity int

struct hkVector4
{
	float x, y, z, w;

	hkVector4() { }
	hkVector4(float, float, float) { }
	hkVector4(float, float, float, float) { }

	float getComponent(int) const { return 0; }
};

struct hkQuaternion
{
	hkVector4 m_vec;
	void set(float, float, float, float) { }
};

struct hkTransform
{

};
#endif