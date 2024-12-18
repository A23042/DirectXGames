#pragma once
/// <summary>
/// �����̂̍쐬
/// �T�C�Y���璸�_�Ӗʂ����߂�
/// �e�ʕӒ��_�Ƌ��̂̏Փ˔�����s��
/// </summary>
/// <author>S.Matsunaga</author>

#include "Object3D.h"
#include "BoxCollisionBase.h"

class Box : public BoxCollisionBase {
public:
	Box(VECTOR3 size = VECTOR3(1, 1, 1), VECTOR3 rot = VECTOR3(0, 0, 0));
	~Box();
	void Start() override;
	void Update() override;
	void Draw() override;

#if 0
	/// <summary>
	/// Box�̒��_�A�ӂ̃x�N�g���A���ʂ̖@�������߂�
	/// </summary>
	/// <param name="x">X���̕ӂ̒���</param>
	/// <param name="y">Y���̕ӂ̒���</param>
	/// <param name="z">Z���̕ӂ̒���</param>
	void CubeSize(float x = 0, float y = 0, float z = 0);
	
	/// <summary>
	/// EditScene�őI����Ԃ�؂�ւ���
	/// �Ăяo�����ƌ��݂̑I����Ԃ�ς���
	/// </summary>
	void SetSelect() {
		if (editObj.isSelect == true)
		{
			editObj.isSelect = false;
		}
		else
		{
			editObj.isSelect = true;
		}
	}
	
	/// <summary>
	/// EditScene�őI������Ă��邩
	/// </summary>
	/// <returns>�I������Ă�����true</returns>
	bool IsSelect() { return editObj.isSelect; }

	// �����Ԃ��x�N�g����Ԃ���������VECTOR3
	
	/// <summary>
	/// ���̂ƕ��ʂ̏Փ˔���
	/// �Փ˂��Ă�����߂荞�݉����x�N�g����Ԃ�
	/// �Փ˂��Ă��Ȃ���ΕӂƂ̏Փ˔���Ɉڂ�
	/// </summary>
	/// <param name="pOgj">�Փ˔������鋅�̂̍\����</param>
	/// <param name="refVec">���˃x�N�g��</param>
	/// <returns>�߂荞�݉����x�N�g��</returns>
	VECTOR3 HitSphereToCubeplane(PhysicsObject& pOgj, VECTOR3 &refVec) override;

	/// <summary>
	/// ���̂ƕӂƂ̏Փ˔���
	/// �Փ˂��Ă�����߂荞�݉����x�N�g����Ԃ�
	/// �Փ˂��Ă��Ȃ���Β��_�Ƃ̏Փ˔���Ɉڂ�
	/// </summary>
	/// <param name="pOgj">�Փ˔������鋅�̂̍\����</param>
	/// <param name="refVec">���˃x�N�g��</param>
	/// <returns>�߂荞�݉����x�N�g��</returns>
	VECTOR3 HitSphereToCubeEdge(PhysicsObject& pOgj, VECTOR3& refVec);
	
	/// <summary>
	/// ���̂ƒ��_�̏Փ˔���
	/// �Փ˂��Ă�����߂荞�݉����x�N�g����Ԃ�
	/// �Փ˂��Ă��Ȃ����VECTOR3(0,0,0)��Ԃ�
	/// </summary>
	/// <param name="pOgj">�Փ˔������鋅�̂̍\����</param>
	/// <param name="refVec">���˃x�N�g��</param>
	/// <returns>�߂荞�݉����x�N�g��</returns>
	VECTOR3 HitSphereToCubeVertices(PhysicsObject& pOgj, VECTOR3& refVec);

	// ���˕Ԃ�x�N�g���̌v�Z
	virtual VECTOR3 ReflectionVec(PhysicsObject& pOgj, VECTOR3 normal);
	
	/// <summary>
	/// ���̂�AABB�ŊȈՓI�ȏՓ˔���
	/// �Փ˂��Ă�����HitSphereCubeplane���񂷂悤�ɂ���
	/// �Փ˂��Ă��Ă��Փ˓_�┽�˃x�N�g���Ȃǂ��v�Z���Ȃ�
	/// </summary>
	/// <param name="pOgj">�Փ˔������鋅�̂̍\����</param>
	/// <returns>�Փ˂��Ă�����true</returns>
	virtual bool CheckSphereAABBCollision(PhysicsObject& pOgj) override;

protected:
	VECTOR3 vertex[8] = {};		// ���_
	CSprite* spr;
	VECTOR3 vPos = {};

//private:
	VECTOR3 normal[6] = {};		// �@��
	VECTOR3 plane[6] = {};		// ���ʂ̖@��	
	VECTOR3 v[12] = {};			// ��
	VECTOR3 edge[12] = {};		// �Ӄx�N�g��
	VECTOR3 pt[8] = {};			// �e���_����v���C���[�ւ̃x�N�g��
	VECTOR3 distanceV[12] = {};	// �e�ӂ���v���C���[�ւ̃x�N�g��

	VECTOR3 pushVec;
	VECTOR3 HitPoint;
	VECTOR3 refVec;

	VECTOR3 min;  // AABB�̍ŏ��_
	VECTOR3 max;  // AABB�̍ő�_

	float d[6] = {};	// ���ʂ̒萔
	float distance[6] = {};	// �ʂƂ̋���
	float Tpt[12] = {};

	XMMATRIX rotationMatrix;

	bool isStart = false;
#endif
private:
	Object3D* child = nullptr;
};