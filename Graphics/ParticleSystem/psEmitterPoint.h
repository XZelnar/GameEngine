#pragma once

#include "psEmitter.h"

class psEmitterPoint : public psEmitter
{
private:
	virtual void EmitParticle(Particle& p);
};