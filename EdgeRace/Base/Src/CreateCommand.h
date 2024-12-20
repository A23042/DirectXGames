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

}

// cpp�ɏ����ƃG���[����
template<class C>
inline void CreateCommand<C>::Undo()
{
	HierarchyManager::RemoveHierarchy(targetObject);
}
