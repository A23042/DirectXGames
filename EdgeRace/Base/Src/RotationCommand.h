// 2024.12.20 S.Matsunaga
// ��]�p�R�}���h�N���X
#pragma once

#include "CommandBase.h"
#include "Object3D.h"

class RotationCommand : public CommandBase
{
public:
	RotationCommand(std::list<Object3D*> targetObj, std::list<VECTOR3> oldRot);
	/// <summary>
	/// �R�}���h���s
	/// �^����ꂽ��]��������
	/// </summary>
	void Do() override;
	/// <summary>
	/// �ۑ�����Ă�Â���]������
	/// </summary>
	void Undo() override;
private:
	std::list<Object3D*> targetObject;
	std::list<VECTOR3> oldRotation;
	std::list<VECTOR3> newRotation;
};