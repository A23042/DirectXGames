// 2024.12.20 S.Matsunaga
// 回転用コマンドクラス
#pragma once

#include "CommandBase.h"
#include "Object3D.h"

class RotationCommand : public CommandBase
{
public:
	RotationCommand(std::list<Object3D*> targetObj, std::list<VECTOR3> oldRot);
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
	std::list<Object3D*> targetObject;
	std::list<VECTOR3> oldRotation;
	std::list<VECTOR3> newRotation;
};