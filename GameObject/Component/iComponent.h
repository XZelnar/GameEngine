#pragma once

#include "../../stdafx.h"

class GameObject;

class IGameObjectComponent
{
public:
	IGameObjectComponent() { parent = NULL; };
	GameObject* GetParent() { return parent; }

public:
	virtual void Initialize(GameObject* _parent) = 0;
	virtual void Dispose() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void GUI() { }

protected:
	GameObject* parent;
};