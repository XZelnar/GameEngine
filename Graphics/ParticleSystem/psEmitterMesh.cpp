#include "psEmitterMesh.h"

#include "../mesh.h"

psEmitterMesh::psEmitterMesh(Mesh* _mesh, EmissionSource es)
{
	mesh = _mesh;
	source = es;

	auto t = topology = mesh->GetTopology();
	if (t == D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP || t == D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ || t == D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ ||
		t == D3D11_PRIMITIVE_TOPOLOGY_LINELIST || t == D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP ||
		t == D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ || t == D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ)
		throw exception("Unsupported topology");
	if (t != D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST && source == EmissionSource::ES_Triangles)
		throw exception("Mesh type is incompatible with emission source");
	if (t == D3D11_PRIMITIVE_TOPOLOGY_POINTLIST && source != EmissionSource::ES_Points)
		throw exception("Mesh type is incompatible with emission source");
}

void psEmitterMesh::EmitParticle(Particle& p)
{
	uniform_int<int> rand;
	static uniform_real<float> rand2(0, 1);
	static uniform_real<float> rand3(-1, 1);
	static default_random_engine e_rand;

	if (source == EmissionSource::ES_Points)
	{
		rand = uniform_int<int>(0, mesh->GetVertexCount());
		int i = rand(e_rand);
		auto a = mesh->GetVertex(i);
		p.position = a.POSITION;
		if (randomDirection)
		{
			p.direction.x = rand3(e_rand);
			p.direction.y = rand3(e_rand);
			p.direction.z = rand3(e_rand);
			D3DXVec3Normalize(&p.direction, &p.direction);
		}
		else
		{
			p.direction = a.NORMAL;
			if (inverseDirection)
				p.direction = -p.direction;
		}
	}
	else if (source == EmissionSource::ES_Lines)
	{
		Mesh::VERTEX v1, v2;
		if (topology == D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
		{
			rand = uniform_int<int>(0, mesh->GetVertexCount());
			int i = rand(e_rand);
			if (i % 3 == 2) i--;
			v1 = mesh->GetVertex(i);
			v2 = mesh->GetVertex(i + 1);
		}
		else if (topology == D3D11_PRIMITIVE_TOPOLOGY_LINELIST)
		{
			rand = uniform_int<int>(0, mesh->GetVertexCount() / 2);
			int i = rand(e_rand) * 2;
			v1 = mesh->GetVertex(i);
			v2 = mesh->GetVertex(i + 1);
		}
		
		float t = rand2(e_rand);
		p.position = v1.POSITION + (v2.POSITION - v1.POSITION) * t;
		if (randomDirection)
		{
			p.direction.x = rand3(e_rand);
			p.direction.y = rand3(e_rand);
			p.direction.z = rand3(e_rand);
			D3DXVec3Normalize(&p.direction, &p.direction);
		}
		else
		{
			p.direction = v1.NORMAL + (v2.NORMAL - v1.NORMAL) * t;
			if (inverseDirection)
				p.direction = -p.direction;
		}
	}
	else if (source == EmissionSource::ES_Triangles)
	{
		Mesh::VERTEX v1, v2, v3;
		D3DXVECTOR3 d2, d3;

		rand = uniform_int<int>(0, mesh->GetVertexCount() / 3);
		int i = rand(e_rand) * 3;
		v1 = mesh->GetVertex(i);
		v2 = mesh->GetVertex(i + 1);
		v3 = mesh->GetVertex(i + 2);

		float t2 = rand2(e_rand);
		float t3 = rand2(e_rand);
		if (t2 + t3 > 1)
		{
			t2 = 1 - t2;
			t3 = 1 - t3;
		}

		d2 = v2.POSITION - v1.POSITION;
		d3 = v3.POSITION - v1.POSITION;
		p.position = v1.POSITION + d2 * t2 + d3 * t3;

		if (randomDirection)
		{
			p.direction.x = rand3(e_rand);
			p.direction.y = rand3(e_rand);
			p.direction.z = rand3(e_rand);
			D3DXVec3Normalize(&p.direction, &p.direction);
		}
		else
		{
			p.direction = v1.NORMAL * (1 - t2 - t3) + v2.NORMAL * t2 + v3.NORMAL * t3;
			if (inverseDirection)
				p.direction = -p.direction;
		}
	}

	p.lifetime = 0;
}