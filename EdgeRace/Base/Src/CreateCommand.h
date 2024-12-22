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
	
	/*����Ώۂ̏��i�[�p*/
	list<VECTOR3> pos;
	list<VECTOR3> rot;
	list<VECTOR3> scale;
};

template<class C>
inline CreateCommand<C>::CreateCommand(list<C*> targetObj) : targetObject(targetObj)
{
	// list�z��ɏ��Ԃɂ��ꂼ��̏����i�[����
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

// Ctrl+Z�ŏ����ꂽ�I�u�W�F�N�g�̐���
template<class C>
inline void CreateCommand<C>::Do()
{
	list<Object3D*> objList = HierarchyManager::GetHierarchyList();
	//list<C*> tempList = {};
	/*List�z��̎Q�Ɨp�C�e���[�^*/
	auto posItr = pos.begin();
	auto rotItr = rot.begin();
	auto scaleItr = scale.begin();
	for (Object3D* obj : targetObject)
	{
		// �q�G�����L�[�ɑ��݂��Ȃ���ΐ�������
		if (find(objList.begin(), objList.end(), obj) == objList.end())
		{
			/*C* temp = new C();
			temp->pObj.center = *posItr;
			temp->SetPosition(*posItr);
			temp->SetRotation(*rotItr);
			temp->SetScale(*scaleItr);
			tempList.push_back(temp);*/
			ObjectManager::SetVisible(obj, true);
			HierarchyManager::AddHierarchy(obj);	// �������ꂽ�I�u�W�F�N�g�̓q�G�����L�[�ɒǉ�����
		}
		posItr++;
		rotItr++;
		scaleItr++;
	}
	// �������ꂽ�I�u�W�F�N�g������Α���ΏۃI�u�W�F�N�g�����ւ���
	/*if (tempList.size() > 0)
	{
		targetObject.clear();
		targetObject = tempList;
	}*/
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
