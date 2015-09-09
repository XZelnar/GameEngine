#pragma once

#include "psEmitter.h"

class Mesh;

class psEmitterMesh : public psEmitter
{
public:
	enum EmissionSource
	{
		ES_Points,
		ES_Lines,
		ES_Triangles
	};

public:
	psEmitterMesh(Mesh* _mesh, EmissionSource es);

private:
	virtual void EmitParticle(Particle& p);

private:
	Mesh* mesh;
	EmissionSource source;
	D3D11_PRIMITIVE_TOPOLOGY topology;
};