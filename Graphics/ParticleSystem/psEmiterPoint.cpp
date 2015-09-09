#include "psEmitterPoint.h"

void psEmitterPoint::EmitParticle(Particle& p)
{
	p.position.x = p.position.y = p.position.z = 0;

	static uniform_real<float> rand(-1, 1);
	static default_random_engine e_rand;

	p.direction.x = rand(e_rand);
	p.direction.y = rand(e_rand);
	p.direction.z = rand(e_rand);
	D3DXVec3Normalize(&p.direction, &p.direction);

	p.lifetime = 0;
}