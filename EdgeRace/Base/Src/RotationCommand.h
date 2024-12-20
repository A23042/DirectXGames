// 2024.12.20 S.Matsunaga
// 回転用コマンドクラス
#pragma once

#include "CommandBase.h"
#include "Object3D.h"

class RotationCommand : public CommandBase
{
public:
	RotationCommand(Object3D* targetObj, VECTOR3 oldRot) : targetObject(targetObj), newRotation(targetObj->Rotation()), oldRotation(oldRot) {};
	/// <summary>
	/// コマンド実行
	/// 与えられた回転を代入する
	/// </summary>
	void Do() override;
	/// <summary>
	/// 保存されてる古い回転を入れる
	/// </summary>
	void Undo() override;
private:
	Object3D* targetObject = nullptr;
	VECTOR3 oldRotation = VECTOR3();
	VECTOR3 newRotation = VECTOR3();
};