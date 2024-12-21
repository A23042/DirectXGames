// 2024.12.20 S.Matsunaga
// スケールコマンドクラス
#pragma once
#include "CommandBase.h"
#include "Object3D.h"

class ScaleCommand : public CommandBase
{
public:
	ScaleCommand(list<Object3D*> targetObj, list<VECTOR3> oldScale);
	/// <summary>
	/// コマンド実行
	/// 与えられたサイズを代入する
	/// </summary>
	void Do() override;
	/// <summary>
	/// 保存されてる古いサイズを入れる
	/// </summary>
	void Undo() override;
private:
	list<Object3D*> targetObject;
	list<VECTOR3> newScale;
	list<VECTOR3> oldScale;
};