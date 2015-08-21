#include "gameObject.h"

#include "Component/componentCollider.h"
#include "Component/componentRigidBody.h"
#include "Component/componentContactListener.h"
#include "Component/componentRenderer.h"
#include "Component/transformation.h"

unsigned int GameObject::CurID = 1;

GameObject::GameObject()
{
	parent = NULL;
	collider = NULL;
	rigidBody = NULL;
	contactListener = NULL;
	renderer = NULL;
	transformation = new Transformation(this);
}

void GameObject::Initialize()
{
	ID = CurID++;
	transformation->parentHasCollider = collider != 0;
	transformation->parentHasRigidBody = rigidBody != 0;
	if (collider)
		collider->Initialize(this);
	if (rigidBody)
		rigidBody->Initialize(this);
	if (contactListener)
		contactListener->Initialize(this);
	if (renderer)
		renderer->Initialize(this);

	for (auto a = children.begin(); a != children.end(); a++)
		(*a)->Initialize();
}

void GameObject::Dispose()
{
	parent = null;

	for (auto a = children.begin(); a != children.end(); a++)
	{
		(*a)->Dispose();
		delete *a;
	}
	children.clear();

	if (collider)
	{
		collider->Dispose();
		delete collider;
	}
	if (rigidBody)
	{
		rigidBody->Dispose();
		delete rigidBody;
	}
	if (contactListener)
	{
		contactListener->Dispose();
		delete contactListener;
	}
	if (renderer)
	{
		renderer->Dispose();
		delete renderer;
	}
}

void GameObject::SetPosition(float x, float y, float z)
{
	SetPosition(D3DXVECTOR3(x, y, z));
}

void GameObject::SetPosition(D3DXVECTOR3 pos)
{
	transformation->setPosition(pos);
	if (rigidBody)
		rigidBody->SetPosition(transformation->GetPositionHK());
}

void GameObject::Move(D3DXVECTOR3 delta)
{
	SetPosition(transformation->GetPosition() + delta);
}

void GameObject::AddLinearVelocity(D3DXVECTOR3 delta)
{
	rigidBody->AddLinearVelocity(delta.x, delta.y, delta.z);
}

void GameObject::AddRotation(float x, float y, float z)
{
	AddRotation(D3DXVECTOR3(x, y, z));
}

void GameObject::AddRotation(D3DXVECTOR3& rot)
{
	transformation->addRotation(rot);
	if (rigidBody)
		rigidBody->SetRotation(transformation->GetRotationHK());
}

void GameObject::SetRotation(D3DXVECTOR3 rot)
{
	transformation->setRotation(rot);
	if (rigidBody)
		rigidBody->SetRotation(transformation->GetRotationHK());
}

void GameObject::SetScale(D3DXVECTOR3 scale)
{
	transformation->setScale(scale);
	if (rigidBody)
		rigidBody->SetPosition(transformation->GetPositionHK());
}

void GameObject::SetLinearVelocity(D3DXVECTOR3 vel)
{
	if (rigidBody)
		rigidBody->SetLinearVelocity(hkVector4(vel.x, vel.y, vel.z));
}

void GameObject::SetParent(GameObject* _parent)
{
	if (parent == _parent)
		return;
	if (parent)
		parent->RemoveChild(this);
	parent = _parent;
	if (parent)
		parent->AddChild(this);
}

void GameObject::SetCollider(ComponentCollider* _collider)
{
	if (collider)
		collider->Dispose();
	collider = _collider;
	transformation->parentHasCollider = collider != 0;
}

void GameObject::SetRigidBody(ComponentRigidBody* _rigidBody)
{
	if (rigidBody)
		rigidBody->Dispose();
	rigidBody = _rigidBody;
	transformation->parentHasRigidBody = rigidBody != 0;
}

void GameObject::SetContactListener(ComponentContactListener* _contactListener)
{
	if (contactListener)
	{
		if (rigidBody)
			rigidBody->RemoveContactListener((hkpContactListener*)contactListener);
		contactListener->Dispose();
	}
	contactListener = _contactListener;
	if (contactListener && rigidBody)
		rigidBody->AddContactListener((hkpContactListener*)contactListener);
}

void GameObject::SetRenderer(ComponentRenderer* _renderer)
{
	if (renderer)
		renderer->Dispose();
	renderer = _renderer;
}

void GameObject::AddChild(GameObject* o)
{
	bool found = false;
	for (auto i = children.begin(); i != children.end(); i++)
		if (*i == o)
		{
			found = true;
			break;
		}
	if (!found)
	{
		children.push_back(o);
		o->SetParent(this);
	}
}

void GameObject::RemoveChild(GameObject* o)
{
	for (auto i = children.begin(); i != children.end(); i++)
		if (*i == o)
		{
			children.erase(i, i + 1);
			o->SetParent(null);
			return;
		}
}



void GameObject::PostPhysicsStep()
{
	if (rigidBody)
		rigidBody->UpdateGameObjectTransformation();

	for (auto a = children.begin(); a != children.end(); a++)
		(*a)->PostPhysicsStep();
}

void GameObject::Update()
{
	if (rigidBody)
		rigidBody->Update();
	if (renderer)
		renderer->Update();

	for (auto a = children.begin(); a != children.end(); a++)
		(*a)->Update();
}

void GameObject::Render()
{
	if (renderer)
		renderer->Render();

	for (auto a = children.begin(); a != children.end(); a++)
		(*a)->Render();
}

void GameObject::GUI()
{
	if (renderer)
		renderer->GUI();

	for (auto a = children.begin(); a != children.end(); a++)
		(*a)->GUI();
}