// 2024.12.29 S.Matsunaga 
// マウスのドラッグされた範囲の中でフラスタム(視錐台)を作成し、フラスタムの中にオブジェクトのバウンディングボックスが内包されているか判定する
#pragma once
#include "BoxCollisionBase.h"

namespace Frustum
{
	/// <summary>
	/// マウスのドラッグ範囲でフラスタムを作成する
	/// </summary>
	/// <param name="startPos">ドラッグ開始位置</param>
	/// <param name="endPos">ドラッグ終了位置</param>
	void CreateFrustum(POINT startPos, POINT endPos);
	/// <summary>
	/// 作成したフラスタムの中にバウンディングボックスが完全に内包されているか
	/// </summary>
	/// <returns>フラスタムの中に入っているオブジェクト</returns>
	std::list<Object3D*> CheckAABB();
}