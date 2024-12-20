#include "ScaleCommand.h"

void ScaleCommand::Do()
{
	targetObject->SetScale(newScale);
}

void ScaleCommand::Undo()
{
	targetObject->SetScale(oldScale);
}
