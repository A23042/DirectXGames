#pragma once
#include "Object3D.h"
#include "Player.h"

class Box : public Object3D {
public:
	Box(float x, float y, float z, float rotX, float rotY, float rotZ);
	~Box();
	void Update() override;
	void CubeSize(float x, float y, float z);
	// �����Ԃ��x�N�g����Ԃ���������VECTOR3
	// ���ʂ̏Փ˔���
	VECTOR3 HitSphereToCubeplane(Sphere& sphere);
	// �ӂ̏Փ˔���
	VECTOR3 HitSphereToCubeEdge(Sphere& sphere);
	// ���_�̏Փ˔���
	VECTOR3 HitSphereToCubeVertices(Sphere& sphere);

	// ���˕Ԃ�x�N�g���̌v�Z(���r��)
	VECTOR3 ReflectionVec(Sphere& sphere, VECTOR3 normal);

private:
	VECTOR3 normal[6] = {};
	VECTOR3 ten[8] = {};
	VECTOR3 plane[6] = {};
	VECTOR3 v[12] = {};
	VECTOR3 edge[12] = {};
	VECTOR3 pt[8] = {};
	float d[6] = {};
	float distance[6] = {};
	VECTOR3 distanceV[12] = {};
	VECTOR3 vPos = {};
	float Tpt[12] = {};

	VECTOR3 pushVec;
	VECTOR3 HitPoint;

	XMMATRIX rotationMatrix;
};