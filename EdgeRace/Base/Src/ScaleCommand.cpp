#include "ScaleCommand.h"

ScaleCommand::ScaleCommand(list<Object3D*> targetObj, list<VECTOR3> oldScale) : targetObject(targetObj), oldScale(oldScale)
{
	for (Object3D* obj : targetObj)
	{
		newScale.push_back(obj->Scale());
	}
}

void ScaleCommand::Do()
{
	auto posItr = newScale.begin();
	for (Object3D* obj : targetObject)
	{
		obj->SetScale(*posItr);
		posItr++;
	}
}

void ScaleCommand::Undo()
{
	auto posItr = oldScale.begin();
	for (Object3D* obj : targetObject)
	{
		obj->SetScale(*posItr);
		posItr++;
	}
}
