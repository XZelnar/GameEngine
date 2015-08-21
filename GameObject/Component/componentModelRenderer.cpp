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

void ComponentModelRenderer::Initialize(GameObject* _parent)
{
	parent = _parent;
	pTransformation = parent->GetTransformation();
}

void ComponentModelRenderer::SetMesh(Mesh* m)
{
	mesh = m;
}

void ComponentModelRenderer::Render()
{
	if (material && mesh && IsVisible && GraphicsEngine::GetRenderPass() != RENDER_PASS_TYPE::RP_GUI)
	{
		material->Activate();
		ShaderManager::SetObjectTransformation(pTransformation);
		mesh->Render();
	}
}