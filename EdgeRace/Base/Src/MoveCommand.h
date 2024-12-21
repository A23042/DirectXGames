// 2024.12.19 S.Matsunaga
// 移動操作用コマンドクラス
#pragma once
#include "CommandBase.h"
#include "Object3D.h"

class MoveCommand : public CommandBase
{
public:
	MoveCommand(list<Object3D*> targetObj, list<VECTOR3> oldPos);
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
	list<Object3D*> targetObject = {};
	list<VECTOR3> newPosition;
	list<VECTOR3> oldPosition;
};