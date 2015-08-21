#include "phantomBoxSimple.h"

#ifndef NO_HAVOK_PHYSICS
PhantomBoxSimple::PhantomBoxSimple()
{
	phantom = null;
	halfExtents = hkVector4(1, 1, 1);
	shape = null;
	transform.setIdentity();
}

void PhantomBoxSimple::Initialize()
{
	shape = new hkpBoxShape(halfExtents);
	PhantomShapeSimple::Initialize();
}

void PhantomBoxSimple::Dispose()
{
	PhantomShapeSimple::Dispose();
	shape->removeReference();
	shape = null;
}

void PhantomBoxSimple::SetShape(hkpShape* s)
{
}

void PhantomBoxSimple::SetHalfExtents(const hkVector4& h)
{
	halfExtents = h;
	if (shape)
	{
		((hkpBoxShape*)shape)->setHalfExtents(halfExtents);
		phantom->setShape(shape);
	}
}
#endif