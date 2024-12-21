// 2024.12.20 S.Matsunaga
// �I�u�W�F�N�g�����R�}���h
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
	// Ctrl+Z�ŏ����ꂽ�I�u�W�F�N�g�̐���
	/*
	auto it = find(HierarchyManager::GetHierarchyList().begin(), HierarchyManager::GetHierarchyList().end(), targetObject);
	if(it == HierarchyManager::GetHierarchyList().end())
	{
		C* temp = new C();
		temp = targetObject;
	}
	*/
}

// cpp�ɏ����ƃG���[����
template<class C>
inline void CreateCommand<C>::Undo()
{
	HierarchyManager::RemoveHierarchy(targetObject);
}
