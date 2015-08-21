#pragma once

#include "../../stdafx.h"
#include "phantomShapeSimple.h"

#ifndef NO_HAVOK_PHYSICS
#include <Physics2012/Collide/Shape/Convex/Box/hkpBoxShape.h>
#endif

class PhantomBoxSimple : public PhantomShapeSimple
{
public:
	PhantomBoxSimple();
	virtual void Initialize();
	virtual void Dispose();
	virtual void SetShape(hkpShape* s);

	void SetHalfExtents(const hkVector4& h);
	const hkVector4& GetHalfExtents() { return halfExtents; }

private:
	hkVector4 halfExtents;
};