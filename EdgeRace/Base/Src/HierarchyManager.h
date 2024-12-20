#pragma once
#include "Object3D.h"

namespace HierarchyManager
{
	void AddHierarchy(Object3D* obj);
	void RemoveHierarchy(Object3D* obj);
	std::list<Object3D*> GetHierarchyList();
}