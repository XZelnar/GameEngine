#include "componentModelRenderer.h"

#include "transformation.h"
#include "../gameObject.h"
#include "../../Graphics/mesh.h"
#include "../../Graphics/texture.h"
#include "../../Graphics/graphicsEngine.h"
#include "../../Graphics/shaderManager.h"
#include "../../Graphics/material.h"

ComponentModelRenderer::ComponentModelRenderer() : ComponentRenderer()
{ 
	IsVisible = true;
	material = null;
	//effect = (Effect*)EffectsManager::GetDefaultEffect();//TODO
}

void ComponentModelRenderer::SetMesh(Mesh* m)
{
	mesh = m;
}

void ComponentModelRenderer::Render(D3DXMATRIX* world)
{
	assert(material);
	assert(mesh);
	if (IsVisible && GraphicsEngine::GetRenderPass() != RENDER_PASS_TYPE::RP_GUI)
	{
		material->Activate();
		ShaderManager::SetMatrixWorld(world);
		mesh->Render();
	}
}