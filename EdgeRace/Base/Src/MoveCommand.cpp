#include "MoveCommand.h"

void MoveCommand::Do()
{
	// ���̍��W��ۑ����Ă����ĐV�������W�ɒu��������
	//oldPosition = targetObject->Position();
	targetObject->pObj.center = newPosition;
	targetObject->SetPosition(newPosition);
}

void MoveCommand::Undo()
{
	// �Â����W�ɒu��������
	targetObject->pObj.center = oldPosition;
	targetObject->SetPosition(oldPosition);
}
