#pragma once

#include "psEmitter.h"

class psEmitterCircle : public psEmitter
{
public:
	psEmitterCircle(D3DXVECTOR3 _normal, float _radius);

private:
	virtual void EmitParticle(Particle& p);

private:
	D3DXVECTOR3 normal, v1, v2;
	float radius;
};