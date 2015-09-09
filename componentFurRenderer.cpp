#include "componentFurRenderer.h"

#include "Graphics/mesh.h"
#include "Graphics/graphicsEngine.h"
#include "Graphics/shaderManager.h"
#include "Graphics/material.h"


void ComponentFurRenderer::Render(D3DXMATRIX* world)
{
	if (material && mesh && IsVisible && GraphicsEngine::GetRenderPass() != RENDER_PASS_TYPE::RP_GUI)
	{
		//material->SetCBuffer("IterationInfo", D3DXVECTOR4(0, 0, 0, 0));
		//ComponentModelRenderer::Render(world);
		//GraphicsEngine::DisableDepth();
		material->Activate();
		ShaderManager::SetMatrixWorld(world);
		for (float i = 0; i < iterations; i++)
		{
			material->SetCBuffer("IterationInfo", D3DXVECTOR4(i / iterations, 0, 0, 0));
			mesh->Render();
		}
		//GraphicsEngine::EnableDepth();
	}
}