#include "renderTarget.h"

#include "renderTargetsStack.h"

vector<RenderTarget*> RenderTarget::fbos;

void RenderTarget::SetActiveRenderTarget()
{
	RenderTargetsStack::PushRenderTarget(this);
}

void RenderTarget::DisableActiveRenderTarget()
{
	RenderTargetsStack::PopRenderTarget(this);
}

void RenderTarget::OnResolutionChanged(int w, int h)
{
	for(int i = 0; i < fbos.size(); i++)
		if (fbos[i]->keepBackBufferSize)
		{
			fbos[i]->releaseResources();
			fbos[i]->createRenderTarget();
		}
}