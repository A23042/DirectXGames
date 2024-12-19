// 2024.12.19 S.Matsunaga
// �ړ�����p�R�}���h�N���X
#pragma once
#include "CommandBase.h"
#include "Object3D.h"

class MoveCommand : public CommandBase
{
public:
	MoveCommand(Object3D* targetObj, VECTOR3 oldPos) : targetObject(targetObj), newPosition(targetObj->Position()), oldPosition(oldPos) {};
	/// <summary>
	/// �R�}���h���s
	/// �^����ꂽ���W��������
	/// </summary>
	void Do() override;
	/// <summary>
	/// �ۑ�����Ă�Â����W������
	/// </summary>
	void Undo() override;
private:
	Object3D* targetObject = nullptr;
	VECTOR3 newPosition = VECTOR3();
	VECTOR3 oldPosition = VECTOR3();
};