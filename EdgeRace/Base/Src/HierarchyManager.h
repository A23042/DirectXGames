#pragma once
#include "Object3D.h"

namespace HierarchyManager
{
	/// <summary>
	/// ヒエラルキー追加する
	/// </summary>
	/// <param name="obj">追加するオブジェクト</param>
	void AddHierarchy(Object3D* obj);
	/// <summary>
	/// ヒエラルキーから削除してDestoryMe()する
	/// </summary>
	/// <param name="obj">削除するオブジェクト</param>
	void RemoveHierarchy(Object3D* obj);
	/// <summary>
	/// ヒエラルキーをクリアする
	/// </summary>
	void ClearHierarchy();
	/// <summary>
	/// ヒエラルキーの中身をすべて返す
	/// </summary>
	/// <returns>ヒエラルキーのリスト</returns>
	std::list<Object3D*> GetHierarchyList();
}