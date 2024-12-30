// 2024.12.22 S.Matsunaga
// オブジェクト生成コマンド
#pragma once

#include "CommandBase.h"
#include "Object3D.h"
#include "HierarchyManager.h"

template<class C>
class CreateCommand : public CommandBase
{
public:
	CreateCommand(list<C*> targetObj) : targetObject(targetObj) {};	// 複数操作時のコンストラクタ
	CreateCommand(C* targetObj);		// 単一操作時のコンストラクタ
	~CreateCommand() {};
	/// <summary>
	/// リストになければ表示してヒストリに追加
	/// </summary>
	void Do() override;
	/// <summary>
	/// リストにあれば非表示にしてヒストリから削除
	/// </summary>
	void Undo() override;
private:
	list<C*> targetObject;	// 操作対象オブジェクト
};

template<class C>
inline CreateCommand<C>::CreateCommand(C* targetObj)
{
	targetObject.push_back(targetObj);
}

template<class C>
inline void CreateCommand<C>::Do()
{
	list<Object3D*> objList = HierarchyManager::GetHierarchyList();	// ヒストリの取得
	for (Object3D* obj : targetObject)
	{
		if (find(objList.begin(), objList.end(), obj) == objList.end())	// ヒストリ内になければ
		{
			ObjectManager::SetVisible(obj, true);
			HierarchyManager::AddHierarchy(obj);	// 生成されたオブジェクトはヒエラルキーに追加する
		}
	}
}

template<class C>
inline void CreateCommand<C>::Undo()
{
	for (Object3D* obj : targetObject)
	{
		HierarchyManager::RemoveHierarchy(obj);
		ObjectManager::SetVisible(obj, false);
	}
}
