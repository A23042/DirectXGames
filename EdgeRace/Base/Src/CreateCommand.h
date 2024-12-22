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
	CreateCommand(list<C*> targetObj);	// 複数操作時のコンストラクタ
	CreateCommand(C* targetObj);		// 単一操作時のコンストラクタ
	~CreateCommand() {};
	void Do() override;
	void Undo() override;
private:
	list<C*> targetObject;	// 操作対象オブジェクト
};

template<class C>
inline CreateCommand<C>::CreateCommand(list<C*> targetObj) : targetObject(targetObj)
{
}

template<class C>
inline CreateCommand<C>::CreateCommand(C* targetObj)
{
	targetObject.push_back(targetObj);
}

// Ctrl+Zで消されたオブジェクトの生成
template<class C>
inline void CreateCommand<C>::Do()
{
	list<Object3D*> objList = HierarchyManager::GetHierarchyList();
	for (Object3D* obj : targetObject)
	{
		if (find(objList.begin(), objList.end(), obj) == objList.end())
		{
			ObjectManager::SetVisible(obj, true);
			HierarchyManager::AddHierarchy(obj);	// 生成されたオブジェクトはヒエラルキーに追加する
		}
	}
}

// オブジェクト削除
template<class C>
inline void CreateCommand<C>::Undo()
{
	for (Object3D* obj : targetObject)
	{
		HierarchyManager::RemoveHierarchy(obj);
		ObjectManager::SetVisible(obj, false);
	}
}
