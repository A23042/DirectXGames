#include "MoveCommand.h"

MoveCommand::MoveCommand(list<Object3D*> targetObj, list<VECTOR3> oldPos) : targetObject(targetObj), oldPosition(oldPos)
{
	for (Object3D* obj : targetObj)
	{
		newPosition.push_back(obj->Position());
	}
}

void MoveCommand::Do()
{
	// 元の座標を保存しておいて新しい座標に置き換える
	auto posItr = newPosition.begin();
	for (Object3D* obj : targetObject)
	{
		obj->pObj.center = *posItr;
		posItr++;
	}
}

void MoveCommand::Undo()
{
	// 古い座標に置き換える
	auto posItr = oldPosition.begin();
	for (Object3D* obj : targetObject)
	{
		obj->pObj.center = *posItr;
		posItr++;
	}
}
