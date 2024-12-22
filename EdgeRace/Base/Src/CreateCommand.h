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
	
	/*操作対象の情報格納用*/
	list<VECTOR3> pos;
	list<VECTOR3> rot;
	list<VECTOR3> scale;
};

template<class C>
inline CreateCommand<C>::CreateCommand(list<C*> targetObj) : targetObject(targetObj)
{
	// list配列に順番にそれぞれの情報を格納する
	for (Object3D* obj : targetObj)
	{
		pos.push_back(obj->Position());
		rot.push_back(obj->Rotation());
		scale.push_back(obj->Scale());
	}
}

template<class C>
inline CreateCommand<C>::CreateCommand(C* targetObj)
{
	targetObject.push_back(targetObj);
	pos.push_back(targetObj->Position());
	rot.push_back(targetObj->Rotation());
	scale.push_back(targetObj->Scale());
}

// Ctrl+Zで消されたオブジェクトの生成
template<class C>
inline void CreateCommand<C>::Do()
{
	list<Object3D*> objList = HierarchyManager::GetHierarchyList();
	//list<C*> tempList = {};
	/*List配列の参照用イテレータ*/
	auto posItr = pos.begin();
	auto rotItr = rot.begin();
	auto scaleItr = scale.begin();
	for (Object3D* obj : targetObject)
	{
		// ヒエラルキーに存在しなければ生成する
		if (find(objList.begin(), objList.end(), obj) == objList.end())
		{
			/*C* temp = new C();
			temp->pObj.center = *posItr;
			temp->SetPosition(*posItr);
			temp->SetRotation(*rotItr);
			temp->SetScale(*scaleItr);
			tempList.push_back(temp);*/
			ObjectManager::SetVisible(obj, true);
			HierarchyManager::AddHierarchy(obj);	// 生成されたオブジェクトはヒエラルキーに追加する
		}
		posItr++;
		rotItr++;
		scaleItr++;
	}
	// 生成されたオブジェクトがあれば操作対象オブジェクトを入れ替える
	/*if (tempList.size() > 0)
	{
		targetObject.clear();
		targetObject = tempList;
	}*/
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
