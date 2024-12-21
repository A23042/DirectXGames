// 2024.12.20 S.Matsunaga
// オブジェクト生成コマンド
#pragma once

#include "CommandBase.h"
#include "Object3D.h"
#include "HierarchyManager.h"

template<class C>
class CreateCommand : public CommandBase
{
public:
	CreateCommand(C* targetObj) : targetObject(targetObj) {};
	~CreateCommand() {};
	void Do() override;
	void Undo() override;
private:
	C* targetObject;
	VECTOR3 pos;
	VECTOR3 rot;
	VECTOR3 scale;
};

template<class C>
inline void CreateCommand<C>::Do()
{
	// Ctrl+Zで消されたオブジェクトの生成
	/*
	auto it = find(HierarchyManager::GetHierarchyList().begin(), HierarchyManager::GetHierarchyList().end(), targetObject);
	if(it == HierarchyManager::GetHierarchyList().end())
	{
		C* temp = new C();
		temp = targetObject;
	}
	*/
}

// cppに書くとエラー発生
template<class C>
inline void CreateCommand<C>::Undo()
{
	HierarchyManager::RemoveHierarchy(targetObject);
}
