#include "phantomShapeSimple.h"

#ifndef NO_HAVOK_PHYSICS
#include "../physicsEngine.h"

PhantomShapeSimple::PhantomShapeSimple()
{
	phantom = null;
	shape = null;
	transform.setIdentity();
}

void PhantomShapeSimple::Initialize()
{
	phantom = new hkpSimpleShapePhantom(shape, transform);
	PhysicsEngine::GetWorld()->addPhantom(phantom);
}

void PhantomShapeSimple::Dispose()
{
	if (phantom)
	{
		PhysicsEngine::GetWorld()->removePhantom(phantom);
		phantom->removeReference();
		phantom = null;
	}
}



void PhantomShapeSimple::SetShape(hkpShape* s)
{
	shape = s;
	if (phantom)
		phantom->setShape(s);
}

void PhantomShapeSimple::SetTransform(hkTransform& t)
{
	transform = t;
	if (phantom)
		phantom->setTransform(t);
}

/// Delete reference once finished
hkpAllCdBodyPairCollector* PhantomShapeSimple::GetPenetrations()
{
	hkpAllCdBodyPairCollector* c = new hkpAllCdBodyPairCollector();
	phantom->getPenetrations(*c);
	return c;
}

/// Delete reference once finished
hkpAllCdPointCollector* PhantomShapeSimple::GetClosestPoints()
{
	hkpAllCdPointCollector* c = new hkpAllCdPointCollector();
	phantom->getClosestPoints(*c);
	return c;
}
#endif