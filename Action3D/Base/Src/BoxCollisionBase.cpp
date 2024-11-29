#include "BoxCollisionBase.h"

BoxCollisionBase::BoxCollisionBase()
{
}

BoxCollisionBase::~BoxCollisionBase()
{
}

void BoxCollisionBase::Start()
{
	pObj.center = transform.position;
	vPos = transform.scale / 2;
	CubeSize(vPos.x, vPos.y, vPos.z);
	isStart = true;
}

void BoxCollisionBase::Update()
{
	transform.position = pObj.center;
}

void BoxCollisionBase::Draw()
{
	mesh->Render(transform.matrix());
}

void BoxCollisionBase::CubeSize(float x, float y, float z)
{
	// �����̂̊e���_���W
	vertex[0] = VECTOR3(x, y, -z);
	vertex[1] = VECTOR3(x, -y, -z);
	vertex[2] = VECTOR3(-x, -y, -z);
	vertex[3] = VECTOR3(-x, y, -z);
	vertex[4] = VECTOR3(x, y, z);
	vertex[5] = VECTOR3(x, -y, z);
	vertex[6] = VECTOR3(-x, -y, z);
	vertex[7] = VECTOR3(-x, y, z);


	// ��]���ڗ�����
	rotationMatrix = XMMatrixRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z);

	// �e���_�ɉ�]�s����|����
	for (int i = 0; i < 8; i++)
	{
		vertex[i] *= rotationMatrix;
	}

	// transform.position���e���_�ɉ��Z
	for (int i = 0; i < 8; i++)
	{
		vertex[i] += transform.position;
	}

	min = vertex[0];
	max = vertex[0];

	// AABB�̍ŏ��_�A�ő�_�����߂�
	//for (int i = 0; i < sizeof(vertex); i++)
	for (int i = 0; i < 8; i++)
	{
		max.x = fmax(max.x, vertex[i].x);
		max.y = fmax(max.y, vertex[i].y);
		max.z = fmax(max.z, vertex[i].z);

		min.x = fmin(min.x, vertex[i].x);
		min.y = fmin(min.y, vertex[i].y);
		min.z = fmin(min.z, vertex[i].z);
	}

	// �e�ӂ̒��_�p�[�c
	int edgePoint[12][2] = {
		{0, 1}, {1, 2}, {2, 3}, {3, 0},//���ʁF�E�A�@���A�@���A�@��
		{0, 4}, {1, 5}, {2, 6}, {3, 7},//���ʁF�E��A�E���A�����A����
		{4, 5}, {5, 6}, {6, 7}, {7, 4} //�w�ʁF�E�A�@���A�@���A�@���A
	};


	// �Ӄx�N�g���쐬
	for (int i = 0; i < 12; i++) {
		edge[i] = vertex[edgePoint[i][1]] - vertex[edgePoint[i][0]];
	}

	int planePoit[6][3] = {
		// �@���x�N�g�������߂邽�߂̕Ӄx�N�g��2�Ƌ��ʂ̒��_ 
		//���ʁ@�@�@�@�E�@�@�@�@���@�@�@�@�@���
		{2, 0, 1}, {1, 4, 5}, {6, 3, 2}, {5, 4, 6},
		//�@��@�@�@�@��
		{7, 0, 3}, {2, 1, 5}
	};

	// �ʂ̖@���ƒ萔�̌v�Z
	for (int i = 0; i < 6; i++)
	{
		VECTOR3 v1 = vertex[planePoit[i][0]] - vertex[planePoit[i][1]];
		VECTOR3 v2 = vertex[planePoit[i][2]] - vertex[planePoit[i][1]];
		plane[i] = normalize(cross(v1, v2));	// ���ʂ̖@��	
		d[i] = dot(-plane[i], vertex[planePoit[i][1]]);// ���ʂ̒萔
	}
	return;

}

VECTOR3 BoxCollisionBase::HitSphereToCubeplane(PhysicsObject& tObj, VECTOR3& refVec)
{
	pushVec = VECTOR3(0, 0, 0);

	// ���̒��S�_����e���_�ւ̃x�N�g��
	for (int i = 0; i < 8; i++)
	{
		pt[i] = tObj.center - vertex[i];
	}

	int TptPoint[12] = {
		{0}, {1}, {2}, {3},
		{0}, {1}, {2}, {3},
		{4}, {5}, {6}, {7}
	};

	// ���̒��S�_����ӂɐ��������낵���Ƃ��ɕӂ͈͓̔��ɂ��邩�ǂ���
	for (int i = 0; i < 12; i++)
	{
		Tpt[i] = dot(edge[i], pt[TptPoint[i]]) / edge[i].LengthSquare();
	}

	int pair[6][2] = {
		{0, 1}, {0, 4}, {2, 6},
		{8, 9}, {3, 4}, {1, 5}
	};

	// ���ʂƂ̋����v�Z
	for (int i = 0; i < 6; i++)
	{
		distance[i] = abs(dot(plane[i], tObj.center) + d[i]) / plane[i].Length();

		// �Փ˂��Ă����炠�Ƃ̋����v�Z���Ȃ�
		if (distance[i] <= tObj.radius)
		{
			// �������ʂɏՓ˂��Ă�����ӂɐ��������낹�邩
			if (Tpt[pair[i][0]] >= 0 && Tpt[pair[i][0]] <= 1 && Tpt[pair[i][1]] >= 0 && Tpt[pair[i][1]] <= 1)
			{
				HitPoint = tObj.center - plane[i] * distance[i];	// �Փ˓_
				refVec = ReflectionVec(tObj, plane[i]);	// ���̂𔽎˂�����
				pushVec = plane[i] * (tObj.radius - distance[i]);	// �߂荞�݂��������邽�߂̌v�Z
				return pushVec;	// �߂荞�݉����̗ʃx�N�g��
			}
		}
	}

	HitSphereToCubeEdge(tObj, refVec);

}

VECTOR3 BoxCollisionBase::HitSphereToCubeEdge(PhysicsObject& tObj, VECTOR3& refVec)
{
	int TptPoint[12] = {
	{0}, {1}, {2}, {3},
	{0}, {1}, {2}, {3},
	{4}, {5}, {6}, {7}
	};

	int pair[12][2] = {
		{0, 1}, {0, 5}, {0, 2}, {0, 4},
		{4, 1}, {1, 5}, {5, 2}, {2, 4},
		{3, 1}, {3, 5}, {3, 2}, {3, 4}
	};

	// �ӂƋ��Ƃ̋����v�Z
	for (int i = 0; i < 12; i++)
	{
		distanceV[i] = edge[i] * dot(edge[i], pt[TptPoint[i]]) / edge[i].LengthSquare() - pt[TptPoint[i]];
		if (distanceV[i].Length() <= tObj.radius) {
			//�@���������낹�邩
			if (0 <= Tpt[i] && Tpt[i] <= 1)
			{
				//VECTOR3 vNormal = normalize(plane[pair[i][0]] + plane[pair[i][1]]) / 2;	// �ӂ̖@���x�N�g��
				VECTOR3 vNormal = normalize(distanceV[i]);	// �ӂ̖@���x�N�g��

				HitPoint = tObj.center - vNormal * distanceV[i].Length();	// �Փ˓_
				refVec = ReflectionVec(tObj, vNormal);
				pushVec = vNormal * (tObj.radius - distanceV[i].Length());
				return pushVec;
			}
		}
	}

	HitSphereToCubeVertices(tObj, refVec);
	return VECTOR3();

}

VECTOR3 BoxCollisionBase::HitSphereToCubeVertices(PhysicsObject& tObj, VECTOR3& refVec)
{
	for (int i = 0; i < 8; i++)
	{
		if (pt[i].Length() < tObj.radius)
		{
			refVec = ReflectionVec(tObj, normalize(pt[i]));
			pushVec = normalize(vertex[i] - tObj.center) * (tObj.radius - pt[i].Length());
			return pushVec;
		}
	}

	return VECTOR3();

}

VECTOR3 BoxCollisionBase::ReflectionVec(PhysicsObject& tObj, VECTOR3 normal)
{
	// �@�������ɔ����W����������
// �@�������ɐ����ȃx�N�g���ɖ��C�W�����v�Z
	VECTOR3 refNormal = dot(tObj.velocity, normal) * normal;

	VECTOR3 refSessen = tObj.velocity - refNormal;

	// �Փ˂����ӂ��̃I�u�W�F�N�g�̔����W���Ɩ��C���l������
	float e2 = (this->pObj.e + tObj.e) / 2;
	float f2 = (this->pObj.f + tObj.f) / 2;

	VECTOR3 b = -refNormal * e2 + refSessen * (1 - f2);

	// ���Ԃ̏C��
	// ���ߍ��݂�����->����	�Z
	// ����->���ߍ��݉���		�~
	return VECTOR3(b);

}

bool BoxCollisionBase::CheckSphereAABBCollision(PhysicsObject& tObj)
{
	if (!isStart)
	{
		Start();
	}

	// ���̂̒��S����ł��߂�AABB�̒��_���擾����
	float x = std::fmax(min.x, std::fmin(tObj.center.x, max.x));
	float y = std::fmax(min.y, std::fmin(tObj.center.y, max.y));
	float z = std::fmax(min.z, std::fmin(tObj.center.z, max.z));

	// AABB�Ƌ��̂̂����Ƃ��߂��_�Ƃ̋����v�Z
	VECTOR3 closest = VECTOR3(x, y, z);
	float distance = sqrt(
		(closest.x - tObj.center.x) * (closest.x - tObj.center.x) +
		(closest.y - tObj.center.y) * (closest.y - tObj.center.y) +
		(closest.z - tObj.center.z) * (closest.z - tObj.center.z));

	// ���������a�������������true
	return distance <= tObj.radius;

}

bool BoxCollisionBase::CheckSphereAABBCollision(PhysicsObject& tObj, float distance)
{
	if (!isStart)
	{
		Start();
	}

	// ���̂̒��S����ł��߂�AABB�̒��_���擾����
	float x = std::fmax(min.x, std::fmin(tObj.center.x, max.x));
	float y = std::fmax(min.y, std::fmin(tObj.center.y, max.y));
	float z = std::fmax(min.z, std::fmin(tObj.center.z, max.z));

	// AABB�Ƌ��̂̂����Ƃ��߂��_�Ƃ̋����v�Z
	VECTOR3 closest = VECTOR3(x, y, z);
	float verDistance = sqrt(
		(closest.x - tObj.center.x) * (closest.x - tObj.center.x) +
		(closest.y - tObj.center.y) * (closest.y - tObj.center.y) +
		(closest.z - tObj.center.z) * (closest.z - tObj.center.z));

	// ���������a�������������true
	if (verDistance <= tObj.radius)
	{
		distance = verDistance;
		return true;
	}
	return false;
}
