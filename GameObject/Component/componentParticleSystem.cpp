#include "componentParticleSystem.h"

#include "../gameObject.h"
#include "transformation.h"
#include "componentRenderer.h"
#include "../../Graphics/ParticleSystem/psEmitterPoint.h"
#include "../../Util/time.h"
#include "../../Graphics/material.h"

ComponentParticleSystem::ComponentParticleSystem(UINT _maxParticles) : simulateInWorldSpace(false), gravity(0, -10, 0)
{
	assert(_maxParticles != 0);
	particles = new Particle[maxParticles = deadParticles = _maxParticles];

	emitter = new psEmitterPoint();
	SetParticleLifetime(3);
	SetEmitParticlesPerSecond(20);
	emitBuffer = 0;
	particleStartingSpeed = 5;
	firstAlive = 0;
	firstDead = 0;

	sizeOverLifetime = new ValueLinear<D3DXVECTOR3>(D3DXVECTOR3(1, 1, 1), D3DXVECTOR3(0, 0, 0));
	colorOverLifetime = new ValueConstant<D3DXVECTOR4>(D3DXVECTOR4(1, 1, 1, 1));
}



void ComponentParticleSystem::SetRenderer(ComponentRenderer* r)
{
	if (r == null)
		return;
	renderer = r;
	material = (Material*)renderer->GetMaterial();
}

void ComponentParticleSystem::SetEmitter(psEmitter* e)
{
	if (!e)
		return;
	delete emitter;
	emitter = e;
}

void ComponentParticleSystem::SetMaxParticles(UINT v)
{
	assert(v != 0);
	delete[] particles;
	particles = new Particle[maxParticles = deadParticles = v];
	firstAlive = 0;
	firstDead = 0;
}

void ComponentParticleSystem::SetParticleLifetime(float t)
{
	particleLifetime = t;
	particleLifetimeIncPerSecond = 1.0f / particleLifetime;
}

void ComponentParticleSystem::SetEmitParticlesPerSecond(UINT t)
{
	t = min(t, maxParticles * min(.5f, particleLifetime));
	emitParticlesPerSecond = t;
}



void ComponentParticleSystem::SetSizeOverLifetime(Value<D3DXVECTOR3>* v)
{
	delete sizeOverLifetime;
	sizeOverLifetime = v;
}

void ComponentParticleSystem::SetColorOverLifetime(Value<D3DXVECTOR4>* v)
{
	delete colorOverLifetime;
	colorOverLifetime = v;
}



void ComponentParticleSystem::Initialize(GameObject* _parent)
{
	parent = _parent;
}

void ComponentParticleSystem::Dispose()
{
	delete[] particles;
}

void ComponentParticleSystem::Update()
{
	static Particle* p;
	static D3DXVECTOR3 grav;
	grav = gravity * Time::GetGameDeltaTime();
	//grav = D3DXVECTOR3(0, 0, 0);
	static float time;
	time = particleLifetimeIncPerSecond * Time::GetGameDeltaTime();

	if (firstDead < firstAlive)
	{
		for (int i = firstAlive; i < maxParticles; i++)
		{
			p = particles + i;
			p->lifetime += time;
			if (p->lifetime >= 1)
			{
				p->isAlive = false;
				deadParticles++;
				firstAlive++;
				if (firstAlive >= maxParticles)
					firstAlive = 0;
				continue;
			}
			p->direction += grav;
			p->position += p->direction * Time::GetGameDeltaTime();
		}
		for (int i = 0; i < firstDead; i++)
		{
			p = particles + i;
			p->lifetime += time;
			if (p->lifetime >= 1)
			{
				p->isAlive = false;
				deadParticles++;
				firstAlive++;
				if (firstAlive >= maxParticles)
					firstAlive = 0;
				continue;
			}
			p->direction += grav;
			p->position += p->direction * Time::GetGameDeltaTime();
		}
	}
	else
	{
		for (int i = firstAlive; i < firstDead; i++)
		{
			p = particles + i;
			p->lifetime += time;
			if (p->lifetime >= 1)
			{
				p->isAlive = false;
				deadParticles++;
				firstAlive++;
				if (firstAlive >= maxParticles)
					firstAlive = 0;
				continue;
			}
			p->direction += grav;
			p->position += p->direction * Time::GetGameDeltaTime();
		}
	}

	emitBuffer += emitParticlesPerSecond * Time::GetGameDeltaTime();
	while (emitBuffer > 1)
	{
		emitBuffer--;
		spawnParticle();
	}
}

void ComponentParticleSystem::spawnParticle()
{
	if (deadParticles == 0)
		return;
	deadParticles--;

	static Particle* p;

	p = particles + firstDead;
	emitter->EmitParticle(*p);
	p->direction *= particleStartingSpeed;
	p->isAlive = true;

	if (simulateInWorldSpace)
	{
		static D3DXMATRIX mTransform, mParent;
		mParent = *parent->GetTransformation()->GetTransformationMatrix();
		D3DXMatrixTranslation(&mTransform, p->position.x, p->position.y, p->position.z);
		mTransform = mTransform * mParent;
		p->position.x = mTransform._41;
		p->position.y = mTransform._42;
		p->position.z = mTransform._43;

		mParent = *parent->GetTransformation()->GetRotationMatrix();
		D3DXMatrixTranslation(&mTransform, p->direction.x, p->direction.y, p->direction.z);
		mTransform = mTransform * mParent;
		p->direction.x = mTransform._41;
		p->direction.y = mTransform._42;
		p->direction.z = mTransform._43;
	}

	firstDead++;
	if (firstDead >= maxParticles)
		firstDead = 0;
}

void ComponentParticleSystem::Render()
{
	static D3DXMATRIX mTransform, mParent, mtp;
	static D3DXVECTOR3 v3tp;
	if (!simulateInWorldSpace)
		mParent = *parent->GetTransformation()->GetTransformationMatrix();
	static Particle* p;

	for (int i = 0; i < maxParticles; i++)
	{
		p = &particles[i];
		if (p->isAlive)
		{
			D3DXMatrixTranslation(&mTransform, p->position.x, p->position.y, p->position.z);
			if (!simulateInWorldSpace)
				mTransform = mTransform * mParent;
			material->SetColor(colorOverLifetime->GetValue(p->lifetime));
			v3tp = sizeOverLifetime->GetValue(p->lifetime);
			D3DXMatrixScaling(&mtp, v3tp.x, v3tp.y, v3tp.z);
			mTransform = mtp * mTransform;
			renderer->Render(&mTransform);
		}
	}
}