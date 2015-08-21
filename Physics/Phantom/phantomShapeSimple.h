#pragma once

#include "../../stdafx.h"

#ifndef NO_HAVOK_PHYSICS
#include <Common/Base/hkBase.h>
#include <Physics2012/Dynamics/Phantom/hkpSimpleShapePhantom.h>
#include <Physics2012/Collide/Query/Collector/BodyPairCollector/hkpAllCdBodyPairCollector.h>
#include <Physics2012/Collide/Query/Collector/PointCollector/hkpAllCdPointCollector.h>

class PhantomShapeSimple
{
public:
	PhantomShapeSimple();
	virtual void Initialize() = 0;
	virtual void Dispose();

	virtual void SetShape(hkpShape* s);
	virtual void SetTransform(hkTransform& t);

	virtual hkpAllCdBodyPairCollector* GetPenetrations();
	virtual hkpAllCdPointCollector* GetClosestPoints();

	virtual const hkpSimpleShapePhantom* GetPhantom() { return phantom; }

protected:
	hkpSimpleShapePhantom* phantom;
	hkpShape* shape;
	hkTransform transform;
};

#else

class PhantomShapeSimple
{
public:
	PhantomShapeSimple() { }
	virtual void Initialize() = 0;
	virtual void Dispose() { }

	virtual void SetShape(hkpShape* s) { }
	virtual void SetTransform(hkTransform& t);

	virtual hkpAllCdBodyPairCollector* GetPenetrations() { }
	virtual hkpAllCdPointCollector* GetClosestPoints() { }

	virtual const hkpSimpleShapePhantom* GetPhantom() { return null; }
};

#endif