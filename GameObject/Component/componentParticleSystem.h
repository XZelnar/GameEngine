#pragma once

#include "../../stdafx.h"

#include "iComponent.h"
#include "../../Graphics/ParticleSystem/particle.h"

class ComponentRenderer;
class psEmitter;
class Material;

class ComponentParticleSystem : public IGameObjectComponent
{
public:
	ComponentParticleSystem(UINT _maxParticles = 1024);

	void SetRenderer(ComponentRenderer* r);
	void SetEmitter(psEmitter* e);
	void SetMaxParticles(UINT v);
	void SetParticleLifetime(float t);
	void SetEmitParticlesPerSecond(UINT t);

	void SetSizeOverLifetime(Value<D3DXVECTOR3>* v);
	void SetColorOverLifetime(Value<D3DXVECTOR4>* v);

	psEmitter* GetEmitter() { return emitter; }

	virtual void Initialize(GameObject* _parent);
	virtual void Dispose();
	virtual void Update();
	virtual void Render();

private:
	void spawnParticle();

public:
	bool simulateInWorldSpace;
	D3DXVECTOR3 gravity;
	float particleStartingSpeed;

private:
	ComponentRenderer* renderer;
	Material* material;
	psEmitter* emitter;
	Particle* particles;
	UINT maxParticles;
	UINT emitParticlesPerSecond;
	float emitBuffer;

	float particleLifetime;
	float particleLifetimeIncPerSecond;

	Value<D3DXVECTOR3>* sizeOverLifetime;
	Value<D3DXVECTOR4>* colorOverLifetime;
	
	UINT deadParticles;
	UINT firstAlive, firstDead;
};