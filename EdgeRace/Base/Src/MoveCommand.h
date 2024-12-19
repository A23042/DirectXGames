// 2024.12.19 S.Matsunaga
// 移動操作用コマンドクラス
#pragma once
#include "CommandBase.h"
#include "Object3D.h"

class MoveCommand : public CommandBase
{
public:
	MoveCommand(Object3D* targetObj, VECTOR3 oldPos) : targetObject(targetObj), newPosition(targetObj->Position()), oldPosition(oldPos) {};
	/// <summary>
	/// コマンド実行
	/// 与えられた座標を代入する
	/// </summary>
	void Do() override;
	/// <summary>
	/// 保存されてる古い座標を入れる
	/// </summary>
	void Undo() override;
private:
	Object3D* targetObject = nullptr;
	VECTOR3 newPosition = VECTOR3();
	VECTOR3 oldPosition = VECTOR3();
};