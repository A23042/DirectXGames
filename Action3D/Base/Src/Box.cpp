#include "Box.h"
#include "Player.h"
// �����Ȃ�Box�I�u�W�F�N�g

Box::Box(VECTOR3 size, VECTOR3 rot)
{
	//SetTag("STAGEOBJ");
	mesh = new CFbxMesh();
	mesh->Load("Data/Object/box00.mesh");

	meshCol = new MeshCollider();
	meshCol->MakeFromMesh(mesh);
	
	vPos = VECTOR3(size.x / 2, size.y / 2, size.z / 2);
	transform.scale = size;

	// ��]�p�����W�A���ɕϊ����A��]�s����쐬
	transform.rotation.x += rot.x / 180.0f * XM_PI;
	transform.rotation.y += rot.y / 180.0f * XM_PI;
	transform.rotation.z += rot.z / 180.0f * XM_PI;

	rotationMatrix = XMMatrixRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z);

	pushVec = VECTOR3(0, 0, 0);
	HitPoint = VECTOR3(0, 0, 0);

	pObj.e = 0.9f;	// �����W��	1�Ō����Ȃ�
	pObj.f = 1.0f;	// ���C		1�Ō����Ȃ�
	refVec = VECTOR3(0, 0, 0);
}

void Box::Update()
{
	std::list<Player*> playeres =
		ObjectManager::FindGameObjects<Player>();
	for (Player* player : playeres) {
		refVec = VECTOR3(0, 0, 0);
		CubeSize(vPos.x, vPos.y, vPos.z);		// �����̂̃T�C�Y�ƈʒu
		pushVec = HitSphereToCubeplane(player->sphere, refVec);	// ��->��->���_�̏Փ˔���
		player->PushVec(-pushVec, refVec);	// �v���C���[���߂荞�񂾗ʂ������ǂ�

		/*ImGui::Begin("HitPoint");
		ImGui::InputFloat("X", &HitPoint.x);
		ImGui::InputFloat("Y", &HitPoint.y);
		ImGui::InputFloat("Z", &HitPoint.z);
		ImGui::End();*/
	}
}

void Box::CubeSize(float x, float y, float z)
{
	// �����̂̊e���_���W
	ten[0] = VECTOR3(x, y, -z);
	ten[1] = VECTOR3(x, -y, -z);
	ten[2] = VECTOR3(-x, -y, -z);
	ten[3] = VECTOR3(-x, y, -z);
	ten[4] = VECTOR3(x, y, z);
	ten[5] = VECTOR3(x, -y, z);
	ten[6] = VECTOR3(-x, -y, z);
	ten[7] = VECTOR3(-x, y, z);

	// ��]������e�X�g
	//transform.rotation.y += 1.0f / 180.0f * XM_PI;
	
	// ��]���ڗ�����
	rotationMatrix = XMMatrixRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z);

	// �e���_�ɉ�]�s����|����
	for (int i = 0; i < 8; i++) {
		ten[i] *= rotationMatrix;
	}

	// transform.position���e���_�ɉ��Z
	for (int i = 0; i < 8; i++) {
		ten[i] += transform.position;
	}

	// �e�ӂ̒��_�p�[�c
	int edgePoint[12][2] = {
		{0, 1}, {1, 2}, {2, 3}, {3, 0},//���ʁF�E�A�@���A�@���A�@��
		{0, 4}, {1, 5}, {2, 6}, {3, 7},//���ʁF�E��A�E���A�����A����
		{4, 5}, {5, 6}, {6, 7}, {7, 4} //�w�ʁF�E�A�@���A�@���A�@���A
	};

	// �ӂ̒������P�ō쐬
	for (int i = 0; i < 12; i++) {
		//	gpt				�_�@���_�p�[�c�̍�				���_�p�[�c�̉E
		//edge[i] = normalize(ten[edgePoint[i][1]] - ten[edgePoint[i][0]]);
		edge[i] = ten[edgePoint[i][1]] - ten[edgePoint[i][0]];
	}

	int planePoit[6][3] = {
		// �@���x�N�g�������߂邽�߂̕Ӄx�N�g��2�Ƌ��ʂ̒��_ 
		//�@�@���ʁ@�@�@�@�E�@�@�@�@�@�@���@�@�@�@�@���
			{2, 0, 1}, {1, 4, 5}, {6, 3, 2}, {5, 4, 6},
		//�@�@��@�@�@�@�@��
			{7, 0, 3}, {2, 1, 5}
	};

	// �ʂ̖@���ƒ萔�̌v�Z
	for (int i = 0; i < 6; i++) {
		VECTOR3 v1 = ten[planePoit[i][0]] - ten[planePoit[i][1]];
		VECTOR3 v2 = ten[planePoit[i][2]] - ten[planePoit[i][1]];
		plane[i] = normalize(cross(v1, v2));	// ���ʂ̖@��	
		d[i] = dot(-plane[i], ten[planePoit[i][1]]);// ���ʂ̒萔
	}
	return ;
}

// �ʂƂ̏Փ�
VECTOR3 Box::HitSphereToCubeplane(Sphere& sphere, VECTOR3 &refVec)
{
	pushVec = VECTOR3(0, 0, 0);

	// ���̒��S�_����e���_�ւ̃x�N�g��
	for (int i = 0; i < 8; i++) {
		pt[i] = sphere.center - ten[i];
	}

	int TptPoint[12] = {
		{0}, {1}, {2}, {3},
		{0}, {1}, {2}, {3},
		{4}, {5}, {6}, {7}
	};
	// ���̒��S�_����ӂɐ��������낵���Ƃ��ɕӂ͈͓̔��ɂ��邩�ǂ���
	// 1~0���͈͓�
	for (int i = 0; i < 12; i++) {
		Tpt[i] = dot(edge[i], pt[TptPoint[i]]) / edge[i].LengthSquare();
	}

	int pair[6][2] = {
		{0, 1}, {0, 4}, {2, 6},
		{8, 9}, {3, 4}, {1, 5}
	};

	// ���ʂƂ̋����v�Z
	for (int i = 0; i < 6; i++) {
		distance[i] = abs(dot(plane[i], sphere.center) + d[i]) / plane[i].Length();

		// �Փ˂��Ă����炠�Ƃ̋����v�Z���Ȃ�
		if (distance[i] <= sphere.radius) {
			// �������ʂɏՓ˂��Ă�����ӂɐ��������낹�邩
			if (Tpt[pair[i][0]] >= 0 && Tpt[pair[i][0]] <= 1 && Tpt[pair[i][1]] >= 0 && Tpt[pair[i][1]] <= 1) {
				HitPoint = sphere.center - plane[i] * distance[i];	// �Փ˓_
				refVec = ReflectionVec(sphere, plane[i]);	// ���̂𔽎˂�����
				pushVec = plane[i] * (sphere.radius - distance[i]);	// �߂荞�݂��������邽�߂̌v�Z
				return pushVec;
			}
		}
	}

	HitSphereToCubeEdge(sphere, refVec);
}

// �ӂƂ̏Փ�
VECTOR3 Box::HitSphereToCubeEdge(Sphere& sphere, VECTOR3& refVec)
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
	for (int i = 0; i < 12; i++) {
		distanceV[i] = edge[i] * dot(edge[i], pt[TptPoint[i]]) / edge[i].LengthSquare() - pt[TptPoint[i]];
		if (distanceV[i].Length() <= sphere.radius) {
			//�@���������낹�邩
			if (0 <= Tpt[i] && Tpt[i] <= 1) {
				//VECTOR3 vNormal = normalize(plane[pair[i][0]] + plane[pair[i][1]]) / 2;	// �ӂ̖@���x�N�g��
				VECTOR3 vNormal = normalize(distanceV[i]);	// �ӂ̖@���x�N�g��

				HitPoint = sphere.center - vNormal * distanceV[i].Length();	// �Փ˓_
				refVec = ReflectionVec(sphere, vNormal);
				pushVec = vNormal * (sphere.radius - distanceV[i].Length());
				return pushVec;
			}
		}
	}
	
	HitSphereToCubeVertices(sphere, refVec);
	return VECTOR3();
}

// ���_�Ƃ̏Փ�
VECTOR3 Box::HitSphereToCubeVertices(Sphere& sphere, VECTOR3& refVec)
{
	for (int i = 0; i < 8; i++) {
		if (pt[i].Length() < sphere.radius) {
			refVec = ReflectionVec(sphere, normalize(pt[i]));
			pushVec = normalize(ten[i] - sphere.center) * (sphere.radius - pt[i].Length());
			return pushVec;
		}
	}

	return VECTOR3();
}

// ���˂����邽�߂̊֐�
// ���������Ƃ��̖@���x�N�g�����󂯎�蒵�˕Ԃ�x�N�g�����v�Z����
VECTOR3 Box::ReflectionVec(Sphere& sphere, VECTOR3 normal)
{
	// �@�������ɔ����W����������
	VECTOR3 refNormal = dot(sphere.velocity, normal) * normal ;
	VECTOR3 refSessen = sphere.velocity - refNormal;
	VECTOR3 b = -refNormal * pObj.e + refSessen * pObj.f;
	// ���Ԃ̏C��
	// ���ߍ��݂�����->����	�Z
	// ����->���ߍ��݉���		�~
	return VECTOR3(b);
}

Box::~Box()
{
	if (mesh != nullptr) {
		delete mesh;
		mesh = nullptr;
	}
}

