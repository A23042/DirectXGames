#include "MoveCommand.h"

void MoveCommand::Do()
{
	// 元の座標を保存しておいて新しい座標に置き換える
	//oldPosition = targetObject->Position();
	targetObject->pObj.center = newPosition;
	targetObject->SetPosition(newPosition);
}

void MoveCommand::Undo()
{
	// 古い座標に置き換える
	targetObject->pObj.center = oldPosition;
	targetObject->SetPosition(oldPosition);
}
