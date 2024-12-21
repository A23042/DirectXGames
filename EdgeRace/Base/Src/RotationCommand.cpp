#include "RotationCommand.h"

RotationCommand::RotationCommand(list<Object3D*> targetObj, list<VECTOR3> oldRot) : targetObject(targetObj), oldRotation(oldRot)
{
	for (Object3D* obj : targetObj)
	{
		newRotation.push_back(obj->Rotation());
	}
}

void RotationCommand::Do()
{
	auto posItr = newRotation.begin();
	for (Object3D* obj : targetObject)
	{
		obj->SetRotation(*posItr);
		posItr++;
	}
}

void RotationCommand::Undo()
{
	auto posItr = oldRotation.begin();
	for (Object3D* obj : targetObject)
	{
		obj->SetRotation(*posItr);
		posItr++;
	}
}
