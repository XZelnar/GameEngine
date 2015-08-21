#include "componentRigidBody.h"

#ifndef NO_HAVOK_PHYSICS
#include "../gameObject.h"
#include "componentCollider.h"
#include "../../Physics/physicsEngine.h"
#include "transformation.h"

ComponentRigidBody::ComponentRigidBody() : IGameObjectComponent()
{
	forceGameObjectRotation = false;
	isStatic = false;
	rigidBody = HK_NULL;
	mass = 1;
	restitution = 0.3f;
	friction = 0.8f;
	linearDamping = 0.5f;
	gravityFactor = 1;
	isInWorld = false;
	SyncRotation = true;
}

ComponentRigidBody::ComponentRigidBody(hkReal _mass) : IGameObjectComponent()
{
	forceGameObjectRotation = false;
	isStatic = false;
	rigidBody = HK_NULL;
	mass = _mass;
	restitution = 0.3f;
	friction = 0.8f;
	linearDamping = 0.5f;
	gravityFactor = 1;
	isInWorld = false;
	SyncRotation = true;
}

void ComponentRigidBody::Initialize(GameObject* _parent)
{
	parent = _parent;

	ComponentCollider* collider = parent->GetCollider();
	if (!collider)
		return;
	
	hkpRigidBodyCinfo rbInfo;
	//shape = *(*collider).GetCollider();
	rbInfo.m_shape = collider->GetCollider();
	rbInfo.m_position = parent->GetTransformation()->GetPositionHK();
	rbInfo.m_rotation = parent->GetTransformation()->GetRotationHK();
	rbInfo.m_motionType = isStatic ? hkpMotion::MOTION_FIXED : hkpMotion::MOTION_DYNAMIC;
	if (!isStatic)
		rbInfo.setMassProperties(((ComponentCollider*)collider)->GetMassProperties(mass));
	
	rigidBody = new hkpRigidBody(rbInfo);

	rigidBody->setRestitution(restitution);
	rigidBody->setFriction(friction);
	rigidBody->setLinearDamping(linearDamping);
	rigidBody->setGravityFactor(gravityFactor);

	if (parent->contactListener)
		rigidBody->addContactListener((hkpContactListener*)parent->contactListener);
}

void ComponentRigidBody::Dispose()
{
	if (rigidBody)
	{
		if (isInWorld)
			RemoveObjectFromWorld();
		rigidBody->removeReference();
		isInWorld = false;
	}
}

void ComponentRigidBody::AddObjectToWorld()
{
	PhysicsEngine::AddEntity(rigidBody);
	isInWorld = true;
}

void ComponentRigidBody::RemoveObjectFromWorld()
{
	PhysicsEngine::RemoveEntity(rigidBody);
	isInWorld = false;
}

void ComponentRigidBody::AddContactListener(hkpContactListener* l)
{
	if (rigidBody)
		rigidBody->addContactListener(l);
}

void ComponentRigidBody::RemoveContactListener(hkpContactListener* l)
{
	if (rigidBody)
		rigidBody->removeContactListener(l);
}

hkVector4 ComponentRigidBody::GetPosition()
{
	if (!rigidBody)
		return hkVector4();
	return rigidBody->getPosition();
}

const hkpRigidBody* ComponentRigidBody::GetRigidBody()
{
	return rigidBody;
}

void ComponentRigidBody::SetPosition(const hkVector4& pos)
{
	if (rigidBody)
		rigidBody->setPosition(pos);
}

void ComponentRigidBody::SetRotation(hkQuaternion& rot)
{
	if (rigidBody)
	{
		hkVector4 ar = rigidBody->getAngularVelocity();
		rigidBody->setRotation(rot);
		rigidBody->setAngularVelocity(ar);
	}
}

void ComponentRigidBody::SetMass(hkReal _mass)
{
	mass = _mass;
	rigidBody->setMass(_mass);
}

void ComponentRigidBody::SetRestitution(float res)
{
	restitution = res;
	if (rigidBody)
		rigidBody->setRestitution(res);
}

void ComponentRigidBody::SetFriction(float fr)
{
	friction = fr;
	if (rigidBody)
		rigidBody->setFriction(fr);
}

void ComponentRigidBody::SetLinearDamping(float ld)
{
	linearDamping = ld;
	if (rigidBody)
		rigidBody->setLinearDamping(ld);
}

void ComponentRigidBody::SetLinearVelocity(const hkVector4& vel)
{
	if (rigidBody)
		rigidBody->setLinearVelocity(vel);
}

void ComponentRigidBody::SetGravityFactor(float f)
{
	gravityFactor = f;
	if (rigidBody)
		rigidBody->setGravityFactor(f);
}

void ComponentRigidBody::SetIsStatic(bool _isStatic)
{
	isStatic = _isStatic;
	if (rigidBody)
		rigidBody->setMotionType(isStatic ? hkpMotion::MOTION_FIXED : hkpMotion::MOTION_DYNAMIC);
}

void ComponentRigidBody::AddLinearVelocity(float x, float y, float z)
{
	static hkVector4 linVel;
	linVel = rigidBody->getLinearVelocity();
	linVel(0) += x;
	linVel(1) += y;
	linVel(2) += z;
	rigidBody->setLinearVelocity(linVel);
}

void ComponentRigidBody::OnSizeChanged()
{
	rigidBody->setShape(((ComponentCollider*)parent->GetCollider())->GetCollider());
}

void ComponentRigidBody::UpdateGameObjectTransformation()
{
	if (!isStatic)
	{
		parent->transformation->setPositionHK(rigidBody->getPosition());
		if (SyncRotation)
		{
			if (forceGameObjectRotation)
				SetRotation(parent->transformation->GetRotationHK());
			else
				parent->transformation->setRotationHK(rigidBody->getRotation());
		}
	}
}

void ComponentRigidBody::Render() { } //Disabled in GameObject
#endif