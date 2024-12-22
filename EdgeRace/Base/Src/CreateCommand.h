// 2024.12.22 S.Matsunaga
// �I�u�W�F�N�g�����R�}���h
#pragma once

#include "CommandBase.h"
#include "Object3D.h"
#include "HierarchyManager.h"

template<class C>
class CreateCommand : public CommandBase
{
public:
	CreateCommand(list<C*> targetObj);	// �������쎞�̃R���X�g���N�^
	CreateCommand(C* targetObj);		// �P�ꑀ�쎞�̃R���X�g���N�^
	~CreateCommand() {};
	void Do() override;
	void Undo() override;
private:
	list<C*> targetObject;	// ����ΏۃI�u�W�F�N�g
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

// Ctrl+Z�ŏ����ꂽ�I�u�W�F�N�g�̐���
template<class C>
inline void CreateCommand<C>::Do()
{
	list<Object3D*> objList = HierarchyManager::GetHierarchyList();
	for (Object3D* obj : targetObject)
	{
		if (find(objList.begin(), objList.end(), obj) == objList.end())
		{
			ObjectManager::SetVisible(obj, true);
			HierarchyManager::AddHierarchy(obj);	// �������ꂽ�I�u�W�F�N�g�̓q�G�����L�[�ɒǉ�����
		}
	}
}

// �I�u�W�F�N�g�폜
template<class C>
inline void CreateCommand<C>::Undo()
{
	for (Object3D* obj : targetObject)
	{
		HierarchyManager::RemoveHierarchy(obj);
		ObjectManager::SetVisible(obj, false);
	}
}
