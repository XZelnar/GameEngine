#pragma once

#include "../stdafx.h"

class Material;

class MaterialsManager
{
	friend class Material;
	friend class GraphicsEngine;

private:
	static void Initialize();
	static void Dispose();
	static void addMaterial(Material* m);
	static void setActive(Material* m);
	static bool isActive(Material* m) { return active == m; }

private:
	static std::vector<Material*> materials;
	static Material* active;
};