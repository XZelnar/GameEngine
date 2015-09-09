#pragma once

#include "../../stdafx.h"

struct Particle
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 direction;
	float lifetime;
	bool isAlive;

	Particle() : position(0, 0, 0), direction(0, 0, 0), lifetime(0), isAlive(false) { }
};