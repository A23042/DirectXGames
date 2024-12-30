// 2024.12.22 S.Matsunaga
// オブジェクト削除コマンド
#pragma once

#include "CommandBase.h"
#include "Object3D.h"
#include "HierarchyManager.h"

template<class C>
class DeleteCommand : public CommandBase
{
public:
	DeleteCommand(list<C*> targetObj) : targetObject(targetObj) {};	// 複数操作時のコンストラクタ
	DeleteCommand(C* targetObj);		// 単一操作時のコンストラクタ
	~DeleteCommand() {};
	void Do() override;
	void Undo() override;
private:
	list<C*> targetObject;	// 操作対象オブジェクト
};

template<class C>
inline DeleteCommand<C>::DeleteCommand(C* targetObj)
{
	targetObject.push_back(targetObj);
}

template<class C>
inline void DeleteCommand<C>::Do()
{
	for (Object3D* obj : targetObject)
	{
		HierarchyManager::RemoveHierarchy(obj);
		ObjectManager::SetVisible(obj, false);
	}
}

template<class C>
inline void DeleteCommand<C>::Undo()
{
	list<Object3D*> objList = HierarchyManager::GetHierarchyList();
	for (Object3D* obj : targetObject)
	{
		// ヒエラルキーに存在しなければ生成する
		if (find(objList.begin(), objList.end(), obj) == objList.end())
		{
			ObjectManager::SetVisible(obj, true);
			HierarchyManager::AddHierarchy(obj);	// 生成されたオブジェクトはヒエラルキーに追加する
		}
	}
}
