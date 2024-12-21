// 2024.12.20 S.Matsunaga
// �I�u�W�F�N�g�폜�R�}���h
#pragma once

#include "CommandBase.h"
#include "Object3D.h"
#include "HierarchyManager.h"

template<class C>
class DeleteCommand : public CommandBase
{
public:
	DeleteCommand(C* targetObj) : targetObject(targetObj) {};
	void Do() override;
	void Undo() override;
private:
	C* targetObject;
	VECTOR3 pos;
	VECTOR3 rot;
	VECTOR3 scale;
};

template<class C>
inline void DeleteCommand<C>::Do()
{
	HierarchyManager::RemoveHierarchy(targetObject);
}

template<class C>
inline void DeleteCommand<C>::Undo()
{
	// �폜���ꂽ�I�u�W�F�N�g�̐���
}
