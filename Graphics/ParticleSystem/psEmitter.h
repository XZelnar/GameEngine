#pragma once

#include "../../stdafx.h"
#include "particle.h"

class psEmitter
{
	friend class ComponentParticleSystem;

public:
	psEmitter() { emitFromShell = false; randomDirection = false; inverseDirection = false; }
	void __DoNotDelete() { }

protected:
	virtual void EmitParticle(Particle& p) = 0;

public:
	bool emitFromShell;
	bool randomDirection;
	bool inverseDirection;
};