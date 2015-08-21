#include "phantomSphereSimple.h"

#ifndef NO_HAVOK_PHYSICS
PhantomSphereSimple::PhantomSphereSimple()
{
	phantom = null;
	radius = 1;
	shape = null;
	transform.setIdentity();
}

void PhantomSphereSimple::Initialize()
{
	shape = new hkpSphereShape(radius);
	PhantomShapeSimple::Initialize();
}

void PhantomSphereSimple::Dispose()
{
	PhantomShapeSimple::Dispose();
	shape->removeReference();
	shape = null;
}

void PhantomSphereSimple::SetShape(hkpShape* s)
{
}

void PhantomSphereSimple::SetRadius(float r)
{
	radius = r;
	if (shape)
	{
		((hkpSphereShape*)shape)->setRadius(r);
		phantom->setShape(shape);
	}
}
#endif