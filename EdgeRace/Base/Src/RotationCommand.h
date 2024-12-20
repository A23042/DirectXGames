// 2024.12.20 S.Matsunaga
// ��]�p�R�}���h�N���X
#pragma once

#include "CommandBase.h"
#include "Object3D.h"

class RotationCommand : public CommandBase
{
public:
	RotationCommand(Object3D* targetObj, VECTOR3 oldRot) : targetObject(targetObj), newRotation(targetObj->Rotation()), oldRotation(oldRot) {};
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
	Object3D* targetObject = nullptr;
	VECTOR3 oldRotation = VECTOR3();
	VECTOR3 newRotation = VECTOR3();
};