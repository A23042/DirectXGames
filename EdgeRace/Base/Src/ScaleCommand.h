// 2024.12.20 S.Matsunaga
// �X�P�[���R�}���h�N���X
#pragma once
#include "CommandBase.h"
#include "Object3D.h"

class ScaleCommand : public CommandBase
{
public:
	ScaleCommand(list<Object3D*> targetObj, list<VECTOR3> oldScale);
	/// <summary>
	/// �R�}���h���s
	/// �^����ꂽ�T�C�Y��������
	/// </summary>
	void Do() override;
	/// <summary>
	/// �ۑ�����Ă�Â��T�C�Y������
	/// </summary>
	void Undo() override;
private:
	list<Object3D*> targetObject;
	list<VECTOR3> newScale;
	list<VECTOR3> oldScale;
};