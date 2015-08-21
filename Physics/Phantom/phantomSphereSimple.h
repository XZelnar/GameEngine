#pragma once

#include "../../stdafx.h"
#include "phantomShapeSimple.h"

#ifndef NO_HAVOK_PHYSICS
#include <Physics2012/Collide/Shape/Convex/Sphere/hkpSphereShape.h>
#endif

class PhantomSphereSimple : public PhantomShapeSimple
{
public:
	PhantomSphereSimple();
	virtual void Initialize();
	virtual void Dispose();
	virtual void SetShape(hkpShape* s);

	void SetRadius(float r);
	float GetRadius() { return radius; }
	
private:
	float radius;
};