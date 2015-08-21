#include "scene.h"

#include "../GameObject/gameObject.h"
#include "../GameObject/Component/componentRigidBody.h"
#include "../GameObject/Component/componentRenderer.h"

Scene::Scene()
{
	sceneObjectsCount = 0;
}

void Scene::Initialize()
{
	for (vector<GameObject*>::size_type i = 0; i < sceneObjectsCount; i++)
		sceneObjects[i]->Initialize();
}

void Scene::Dispose()
{
	for (vector<GameObject*>::size_type i = 0; i < sceneObjectsCount; i++)
	{
		sceneObjects[i]->Dispose();
		delete sceneObjects[i];
	}
	sceneObjects.clear();
	sceneObjectsCount = 0;
}

void Scene::PreInput()
{
	for (vector<GameObject*>::size_type i = 0; i < sceneObjectsCount; i++)
		sceneObjects[i]->PreInput();
}

void Scene::PostInput()
{
	for (vector<GameObject*>::size_type i = 0; i < sceneObjectsCount; i++)
		sceneObjects[i]->PostInput();
}

void Scene::PreUpdate()
{
	for (vector<GameObject*>::size_type i = 0; i < sceneObjectsCount; i++)
		sceneObjects[i]->PreUpdate();
}

void Scene::Update()
{
	for (vector<GameObject*>::size_type i = 0; i < sceneObjectsCount; i++)
		sceneObjects[i]->Update();
}

void Scene::PostUpdate()
{
	for (vector<GameObject*>::size_type i = 0; i < sceneObjectsCount; i++)
		sceneObjects[i]->PostUpdate();
}

void Scene::PrePhysicsStep()
{
	for (vector<GameObject*>::size_type i = 0; i < sceneObjectsCount; i++)
		sceneObjects[i]->PrePhysicsStep();
}

void Scene::PostPhysicsStep()
{
	for (vector<GameObject*>::size_type i = 0; i < sceneObjectsCount; i++)
		sceneObjects[i]->PostPhysicsStep();
}

void Scene::PreRender()
{
	for (vector<GameObject*>::size_type i = 0; i < sceneObjectsCount; i++)
		sceneObjects[i]->PreRender();
}

void Scene::Render()
{
	for (vector<GameObject*>::size_type i = 0; i < sceneObjectsCount; i++)
		sceneObjects[i]->Render();
}

void Scene::PostRender()
{
	for (vector<GameObject*>::size_type i = 0; i < sceneObjectsCount; i++)
		sceneObjects[i]->PostRender();
}

void Scene::PreGUI()
{
	for (vector<GameObject*>::size_type i = 0; i < sceneObjectsCount; i++)
		sceneObjects[i]->PreGUI();
}

void Scene::GUI()
{
	for (vector<GameObject*>::size_type i = 0; i < sceneObjectsCount; i++)
		sceneObjects[i]->GUI();
}

void Scene::PostGUI()
{
	for (vector<GameObject*>::size_type i = 0; i < sceneObjectsCount; i++)
		sceneObjects[i]->PostGUI();
}

void Scene::OnResolutionChanged(int neww, int newh, int oldw, int oldh)
{
	for (vector<GameObject*>::size_type i = 0; i < sceneObjectsCount; i++)
		sceneObjects[i]->OnResolutionChanged(neww, newh, oldw, oldh);
}

void Scene::OnActivated()
{
	for (vector<GameObject*>::size_type i = 0; i < sceneObjectsCount; i++)
		if (sceneObjects[i]->HasRigidBody())
			sceneObjects[i]->GetRigidBody()->AddObjectToWorld();
}

void Scene::OnDeactivated()
{
	for (vector<GameObject*>::size_type i = 0; i < sceneObjectsCount; i++)
		if (sceneObjects[i]->HasRigidBody())
			sceneObjects[i]->GetRigidBody()->RemoveObjectFromWorld();
}



void Scene::AddSceneObject(GameObject* object)
{
	sceneObjects.push_back(object);
	sceneObjectsCount++;
}

void Scene::RemoveSceneObject(GameObject* object)
{
	for (auto a = sceneObjects.begin(); a != sceneObjects.end(); a++)
		if (*a == object)
		{
			sceneObjects.erase(a, a + 1);
			sceneObjectsCount--;
			return;
		}
}