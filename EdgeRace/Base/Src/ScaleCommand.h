// 2024.12.20 S.Matsunaga
// �X�P�[���R�}���h�N���X
#pragma once
#include "CommandBase.h"
#include "Object3D.h"

class ScaleCommand : public CommandBase
{
public:
	ScaleCommand(Object3D* targetObj, VECTOR3 oldScale) : targetObject(targetObj), newScale(targetObj->Scale()), oldScale(oldScale) {};
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
	Object3D* targetObject = nullptr;
	VECTOR3 newScale = VECTOR3();
	VECTOR3 oldScale = VECTOR3();
};