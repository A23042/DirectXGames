#pragma once
#include "Object3D.h"

class Box : public Object3D {
public:
	Box(VECTOR3 size = VECTOR3(0,0,0), VECTOR3 rot = VECTOR3(0,0,0));
	~Box();
	void Start() override;
	void Update() override;
	void CubeSize(float x = 0, float y = 0, float z = 0);
	// �����Ԃ��x�N�g����Ԃ���������VECTOR3
	// ���ʂ̏Փ˔���
	VECTOR3 HitSphereToCubeplane(PhysicsObject& pOgj, VECTOR3 &refVec) override;
	// �ӂ̏Փ˔���
	VECTOR3 HitSphereToCubeEdge(PhysicsObject& pOgj, VECTOR3& refVec);
	// ���_�̏Փ˔���
	VECTOR3 HitSphereToCubeVertices(PhysicsObject& pOgj, VECTOR3& refVec);

	// ���˕Ԃ�x�N�g���̌v�Z(���r��)
	virtual VECTOR3 ReflectionVec(PhysicsObject& pOgj, VECTOR3 normal);

private:
	VECTOR3 normal[6] = {};		// �@��
	VECTOR3 ten[8] = {};		// ���_
	VECTOR3 plane[6] = {};		// �@��
	VECTOR3 v[12] = {};			// ��
	VECTOR3 edge[12] = {};		// ��
	VECTOR3 pt[8] = {};			// �e���_����v���C���[�ւ̃x�N�g��
	VECTOR3 distanceV[12] = {};	// �e���_����v���C���[�ւ̋���
	VECTOR3 vPos = {};			// 

	VECTOR3 pushVec;
	VECTOR3 HitPoint;
	VECTOR3 refVec;

	float d[6] = {};
	float distance[6] = {};
	float Tpt[12] = {};

	XMMATRIX rotationMatrix;
};