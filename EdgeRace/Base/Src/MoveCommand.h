// 2024.12.19 S.Matsunaga
// �ړ�����p�R�}���h�N���X
#pragma once
#include "CommandBase.h"
#include "Object3D.h"

class MoveCommand : public CommandBase
{
public:
	MoveCommand(list<Object3D*> targetObj, list<VECTOR3> oldPos);
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
	list<Object3D*> targetObject = {};
	list<VECTOR3> newPosition;
	list<VECTOR3> oldPosition;
};