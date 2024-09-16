#include "Box.h"
#include "Player.h"

Box::Box(float x, float y, float z)
{
	SetTag("STAGEOBJ");
	mesh = new CFbxMesh();
	mesh->Load("Data/Object/box00.mesh");

	meshCol = new MeshCollider();
	meshCol->MakeFromMesh(mesh);

	vPos = VECTOR3(x / 2, y / 2, z / 2);

	ten[0] = VECTOR3(transform.position.x + vPos.x, transform.position.y + vPos.y, transform.position.z - vPos.z);
	ten[1] = VECTOR3(transform.position.x + vPos.x, transform.position.y - vPos.y, transform.position.z - vPos.z);
	ten[2] = VECTOR3(transform.position.x - vPos.x, transform.position.y - vPos.y, transform.position.z - vPos.z);
	ten[3] = VECTOR3(transform.position.x - vPos.x, transform.position.y + vPos.y, transform.position.z - vPos.z);
	ten[4] = VECTOR3(transform.position.x + vPos.x, transform.position.y + vPos.y, transform.position.z + vPos.z);
	ten[5] = VECTOR3(transform.position.x + vPos.x, transform.position.y - vPos.y, transform.position.z + vPos.z);
	ten[6] = VECTOR3(transform.position.x - vPos.x, transform.position.y - vPos.y, transform.position.z + vPos.z);
	ten[7] = VECTOR3(transform.position.x - vPos.x, transform.position.y + vPos.y, transform.position.z + vPos.z);

}

void Box::Update()
{

	std::list<Player*> playeres =
		ObjectManager::FindGameObjects<Player>();
	for (Player* player : playeres) {
		CubeSize(vPos.x, vPos.y, vPos.z);
		HitSphereToCubeplane(player);
		transform.position += pushVec;
	}
}

void Box::CubeSize(float x, float y, float z)
{
	// �����̂̊e���_���W
	ten[0] = VECTOR3(transform.position.x + vPos.x, transform.position.y + vPos.y, transform.position.z - vPos.z);
	ten[1] = VECTOR3(transform.position.x + vPos.x, transform.position.y - vPos.y, transform.position.z - vPos.z);
	ten[2] = VECTOR3(transform.position.x - vPos.x, transform.position.y - vPos.y, transform.position.z - vPos.z);
	ten[3] = VECTOR3(transform.position.x - vPos.x, transform.position.y + vPos.y, transform.position.z - vPos.z);
	ten[4] = VECTOR3(transform.position.x + vPos.x, transform.position.y + vPos.y, transform.position.z + vPos.z);
	ten[5] = VECTOR3(transform.position.x + vPos.x, transform.position.y - vPos.y, transform.position.z + vPos.z);
	ten[6] = VECTOR3(transform.position.x - vPos.x, transform.position.y - vPos.y, transform.position.z + vPos.z);
	ten[7] = VECTOR3(transform.position.x - vPos.x, transform.position.y + vPos.y, transform.position.z + vPos.z);
	
	// �e�ӂ̒��_�p�[�c
	int edgePoint[12][2] = {
		{0, 1}, {1, 2}, {2, 3}, {3, 0},//���ʁF�E�A�@���A�@���A�@��
		{0, 4}, {1, 5}, {2, 6}, {3, 7},//���ʁF�E��A�E���A�����A����
		{4, 5}, {5, 6}, {6, 7}, {7, 4} //�w�ʁF�E�A�@���A�@���A�@���A
	};

	// �ӂ̒������P�ō쐬
	for (int i = 0; i < 12; i++) {
		//	gpt				�_�@���_�p�[�c�̍�				���_�p�[�c�̉E
		edge[i] = normalize(ten[edgePoint[i][1]] - ten[edgePoint[i][0]]);
	}

	int planePoit[6][3] = {
		// �@���x�N�g�������߂邽�߂̕Ӄx�N�g��2�Ƌ��ʂ̒��_ 
		//�@�@���ʁ@�@�@�@�E�@�@�@�@�@�@���@�@�@�@�@���
			{1, 0, 2}, {5, 4, 1}, {2, 3, 6}, {5, 4, 6},
		//�@�@��@�@�@�@�@��
			{3, 0, 7}, {2, 1, 5}
	};

	for (int i = 0; i < 6; i++) {
		VECTOR3 v1 = ten[planePoit[i][0]] - ten[planePoit[i][1]];
		VECTOR3 v2 = ten[planePoit[i][2]] - ten[planePoit[i][1]];
		plane[i] = normalize(cross(v1, v2));

		d[i] = dot(-plane[i], ten[planePoit[i][1]]);
	}
	return ;
}

VECTOR3 Box::HitSphereToCubeplane(Object3D* player)
{
	pushVec = VECTOR3(0, 0, 0);

	// ���̒��S�_����e���_�ւ̃x�N�g��
	for (int i = 0; i < 8; i++) {
		pt[i] = player->Position() - ten[i];
	}

	// ���̒��S�_����ӂɐ��������낵���Ƃ��ɕӂ͈͓̔��ɂ��邩�ǂ���
	// 1~0���͈͓�

	int TptPoint[12] = {
		{0}, {1}, {2}, {3},
		{0}, {1}, {2}, {3},
		{4}, {5}, {6}, {7}
	};

	for (int i = 0; i < 12; i++) {
		Tpt[i] = dot(edge[i], pt[TptPoint[i]]) / edge[i].Length();
	}

	// ���ʂƂ̋����v�Z

	for (int i = 0; i < 6; i++) {
		distance[i] = abs(dot(plane[i], player->Position()) + d[i]) / plane[i].Length();
	}

	// ��1
	if (distance[0] <= 0.5f) {	// �������ʂɐڂ��Ă���
		if (0 <= Tpt[0] && Tpt[0] <= 1 && 0 <= Tpt[1] && Tpt[1] <= 1) {
			//transform.position.z += 0.02f;
			pushVec.z = 0.02;
			return pushVec;
		}
	}
	// ��2
	if (distance[1] <= 0.5f) {
		if (0 <= Tpt[0] && Tpt[0]<= 1 && 0 <= Tpt[4] && Tpt[4] <= 1) {
			//transform.position.x -= 0.02;
			pushVec.x = -0.02f;
			return pushVec;
		}
	}
	// ��3
	if (distance[2] <= 0.5f) {
		if (0 <= Tpt[2] && Tpt[2] <= 1 && 0 <= Tpt[6] && Tpt[6] <= 1) {
			//transform.position.x += 0.02;
			pushVec.x = 0.02f;
			return pushVec;
		}
	}
	// ��4
	if (distance[3] <= 0.5f) {
		if (0 <= Tpt[8] && Tpt[8] <= 1 && 0 <= Tpt[9] && Tpt[9] <= 1) {
			//transform.position.z -= 0.02;
			pushVec.z = -0.02f;
			return pushVec;
		}
	}
	// ��5
	if (distance[4] <= 0.5f) {
		if (0 <= Tpt[3] && Tpt[3] <= 1 && 0 <= Tpt[4] && Tpt[4] <= 1) {
			//transform.position.y -= 0.02;
			pushVec.y = -0.02f;
			return pushVec;
		}
	}
	// ��6
	if (distance[5] <= 0.5f) {
		if (0 <= Tpt[1] && Tpt[1] <= 1 && 0 <= Tpt[5] && Tpt[5] <= 1) {
			//transform.position.y += 0.02;
			pushVec.y = 0.02f;
			return pushVec;
		}
	}
	
	HitSphereToCubeEdge(player);

	return VECTOR3();
}

VECTOR3 Box::HitSphereToCubeEdge(Object3D* player)
{
	int TptPoint[12] = {
		{0}, {1}, {2}, {3},
		{0}, {1}, {2}, {3},
		{4}, {5}, {6}, {7}
	};

	for (int i = 0; i < 12; i++) {
		distanceV[i] = edge[i] * dot(edge[i], pt[TptPoint[i]]) / edge[i].Length() - pt[TptPoint[i]];
	}

	// ��0
	if (0 <= Tpt[0] && Tpt[0] <= 1) {
		if (distanceV[0].Length() <= 0.5f) {
			//transform.position.z += 0.02f;
			//transform.position.x -= 0.02f;

			pushVec = VECTOR3(-0.02, 0, 0.02);
			return pushVec;
		}
	}
	// ��1
	if (0 <= Tpt[1] && Tpt[1] <= 1) {
		if (distanceV[1].Length() <= 0.5f) {
			//transform.position.y += 0.02f;
			//transform.position.z += 0.02f;

			pushVec = VECTOR3(0, 0.02, 0.02);
			return pushVec;
		}
	}
	// ��2
	if (0 <= Tpt[2] && Tpt[2] <= 1) {
		if (distanceV[2].Length() <= 0.5f) {
			//transform.position.z += 0.02f;
			//transform.position.x += 0.02f;

			pushVec = VECTOR3(0.02, 0, 0.02);
			return pushVec;
		}
	}
	// ��3
	if (0 <= Tpt[3] && Tpt[3] <= 1) {
		if (distanceV[3].Length() <= 0.5f) {
			//transform.position.y -= 0.02f;
			//transform.position.z += 0.02f;

			pushVec = VECTOR3(0, -0.02, 0.02);
			return pushVec;
		}
	}
	// ��4
	if (0 <= Tpt[4] && Tpt[4] <= 1) {
		if (distanceV[4].Length() <= 0.5f) {
			//transform.position.y -= 0.02f;
			//transform.position.x -= 0.02f;

			pushVec = VECTOR3(-0.02, -0.02, 0);
			return pushVec;
		}
	}
	// ��5
	if (0 <= Tpt[5] && Tpt[5] <= 1) {
		if (distanceV[5].Length() <= 0.5f) {
			//transform.position.y += 0.02f;
			//transform.position.x -= 0.02f;

			pushVec = VECTOR3(-0.02, 0.02, 0);
			return pushVec;
		}
	}
	// ��6
	if (0 <= Tpt[6] && Tpt[6] <= 1) {
		if (distanceV[6].Length() <= 0.5f) {
			//transform.position.y += 0.02f;
			//transform.position.x += 0.02f;

			pushVec = VECTOR3(0.02, 0.02, 0);
			return pushVec;
		}
	}
	// ��7
	if (0 <= Tpt[7] && Tpt[7] <= 1) {
		if (distanceV[7].Length() <= 0.5f) {
			//transform.position.y -= 0.02f;
			//transform.position.x += 0.02f;

			pushVec = VECTOR3(0.02, -0.02, 0);
			return pushVec;
		}
	}
	// ��8
	if (0 <= Tpt[8] && Tpt[8] <= 1) {
		if (distanceV[8].Length() <= 0.5f) {
			//transform.position.z -= 0.02f;
			//transform.position.x -= 0.02f;

			pushVec = VECTOR3(-0.02, 0, -0.02);
			return pushVec;
		}
	}
	// ��9
	if (0 <= Tpt[9] && Tpt[9] <= 1) {
		if (distanceV[9].Length() <= 0.5f) {
			//transform.position.y += 0.02f;
			//transform.position.z -= 0.02f;

			pushVec = VECTOR3(0, 0.02, -0.02);
			return pushVec;
		}
	}
	// ��10
	if (0 <= Tpt[10] && Tpt[10] <= 1) {
		if (distanceV[10].Length() <= 0.5f) {
			//transform.position.z -= 0.02f;
			//transform.position.x += 0.02f;

			pushVec = VECTOR3(0.02, 0, -0.02);
			return pushVec;
		}
	}
	// ��11
	if (0 <= Tpt[11] && Tpt[11] <= 1) {
		if (distanceV[11].Length() <= 0.5f) {
			//transform.position.y -= 0.02f;
			//transform.position.z -= 0.02f;

			pushVec = VECTOR3(0, -0.02, -0.02);
			return pushVec;
		}
	}
	HitSphereToCubeVertices(player);
	return VECTOR3();
}

VECTOR3 Box::HitSphereToCubeVertices(Object3D* player)
{

	// ���_0
	if (pt[0].Length() <= 0.5f) {
		pushVec = VECTOR3(-0.02, -0.02, 0.02);
		return pushVec;
	}
	// ���_1
	if (pt[1].Length() <= 0.5f) {
		pushVec = VECTOR3(-0.02, 0.02, 0.02);
		return pushVec;
	}
	// ���_2
	if (pt[2].Length() <= 0.5f) {
		pushVec = VECTOR3(0.02, 0.02, 0.02);
		return pushVec;
	}
	// ���_3
	if (pt[3].Length() <= 0.5f) {
		pushVec = VECTOR3(0.02, -0.02, 0.02);
		return pushVec;
	}
	// ���_4
	if (pt[4].Length() <= 0.5f) {
		pushVec = VECTOR3(-0.02, -0.02, -0.02);
		return pushVec;
	}
	// ���_5
	if (pt[5].Length() <= 0.5f) {
		pushVec = VECTOR3(-0.02, 0.02, -0.02);
		return pushVec;
	}
	// ���_6
	if (pt[6].Length() <= 0.5f) {
		pushVec = VECTOR3(0.02, 0.02, -0.02);
		return pushVec;
	}
	// ���_7
	if (pt[7].Length() <= 0.5f) {
		pushVec = VECTOR3(0.02, -0.02, -0.02);
		return pushVec;
	}

	return VECTOR3();
}



Box::~Box()
{
	if (mesh != nullptr) {
		delete mesh;
		mesh = nullptr;
	}
}

