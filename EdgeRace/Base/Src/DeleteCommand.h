// 2024.12.22 S.Matsunaga
// �I�u�W�F�N�g�폜�R�}���h
#pragma once

#include "CommandBase.h"
#include "Object3D.h"
#include "HierarchyManager.h"

template<class C>
class DeleteCommand : public CommandBase
{
public:
	DeleteCommand(list<C*> targetObj);	// �������쎞�̃R���X�g���N�^
	DeleteCommand(C* targetObj);		// �P�ꑀ�쎞�̃R���X�g���N�^
	~DeleteCommand() {};
	void Do() override;
	void Undo() override;
private:
	list<C*> targetObject;	// ����ΏۃI�u�W�F�N�g
};

template<class C>
inline DeleteCommand<C>::DeleteCommand(list<C*> targetObj) : targetObject(targetObj)
{
}

template<class C>
inline DeleteCommand<C>::DeleteCommand(C* targetObj)
{
	targetObject.push_back(targetObj);
}

// Ctrl+Z�Ő����I�u�W�F�N�g�̍폜
template<class C>
inline void DeleteCommand<C>::Do()
{
	for (Object3D* obj : targetObject)
	{
		HierarchyManager::RemoveHierarchy(obj);
		ObjectManager::SetVisible(obj, false);
	}
}

// �I�u�W�F�N�g����
template<class C>
inline void DeleteCommand<C>::Undo()
{
	list<Object3D*> objList = HierarchyManager::GetHierarchyList();
	for (Object3D* obj : targetObject)
	{
		// �q�G�����L�[�ɑ��݂��Ȃ���ΐ�������
		if (find(objList.begin(), objList.end(), obj) == objList.end())
		{
			ObjectManager::SetVisible(obj, true);
			HierarchyManager::AddHierarchy(obj);	// �������ꂽ�I�u�W�F�N�g�̓q�G�����L�[�ɒǉ�����
		}
	}
}
