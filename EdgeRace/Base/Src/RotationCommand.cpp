#include "RotationCommand.h"

void RotationCommand::Do()
{
	targetObject->SetPosition(newRotation);
}

void RotationCommand::Undo()
{
	targetObject->SetRotation(oldRotation);
}
