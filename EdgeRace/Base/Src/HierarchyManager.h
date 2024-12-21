#pragma once
#include "Object3D.h"

namespace HierarchyManager
{
	/// <summary>
	/// �q�G�����L�[�ǉ�����
	/// </summary>
	/// <param name="obj">�ǉ�����I�u�W�F�N�g</param>
	void AddHierarchy(Object3D* obj);
	/// <summary>
	/// �q�G�����L�[����폜����DestoryMe()����
	/// </summary>
	/// <param name="obj">�폜����I�u�W�F�N�g</param>
	void RemoveHierarchy(Object3D* obj);
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