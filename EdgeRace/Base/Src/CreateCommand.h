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
	CreateCommand(list<C*> targetObj) : targetObject(targetObj) {};	// �������쎞�̃R���X�g���N�^
	CreateCommand(C* targetObj);		// �P�ꑀ�쎞�̃R���X�g���N�^
	~CreateCommand() {};
	/// <summary>
	/// ���X�g�ɂȂ���Ε\�����ăq�X�g���ɒǉ�
	/// </summary>
	void Do() override;
	/// <summary>
	/// ���X�g�ɂ���Δ�\���ɂ��ăq�X�g������폜
	/// </summary>
	void Undo() override;
private:
	list<C*> targetObject;	// ����ΏۃI�u�W�F�N�g
};

template<class C>
inline CreateCommand<C>::CreateCommand(C* targetObj)
{
	targetObject.push_back(targetObj);
}

template<class C>
inline void CreateCommand<C>::Do()
{
	list<Object3D*> objList = HierarchyManager::GetHierarchyList();	// �q�X�g���̎擾
	for (Object3D* obj : targetObject)
	{
		if (find(objList.begin(), objList.end(), obj) == objList.end())	// �q�X�g�����ɂȂ����
		{
			ObjectManager::SetVisible(obj, true);
			HierarchyManager::AddHierarchy(obj);	// �������ꂽ�I�u�W�F�N�g�̓q�G�����L�[�ɒǉ�����
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
