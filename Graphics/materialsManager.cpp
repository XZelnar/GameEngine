#include "materialsManager.h"

#include "material.h"

std::vector<Material*> MaterialsManager::materials;
Material* MaterialsManager::active = null;

void MaterialsManager::Initialize()
{
}

void MaterialsManager::Dispose()
{
	for (auto i = materials.begin(); i != materials.end(); i++)
		delete *i._Ptr;
	materials.clear();
}



void MaterialsManager::addMaterial(Material* m)
{
	if (m)
		materials.push_back(m);
}

void MaterialsManager::setActive(Material* m)
{
	if (active)
		active->isActive = false;
	active = m;
	if (m)
		m->isActive = true;
}