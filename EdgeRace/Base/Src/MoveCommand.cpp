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
	// ���̍��W��ۑ����Ă����ĐV�������W�ɒu��������
	auto posItr = newPosition.begin();
	for (Object3D* obj : targetObject)
	{
		obj->pObj.center = *posItr;
		posItr++;
	}
}

void MoveCommand::Undo()
{
	// �Â����W�ɒu��������
	auto posItr = oldPosition.begin();
	for (Object3D* obj : targetObject)
	{
		obj->pObj.center = *posItr;
		posItr++;
	}
}
