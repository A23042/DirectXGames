// 2024.12.29 S.Matsunaga 
// �}�E�X�̃h���b�O���ꂽ�͈͂̒��Ńt���X�^��(������)���쐬���A�t���X�^���̒��ɃI�u�W�F�N�g�̃o�E���f�B���O�{�b�N�X�������Ă��邩���肷��
#pragma once
#include "BoxCollisionBase.h"

namespace Frustum
{
	/// <summary>
	/// �}�E�X�̃h���b�O�͈͂Ńt���X�^�����쐬����
	/// </summary>
	/// <param name="startPos">�h���b�O�J�n�ʒu</param>
	/// <param name="endPos">�h���b�O�I���ʒu</param>
	void CreateFrustum(POINT startPos, POINT endPos);
	/// <summary>
	/// �쐬�����t���X�^���̒��Ƀo�E���f�B���O�{�b�N�X�����S�ɓ����Ă��邩
	/// </summary>
	/// <returns>�t���X�^���̒��ɓ����Ă���I�u�W�F�N�g</returns>
	std::list<Object3D*> CheckAABB();
}