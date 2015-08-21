#include "componentColliderConvexVertices.h"

#ifndef NO_HAVOK_PHYSICS
#include "componentRigidBody.h"
#include "../gameObject.h"
#include "transformation.h"
#include "../../Graphics/mesh.h"

ComponentColliderConvexVertices::ComponentColliderConvexVertices(Mesh* m) : ComponentCollider()
{
	mesh = m;
	collider = null;
}

void ComponentColliderConvexVertices::Initialize(GameObject* _parent)
{
	parent = _parent;

	int vc = mesh->GetVertexCount();
	hkReal* vraw = new hkReal[vc * 4];
	auto a = mesh->GetVertices();
	assert(a);
	for (int i = 0; i < vc; i++)
	{
		vraw[i * 4] = a[i].POSITION.x;
		vraw[i * 4 + 1] = a[i].POSITION.y;
		vraw[i * 4 + 2] = a[i].POSITION.z;
		vraw[i * 4 + 3] = 0;
	}

	vertices.m_numVertices = vc;
	vertices.m_striding = sizeof(hkReal) * 4;
	vertices.m_vertices = vraw;

	collider = new hkpConvexVerticesShape(vertices);
}

void ComponentColliderConvexVertices::Dispose()
{
	if (collider)
	{
		collider->removeReference();
		delete collider;
		delete[] vertices.m_vertices;
	}
}

hkMassProperties& ComponentColliderConvexVertices::GetMassProperties(hkReal mass)
{
	auto res = hkInertiaTensorComputer::computeVertexCloudMassProperties(vertices.m_vertices, vertices.m_striding, vertices.m_numVertices, mass, r);
	return r;
}
#endif