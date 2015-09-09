#include "stdafx.h"
#include "window.h"
#include "Graphics/graphicsEngine.h"
#include "Graphics/shaderManager.h"
#include "Physics/physicsEngine.h"
#include "Input/inputManager.h"
#include "Scene/sceneManager.h"
#include "Util/time.h"

#include "Graphics/graphicsResourceManager.h"
#include "Scene/scene.h"
#include "Graphics/textureDynamic.h"
#include "GameObject/gameObject.h"
#include "GameObject/Component/componentColliderBox.h"
#include "GameObject/Component/componentColliderConvexVertices.h"
#include "componentFurRenderer.h"
#include "GameObject/Component/componentRigidBody.h"
#include "Graphics/mesh.h"
#include "Graphics/material.h"
#include "Graphics/Camera/cameraManager.h"
#include "Graphics/Camera/cameraProjection.h"
#include "GameObject/Component/transformation.h"
#include "GameObject/Component/componentParticleSystem.h"
#include "Graphics/ParticleSystem/psEmitterMesh.h"
#include "engineValue.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Window::Initialize(hInstance, nCmdShow);
	PhysicsEngine::Initialize();
	GraphicsEngine::Initialize(Window::GetHWND());
	InputManager::Initialize(hInstance);
	SceneManager::Initialize();



	auto a = new Scene();
	GameObject* f, *g;
	Material* mat;
	ComponentModelRenderer* cmr;

	//floor
	{
		f = new GameObject();
		f->SetPosition(0, -20, 0);
		f->SetScale(D3DXVECTOR3(100, 1, 100));
		auto r = cmr = new ComponentModelRenderer();
		r->SetMesh(GraphicsResourceManager::GetInstance().GetMesh(L"Resources/box.obj"));
		auto fmat = new Material(GraphicsResourceManager::GetInstance().GetShader(L"Resources/Shaders/simple.fx"));
		fmat->SetTexture(GraphicsResourceManager::GetInstance().GetTexture(L"Resources/texel.png"), 0);
		r->SetMaterial(fmat);
		f->SetRenderer(r);
		auto c = new ComponentColliderBox(hkVector4(1, 1, 1, 0));
		f->SetCollider(c);
		auto rb = new ComponentRigidBody(1.0f);
		rb->SetIsStatic(true);
		f->SetRigidBody(rb);
	}

	//torus
	{
		g = new GameObject();
		g->SetPosition(0, 0, 0);
		//g->SetRotation(D3DXVECTOR3(D3DX_PI / 2 - 0.01f, 2.7f, 0));
		auto r = new ComponentFurRenderer();
		r->iterations = 40;
		r->SetMesh(GraphicsResourceManager::GetInstance().GetMesh(L"Resources/torus.obj"));
		mat = new Material(GraphicsResourceManager::GetInstance().GetShader(L"Resources/Shaders/fur.fx"));
		mat->SetTexture(GraphicsResourceManager::GetInstance().GetTexture(L"Resources/mask.png"), 1);
		r->SetMaterial(mat);
		g->SetRenderer(r);
		auto c = new ComponentColliderConvexVertices(GraphicsResourceManager::GetInstance().GetMesh(L"Resources/torusLP.obj"));
		g->SetCollider(c);
		auto rb = new ComponentRigidBody(1.0f);
		g->SetRigidBody(rb);

		auto ps = new ComponentParticleSystem(2048);
		ps->SetRenderer(cmr);
		ps->simulateInWorldSpace = true;
		ps->gravity = D3DXVECTOR3(0, 0, 0);
		ps->SetParticleLifetime(1);
		ps->SetEmitParticlesPerSecond(600);
		ps->SetSizeOverLifetime(new ValueLinear<D3DXVECTOR3>(D3DXVECTOR3(.2f, .2f, .2f), D3DXVECTOR3(0, 0, 0)));
		ps->SetColorOverLifetime(new ValueConstant<D3DXVECTOR4>(D3DXVECTOR4(0.2f, 0.8f, 0.5f, 1)));
		auto pse = new psEmitterMesh(r->GetMesh(), psEmitterMesh::EmissionSource::ES_Points);
		ps->SetEmitter(pse);
		g->SetParticleSystem(ps);
	}

	//fur texture
	const int w = 256, h = 1280;
	int gridSize = 5;
	TextureDynamic* t = new TextureDynamic(w, h);
	byte* arr = new byte[w * h * 4];
	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++)
		{
			//arr[(x + y * w) * 4] = arr[(x + y * w) * 4 + 1] = arr[(x + y * w) * 4 + 2] = 128;
			arr[(x + y * w) * 4 + 3] = byte((sin((float)y) * sin((float)x) + 1 ) * 128);
		}
	t->SetPixels(arr);
	delete[] arr;
	mat->SetTexture(t);
	mat->SetCBuffer("FurInfo", D3DXVECTOR4(3, 0, -1, 0));
	mat->SetCBuffer("IterationInfo", D3DXVECTOR4(0, 0, 0, 0));

	//general
	a->AddSceneObject(g);
#ifndef NO_HAVOK_PHYSICS
	a->AddSceneObject(f);
#endif
	a->Initialize();
	SceneManager::AddScene("main", a);

	CameraProjection* cp = new CameraProjection();
	cp->SetPosition(D3DXVECTOR3(0, 0, 40), D3DXVECTOR3(0, 0, 0));
	CameraManager::SetActiveCamera(cp);



	DWORD timeStep = 1000.0f / 60.0f;
	DWORD timeLast = timeGetTime();

	MSG msg;
	while(1)
	{
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				goto postMainCycle;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

#ifdef NO_HAVOK_PHYSICS
		static float t = D3DX_PI;
		t -= 0.01f;
		if (t < -D3DX_PI)
			t += D3DX_PI * 2;
		g->SetRotation(D3DXVECTOR3(D3DX_PI / 2, t, 0));
#endif

		cp->SetLookAt(g->GetTransformation()->GetPosition());

		Time::Update();
		
		SceneManager::PreInput();
		InputManager::Update();
		SceneManager::PostInput();
		
		SceneManager::PrePhysicsStep();
		PhysicsEngine::SimulationStep(1.0f / 60.0f);
		SceneManager::PostPhysicsStep();
		
		SceneManager::PreUpdate();
		SceneManager::Update();
		GraphicsEngine::Update();
		SceneManager::PostUpdate();
		
		SceneManager::PreRender();
		GraphicsEngine::BeginFrame();
		SceneManager::Render();
		SceneManager::PostRender();

		SceneManager::PreGUI();
		GraphicsEngine::BeginGUI();
		SceneManager::GUI();
		SceneManager::PostGUI();

		GraphicsEngine::EndFrame();

		//while(timeGetTime() < timeLast + timeStep) ;
		//timeLast = timeGetTime();
	}

	postMainCycle:
	
	SceneManager::Dispose();
	InputManager::Dispose();
	GraphicsResourceManager::GetInstance().Dispose();
	GraphicsEngine::Dispose();

	return 0;
}