// 2024.12.20 S.Matsunaga
// �q�G�����L�[�Ǘ��N���X
// �}�b�v�G�f�B�^�V�[���Ŏg�p����
#pragma once
#include "Object3D.h"

namespace HierarchyManager
{
	/// <summary>
	/// �q�G�����L�[�ǉ�����
	/// </summary>
	/// <param name="obj">�ǉ�����I�u�W�F�N�g</param>
	void AddHierarchy(Object3D* obj);
	void AddHierarchy(std::list<Object3D*> obj);	// list�Ή�

	/// <summary>
	/// �q�G�����L�[����폜����DestoryMe()����
	/// </summary>
	/// <param name="obj">�폜����I�u�W�F�N�g</param>
	void RemoveHierarchy(Object3D* obj);
	void RemoveHierarchy(std::list<Object3D*> obj);

	/// <summary>
	/// �q�G�����L�[���N���A����
	/// </summary>
	void ClearHierarchy();

	/// <summary>
	/// �q�G�����L�[�̒��g�����ׂĕԂ�
	/// </summary>
	/// <returns>�q�G�����L�[�̃��X�g</returns>
	std::list<Object3D*> GetHierarchyList();
}