#include "HierarchyManager.h"

namespace
{
	std::list<Object3D*> hierarchyList;
}

void HierarchyManager::AddHierarchy(Object3D* obj)
{
	// �q�G�����L�[�ɂȂ���Βǉ�
	if (std::find(hierarchyList.begin(), hierarchyList.end(), obj) == hierarchyList.end())
	{
		hierarchyList.push_back(obj);
	}
}

void HierarchyManager::RemoveHierarchy(Object3D* obj)
{
	// �q�G�����L�[�ɂ���΍폜
	if (std::find(hierarchyList.begin(), hierarchyList.end(), obj) != hierarchyList.end())
	{
		hierarchyList.remove(obj);
		obj->DestroyMe();
	}
}

void HierarchyManager::ClearHierarchy()
{
	hierarchyList.clear();
}

std::list<Object3D*> HierarchyManager::GetHierarchyList()
{
	return hierarchyList;
}
