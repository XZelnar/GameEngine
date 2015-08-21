#include "sceneManager.h"

#include "scene.h"

Scene* SceneManager::activeScene = null;
map<char*, Scene*, cmp_LPCSTR> SceneManager::loadedScenes;
int SceneManager::scenesCount = 0;

void SceneManager::Initialize()
{
}

void SceneManager::Dispose()
{
	for (auto i = loadedScenes.begin(); i != loadedScenes.end(); i++)
	{
		i->second->Dispose();
		delete i->second;
	}
	loadedScenes.clear();
	scenesCount = 0;
}



Scene& SceneManager::AddScene(char* name)
{
	auto a = loadedScenes[name];
	if (!a)
	{
		a = loadedScenes[name] = new Scene();
		a->Initialize();
	}

	if (!activeScene)
		SetActiveScene(name);

	return *a;
}

Scene& SceneManager::AddScene(char* name, Scene* s)
{
	auto a = loadedScenes[name];
	if (a)
		throw std::exception("Scene with same name already exists");
	loadedScenes[name] = s;
	s->Initialize();
	if (!activeScene)
		SetActiveScene(name);
	return *s;
}

Scene& SceneManager::GetScene(char* name)
{
	return *loadedScenes[name];
}

bool SceneManager::SceneExists(char* name)
{
	return loadedScenes[name];
}

void SceneManager::RemoveScene(char* name)
{
	auto a = loadedScenes[name];
	a->Dispose();
	delete a;
	loadedScenes.erase(name);
}



void SceneManager::PreInput()
{
	assert(activeScene);
	activeScene->PreInput();
}

void SceneManager::PostInput()
{
	assert(activeScene);
	activeScene->PostInput();
}

void SceneManager::PreUpdate()
{
	assert(activeScene);
	activeScene->PreUpdate();
}

void SceneManager::Update()
{
	assert(activeScene);
	activeScene->Update();
}

void SceneManager::PostUpdate()
{
	assert(activeScene);
	activeScene->PostUpdate();
}

void SceneManager::PrePhysicsStep()
{
	assert(activeScene);
	activeScene->PrePhysicsStep();
}

void SceneManager::PostPhysicsStep()
{
	assert(activeScene);
	activeScene->PostPhysicsStep();
}

void SceneManager::PreRender()
{
	assert(activeScene);
	activeScene->PreRender();
}

void SceneManager::Render()
{
	assert(activeScene);
	activeScene->Render();
}

void SceneManager::PostRender()
{
	assert(activeScene);
	activeScene->PostRender();
}

void SceneManager::PreGUI()
{
	assert(activeScene);
	activeScene->PreGUI();
}

void SceneManager::GUI()
{
	assert(activeScene);
	activeScene->GUI();
}

void SceneManager::PostGUI()
{
	assert(activeScene);
	activeScene->PostGUI();
}

void SceneManager::OnResolutionChanged(int neww, int newh, int oldw, int oldh)
{
	assert(activeScene);
	activeScene->OnResolutionChanged(neww, newh, oldw, oldh);
}



void SceneManager::SetActiveScene(char* name)
{
	if (loadedScenes[name] == null)
		return;
	if (activeScene)
		activeScene->OnDeactivated();
	activeScene = loadedScenes[name];
	activeScene->OnActivated();
}