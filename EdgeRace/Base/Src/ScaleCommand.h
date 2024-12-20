// 2024.12.20 S.Matsunaga
// スケールコマンドクラス
#pragma once
#include "CommandBase.h"
#include "Object3D.h"

class ScaleCommand : public CommandBase
{
public:
	ScaleCommand(Object3D* targetObj, VECTOR3 oldScale) : targetObject(targetObj), newScale(targetObj->Scale()), oldScale(oldScale) {};
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
	Object3D* targetObject = nullptr;
	VECTOR3 newScale = VECTOR3();
	VECTOR3 oldScale = VECTOR3();
};