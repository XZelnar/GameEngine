#include "psEmitterCircle.h"

psEmitterCircle::psEmitterCircle(D3DXVECTOR3 _normal, float _radius)
{
	normal = _normal;
	radius = _radius;

	D3DXVECTOR3 t;
	if (normal.x != 0)
		t = D3DXVECTOR3(0, 1, 1);
	else
		t = D3DXVECTOR3(1, 0, 0);
	D3DXVec3Cross(&v1, &normal, &t);
	D3DXVec3Normalize(&v1, &v1);
	D3DXVec3Cross(&v2, &normal, &v1);
	D3DXVec3Normalize(&v2, &v2);
}

void psEmitterCircle::EmitParticle(Particle& p)
{
	static uniform_real<float> rand(-D3DX_PI, D3DX_PI);
	static uniform_real<float> rand2(0, 1);
	static uniform_real<float> rand3(-1, 1);
	static default_random_engine e_rand;

	static float angle, r;
	angle = rand(e_rand);
	if (emitFromShell)
		r = radius;
	else
		r = rand2(e_rand) * radius;

	static float s, c;
	s = sin(angle);
	c = cos(angle);

	p.position = (v1 * s + v2 * c) * r;

	if (randomDirection)
	{
		p.direction.x = rand3(e_rand);
		p.direction.y = rand3(e_rand);
		p.direction.z = rand3(e_rand);
		D3DXVec3Normalize(&p.direction, &p.direction);
	}
	else
	{
		D3DXVec3Normalize(&p.direction, &p.position);
		if (inverseDirection)
			p.direction = -p.direction;
	}

	p.lifetime = 0;
}