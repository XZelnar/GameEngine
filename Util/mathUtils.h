#pragma once

#include "../stdafx.h"
#ifndef NO_HAVOK_PHYSICS
#include <Common/Base/hkBase.h>
#endif

class MathUtils
{
public:
	static void QuaternionToEuler(const hkQuaternion& quat, D3DXVECTOR3& euler);
	static hkQuaternion& EulerToQuaternion(const D3DXVECTOR3& euler, hkQuaternion& quat);
	/// returns out obect for chaining
	static D3DXVECTOR3& AnyPerpendicularVector(const D3DXVECTOR3& in, D3DXVECTOR3& out);
	/// returns out obect for chaining
	static D3DXVECTOR3& VectorCrossProduct(const D3DXVECTOR3& in1, const D3DXVECTOR3& in2, D3DXVECTOR3& out);
	/// returns out obect for chaining
	static D3DXVECTOR3& VectorToEuler(const D3DXVECTOR3& in, D3DXVECTOR3& out);
};