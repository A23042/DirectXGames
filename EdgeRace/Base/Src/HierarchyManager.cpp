#include "HierarchyManager.h"

namespace
{
	std::list<Object3D*> hierarchyList;
}

void HierarchyManager::AddHierarchy(Object3D* obj)
{
	// ヒエラルキーになければ追加
	if (std::find(hierarchyList.begin(), hierarchyList.end(), obj) == hierarchyList.end())
	{
		hierarchyList.push_back(obj);
	}
}

void HierarchyManager::AddHierarchy(std::list<Object3D*> objs)
{
	for (Object3D* obj : objs)
	{
		// ヒエラルキーになければ追加
		if (std::find(hierarchyList.begin(), hierarchyList.end(), obj) == hierarchyList.end())
		{
			hierarchyList.push_back(obj);
		}
	}
}

void HierarchyManager::RemoveHierarchy(Object3D* obj)
{
	// ヒエラルキーにあれば削除
	if (std::find(hierarchyList.begin(), hierarchyList.end(), obj) != hierarchyList.end())
	{
		hierarchyList.remove(obj);
		//obj->DestroyMe();
	}
}

void HierarchyManager::RemoveHierarchy(std::list<Object3D*> objs)
{
	for(Object3D* obj : objs)
	{
		// ヒエラルキーにあれば削除
		if (std::find(hierarchyList.begin(), hierarchyList.end(), obj) != hierarchyList.end())
		{
			hierarchyList.remove(obj);
			//obj->DestroyMe();
		}
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
