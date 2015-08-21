#include "componentFurRenderer.h"

#include "Graphics/mesh.h"
#include "Graphics/graphicsEngine.h"
#include "Graphics/shaderManager.h"
#include "Graphics/material.h"


void ComponentFurRenderer::Render()
{
	if (material && mesh && IsVisible && GraphicsEngine::GetRenderPass() != RENDER_PASS_TYPE::RP_GUI)
	{
		material->SetVertexCBuffer(materialIterVarIndex, D3DXVECTOR4(0, 0, 0, 0));
		ComponentModelRenderer::Render();
		//GraphicsEngine::DisableDepth();
		material->Activate();
		ShaderManager::SetObjectTransformation(pTransformation);
		for (float i = 1; i < iterations; i++)
		{
			material->SetVertexCBuffer(materialIterVarIndex, D3DXVECTOR4(i / iterations, 0, 0, 0));
			mesh->Render();
		}
		//GraphicsEngine::EnableDepth();
	}
}