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
	
#if 0
	// �R�̒��_���畽�ʂ̖@���x�N�g��(���ʂɐ����ȃx�N�g��)�����
	//��1
	v1 = ten1 - ten0;
	v2 = ten2 - ten0;
	normal[0] = cross(v1, v2);	// �O��
	normalize(normal[0]);
	d[0] = dot(-normal[0], ten0);
	//��2
	v3 = ten5 - ten4;
	v4 = ten1 - ten4;
	normal[1] = cross(v3, v4);
	normalize(normal[1]);
	d[1] = dot(-normal[1], ten4);
	//��3
	v5 = ten2 - ten3;
	v6 = ten6 - ten3;
	normal[2] = cross(v5, v6);
	normalize(normal[2]);
	d[2] = dot(-normal[2], ten3);
	//��4
	v7 = ten5 - ten4;
	v8 = ten6 - ten4;
	normal[3] = cross(v7, v8);
	normalize(normal[3]);
	d[3] = dot(-normal[3], ten4);
	//��5
	v9 = ten3 - ten0;
	v10 = ten7 - ten0;
	normal[4] = cross(v9, v10);
	normalize(normal[4]);
	d[4] = dot(-normal[4], ten0);
	//��6
	v11 = ten2 - ten1;
	v12 = ten5 - ten1;
	normal[5] = cross(v11, v12);
	normalize(normal[5]);
	d[5] = dot(-normal[5], ten1);
#endif

	std::list<Player*> playeres =
		ObjectManager::FindGameObjects<Player>();
	for (Player* player : playeres) {
		CubeSize(vPos.x, vPos.y, vPos.z);
		HitSphereToCubeplane(player);
		transform.position += pushVec;
#if 0
		// ���̒��S�_����e���_�ւ̃x�N�g��
		pt0 = player->Position() - ten0;
		pt1 = player->Position() - ten1;
		pt2 = player->Position() - ten2;
		pt3 = player->Position() - ten3;
		pt4 = player->Position() - ten4;
		pt5 = player->Position() - ten5;
		pt6 = player->Position() - ten6;
		pt7 = player->Position() - ten7;

		// ���̒��S�_����ӂɐ��������낵���Ƃ��ɕӂ͈͓̔��ɂ��邩�ǂ���
		// 1~0���͈͓�
		float Tpt0 = dot(edge0, pt0) / edge0.Length();
		float Tpt1 = dot(edge1, pt1) / edge1.Length();
		float Tpt2 = dot(edge2, pt2) / edge2.Length();
		float Tpt3 = dot(edge3, pt3) / edge3.Length();

		float Tpt4 = dot(edge4, pt4) / edge4.Length();
		float Tpt5 = dot(edge5, pt5) / edge5.Length();
		float Tpt6 = dot(edge6, pt6) / edge6.Length();
		float Tpt7 = dot(edge7, pt7) / edge7.Length();

		float Tpt8 = dot(edge8, pt4) / edge8.Length();
		float Tpt9 = dot(edge9, pt5) / edge9.Length();
		float Tpt10 = dot(edge10, pt6) / edge10.Length();
		float Tpt11 = dot(edge11, pt7) / edge11.Length();
//#endif
//#if 0
		// ���ʂƂ̋����v�Z
		float distance0 = abs(dot(normal0, player->Position()) + d0) / normal0.Length();
		float distance1 = abs(dot(normal1, player->Position()) + d1) / normal1.Length();
		float distance2 = abs(dot(normal2, player->Position()) + d2) / normal2.Length();
		float distance3 = abs(dot(normal3, player->Position()) + d3) / normal3.Length();
		float distance4 = abs(dot(normal4, player->Position()) + d4) / normal4.Length();
		float distance5 = abs(dot(normal5, player->Position()) + d5) / normal5.Length();
//#endif
		// �ӂƂ̋���
		VECTOR3 distance00 = edge0  * dot(edge0, pt0) / edge0.Length() - pt0;
		VECTOR3 distance01 = edge1  * dot(edge1, pt1) / edge1.Length() - pt1;
		VECTOR3 distance02 = edge2  * dot(edge2, pt2) / edge2.Length() - pt2;
		VECTOR3 distance03 = edge3  * dot(edge3, pt3) / edge3.Length() - pt3;

		VECTOR3 distance04 = edge4  * dot(edge4, pt0) / edge4.Length() - pt0;
		VECTOR3 distance05 = edge5  * dot(edge5, pt1) / edge5.Length() - pt1;
		VECTOR3 distance06 = edge6  * dot(edge6, pt2) / edge6.Length() - pt2;
		VECTOR3 distance07 = edge7  * dot(edge7, pt3) / edge7.Length() - pt3;

		VECTOR3 distance08 = edge8  * dot(edge8, pt4) / edge8.Length() - pt4;
		VECTOR3 distance09 = edge9  * dot(edge9, pt5) / edge9.Length() - pt5;
		VECTOR3 distance10 = edge10 * dot(edge10, pt6) / edge10.Length() - pt6;
		VECTOR3 distance11 = edge11 * dot(edge11, pt7) / edge11.Length() - pt7;
		
		// ���_0
		if (pt0.Length() <= 0.5f) {
			transform.position.x -= 0.02f;
			transform.position.y -= 0.02f;
			transform.position.z += 0.02f;
		}
		// ���_1
		if (pt1.Length() <= 0.5f) {
			transform.position.x -= 0.02f;
			transform.position.y += 0.02f;
			transform.position.z += 0.02f;
		}
		// ���_2
		if (pt2.Length() <= 0.5f) {
			transform.position.x += 0.02f;
			transform.position.y += 0.02f;
			transform.position.z += 0.02f;
		}
		// ���_3
		if (pt3.Length() <= 0.5f) {
			transform.position.x += 0.02f;
			transform.position.y -= 0.02f;
			transform.position.z += 0.02f;
		}
		// ���_4
		if (pt4.Length() <= 0.5f) {
			transform.position.x -= 0.02f;
			transform.position.y -= 0.02f;
			transform.position.z -= 0.02f;
		}
		// ���_5
		if (pt5.Length() <= 0.5f) {
			transform.position.x -= 0.02f;
			transform.position.y += 0.02f;
			transform.position.z -= 0.02f;
		}
		// ���_6
		if (pt6.Length() <= 0.5f) {
			transform.position.x += 0.02f;
			transform.position.y += 0.02f;
			transform.position.z -= 0.02f;
		}
		// ���_7
		if (pt7.Length() <= 0.5f) {
			transform.position.x += 0.02f;
			transform.position.y -= 0.02f;
			transform.position.z -= 0.02f;
		}

		// ��0
		if (0 <= Tpt0 && Tpt0 <= 1) {
			if (distance00.Length() <= 0.5f) {
				transform.position.z += 0.02f;
				transform.position.x -= 0.02f;
			}
		}
		// ��1
		if (0 <= Tpt1 && Tpt1 <= 1) {
			if (distance01.Length() <= 0.5f) {
				transform.position.y += 0.02f;
				transform.position.z += 0.02f;
			}
		}
		// ��2
		if (0 <= Tpt2 && Tpt2 <= 1) {
			if (distance02.Length() <= 0.5f) {
				transform.position.z += 0.02f;
				transform.position.x += 0.02f;
			}
		}
		// ��3
		if (0 <= Tpt3 && Tpt3 <= 1) {
			if (distance03.Length() <= 0.5f) {
				transform.position.y -= 0.02f;
				transform.position.z += 0.02f;
			}
		}
		// ��4
		if (0 <= Tpt4 && Tpt4 <= 1) {
			if (distance04.Length() <= 0.5f) {
				transform.position.y -= 0.02f;
				transform.position.x -= 0.02f;
			}
		}
		// ��5
		if (0 <= Tpt5 && Tpt5 <= 1) {
			if (distance05.Length() <= 0.5f) {
				transform.position.y += 0.02f;
				transform.position.x -= 0.02f;
			}
		}
		// ��6
		if (0 <= Tpt6 && Tpt6 <= 1) {
			if (distance06.Length() <= 0.5f) {
				transform.position.y += 0.02f;
				transform.position.x += 0.02f;
			}
		}
		// ��7
		if (0 <= Tpt7 && Tpt7 <= 1) {
			if (distance07.Length() <= 0.5f) {
				transform.position.y -= 0.02f;
				transform.position.x += 0.02f;
			}
		}
		// ��8
		if (0 <= Tpt8 && Tpt8 <= 1) {
			if (distance08.Length() <= 0.5f) {
				transform.position.z -= 0.02f;
				transform.position.x -= 0.02f;
			}
		}
		// ��9
		if (0 <= Tpt9 && Tpt9 <= 1) {
			if (distance09.Length() <= 0.5f) {
				transform.position.y += 0.02f;
				transform.position.z -= 0.02f;
			}
		}
		// ��10
		if (0 <= Tpt10 && Tpt10 <= 1) {
			if (distance10.Length() <= 0.5f) {
				transform.position.z -= 0.02f;
				transform.position.x += 0.02f;
			}
		}
		// ��11
		if (0 <= Tpt11 && Tpt11 <= 1) {
			if (distance11.Length() <= 0.5f) {
				transform.position.y -= 0.02f;
				transform.position.z -= 0.02f;
			}
		}
#endif
#if 0
		// ��1
		if (distance0 <= 0.5f) {	// �������ʂɐڂ��Ă���
			if (0 <= Tpt0 && Tpt0 <= 1 && 0 <= Tpt1 && Tpt1 <= 1 && 0 <= Tpt2 && Tpt2 <= 1 && 0 <= Tpt3 && Tpt3 <= 1) {
				transform.position.z += 0.02;
			}
		}
		// ��2
		if (distance1 <= 0.5f) {
			if (0 <= Tpt0 && Tpt0 <= 1 && 0 <= Tpt4 && Tpt4 <= 1 && 0 <= Tpt5 && Tpt5 <= 1 && 0 <= Tpt8 && Tpt8 <= 1) {
				transform.position.x -= 0.02;
			}
		}
		// ��3
		if (distance2 <= 0.5f) {
			if (0 <= Tpt2 && Tpt2 <= 1 && 0 <= Tpt6 && Tpt6 <= 1 && 0 <= Tpt7 && Tpt7 <= 1 && 0 <= Tpt10 && Tpt10 <= 1) {
				transform.position.x += 0.02;
			}
		}
		// ��4
		if (distance3 <= 0.5f) {
			if (0 <= Tpt8 && Tpt8 <= 1 && 0 <= Tpt9 && Tpt9 <= 1 && 0 <= Tpt10 && Tpt10 <= 1 && 0 <= Tpt11 && Tpt11 <= 1) {
				transform.position.z -= 0.02;
			}
		}
		// ��5
		if (distance4 <= 0.5f) {
			if (0 <= Tpt3 && Tpt3 <= 1 && 0 <= Tpt4 && Tpt4 <= 1 && 0 <= Tpt7 && Tpt7 <= 1 && 0 <= Tpt11 && Tpt11 <= 1) {
				transform.position.y -= 0.02;
			}
		}
		// ��6
		if (distance5 <= 0.5f) {
			if (0 <= Tpt1 && Tpt1 <= 1 && 0 <= Tpt5 && Tpt5 <= 1 && 0 <= Tpt6 && Tpt6 <= 1 && 0 <= Tpt9 && Tpt9 <= 1) {
				transform.position.y += 0.02;
			}
		}
#endif

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

	// �e�ʂ̖@���x�N�g�������߂�
	//int planePoit[6][3] = {
	// �@���x�N�g�������߂邽�߂̕Ӄx�N�g��2�Ƌ��ʂ̒��_ 
	//�@�@���ʁ@�@�@�@�E�@�@�@�@�@�@���@�@�@�@�@���
	//	{0, 1, 0}, {0, 4, 0}, {2, 6, 2}, {8, 9, 5},
	//�@�@��@�@�@�@�@��
	//	{3, 4, 0}, {1, 5, 1}
	//};

	int planePoit[6][3] = {
		// �@���x�N�g�������߂邽�߂̕Ӄx�N�g��2�Ƌ��ʂ̒��_ 
		//�@�@���ʁ@�@�@�@�E�@�@�@�@�@�@���@�@�@�@�@���
			{1, 0, 2}, {5, 4, 1}, {2, 3, 6}, {5, 4, 6},
		//�@�@��@�@�@�@�@��
			{3, 0, 7}, {2, 1, 5}
	};

	for (int i = 0; i < 6; i++) {
		//plane[i] = normalize(cross(edge[planePoit[i][1]], edge[planePoit[i][0]]));
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
	float Tpt0 = dot(edge[0], pt[0]) / edge[0].Length();
	float Tpt1 = dot(edge[1], pt[1]) / edge[1].Length();
	float Tpt2 = dot(edge[2], pt[2]) / edge[2].Length();
	float Tpt3 = dot(edge[3], pt[3]) / edge[3].Length();

	float Tpt4 = dot(edge[4], pt[0]) / edge[4].Length();
	float Tpt5 = dot(edge[5], pt[1]) / edge[5].Length();
	float Tpt6 = dot(edge[6], pt[2]) / edge[6].Length();
	float Tpt7 = dot(edge[7], pt[3]) / edge[7].Length();

	float Tpt8 = dot(edge[8], pt[4]) / edge[8].Length();
	float Tpt9 = dot(edge[9], pt[5]) / edge[9].Length();
	float Tpt10 = dot(edge[10], pt[6]) / edge[10].Length();
	float Tpt11 = dot(edge[11], pt[7]) / edge[11].Length();

	float Tpt[12] = { NULL };

	for (int i = 0; i < 12; i++) {

	}

	// ���ʂƂ̋����v�Z
	/*
	distance[0] = abs(dot(plane[0], player->Position()) + d[0]) / plane[0].Length();
	distance[1] = abs(dot(plane[1], player->Position()) + d[1]) / plane[1].Length();
	distance[2] = abs(dot(plane[2], player->Position()) + d[2]) / plane[2].Length();
	distance[3] = abs(dot(plane[3], player->Position()) + d[3]) / plane[3].Length();
	distance[4] = abs(dot(plane[4], player->Position()) + d[4]) / plane[4].Length();
	distance[5] = abs(dot(plane[5], player->Position()) + d[5]) / plane[5].Length();
	*/

	for (int i = 0; i < 6; i++) {
		distance[i] = abs(dot(plane[i], player->Position()) + d[i]) / plane[i].Length();
	}

	// ��1
	if (distance[0] <= 0.5f) {	// �������ʂɐڂ��Ă���
		if (0 <= Tpt0 && Tpt0 <= 1 && 0 <= Tpt1 && Tpt1 <= 1) {
			//transform.position.z += 0.02f;
			pushVec.z = 0.02;
			return pushVec;
		}
	}
	// ��2
	if (distance[1] <= 0.5f) {
		if (0 <= Tpt0 && Tpt0 <= 1 && 0 <= Tpt4 && Tpt4 <= 1) {
			//transform.position.x -= 0.02;
			pushVec.x = -0.02f;
			return pushVec;
		}
	}
	// ��3
	if (distance[2] <= 0.5f) {
		if (0 <= Tpt2 && Tpt2 <= 1 && 0 <= Tpt6 && Tpt6 <= 1) {
			//transform.position.x += 0.02;
			pushVec.x = 0.02f;
			return pushVec;
		}
	}
	// ��4
	if (distance[3] <= 0.5f) {
		if (0 <= Tpt8 && Tpt8 <= 1 && 0 <= Tpt9 && Tpt9 <= 1) {
			//transform.position.z -= 0.02;
			pushVec.z = -0.02f;
			return pushVec;
		}
	}
	// ��5
	if (distance[4] <= 0.5f) {
		if (0 <= Tpt3 && Tpt3 <= 1 && 0 <= Tpt4 && Tpt4 <= 1) {
			//transform.position.y -= 0.02;
			pushVec.y = -0.02f;
			return pushVec;
		}
	}
	// ��6
	if (distance[5] <= 0.5f) {
		if (0 <= Tpt1 && Tpt1 <= 1 && 0 <= Tpt5 && Tpt5 <= 1) {
			//transform.position.y += 0.02;
			pushVec.y = 0.02f;
			return pushVec;
		}
	}
	// �ӂƂ̋���
	VECTOR3 distance00 = edge[0] * dot(edge[0], pt[0]) / edge[0].Length() - pt[0];
	VECTOR3 distance01 = edge[1] * dot(edge[1], pt[1]) / edge[1].Length() - pt[1];
	VECTOR3 distance02 = edge[2] * dot(edge[2], pt[2]) / edge[2].Length() - pt[2];
	VECTOR3 distance03 = edge[3] * dot(edge[3], pt[3]) / edge[3].Length() - pt[3];

	VECTOR3 distance04 = edge[4] * dot(edge[4], pt[0]) / edge[4].Length() - pt[0];
	VECTOR3 distance05 = edge[5] * dot(edge[5], pt[1]) / edge[5].Length() - pt[1];
	VECTOR3 distance06 = edge[6] * dot(edge[6], pt[2]) / edge[6].Length() - pt[2];
	VECTOR3 distance07 = edge[7] * dot(edge[7], pt[3]) / edge[7].Length() - pt[3];

	VECTOR3 distance08 = edge[8] * dot(edge[8], pt[4]) / edge[8].Length() - pt[4];
	VECTOR3 distance09 = edge[9] * dot(edge[9], pt[5]) / edge[9].Length() - pt[5];
	VECTOR3 distance10 = edge[10] * dot(edge[10], pt[6]) / edge[10].Length() - pt[6];
	VECTOR3 distance11 = edge[11] * dot(edge[11], pt[7]) / edge[11].Length() - pt[7];

	// ���_0
	if (pt[0].Length() <= 0.5f) {
		transform.position.x -= 0.02f;
		transform.position.y -= 0.02f;
		transform.position.z += 0.02f;
	}
	// ���_1
	if (pt[1].Length() <= 0.5f) {
		transform.position.x -= 0.02f;
		transform.position.y += 0.02f;
		transform.position.z += 0.02f;
	}
	// ���_2
	if (pt[2].Length() <= 0.5f) {
		transform.position.x += 0.02f;
		transform.position.y += 0.02f;
		transform.position.z += 0.02f;
	}
	// ���_3
	if (pt[3].Length() <= 0.5f) {
		transform.position.x += 0.02f;
		transform.position.y -= 0.02f;
		transform.position.z += 0.02f;
	}
	// ���_4
	if (pt[4].Length() <= 0.5f) {
		transform.position.x -= 0.02f;
		transform.position.y -= 0.02f;
		transform.position.z -= 0.02f;
	}
	// ���_5
	if (pt[5].Length() <= 0.5f) {
		transform.position.x -= 0.02f;
		transform.position.y += 0.02f;
		transform.position.z -= 0.02f;
	}
	// ���_6
	if (pt[6].Length() <= 0.5f) {
		transform.position.x += 0.02f;
		transform.position.y += 0.02f;
		transform.position.z -= 0.02f;
	}
	// ���_7
	if (pt[7].Length() <= 0.5f) {
		transform.position.x += 0.02f;
		transform.position.y -= 0.02f;
		transform.position.z -= 0.02f;
	}

	// ��0
	if (0 <= Tpt0 && Tpt0 <= 1) {
		if (distance00.Length() <= 0.5f) {
			transform.position.z += 0.02f;
			transform.position.x -= 0.02f;
		}
	}
	// ��1
	if (0 <= Tpt1 && Tpt1 <= 1) {
		if (distance01.Length() <= 0.5f) {
			transform.position.y += 0.02f;
			transform.position.z += 0.02f;
		}
	}
	// ��2
	if (0 <= Tpt2 && Tpt2 <= 1) {
		if (distance02.Length() <= 0.5f) {
			transform.position.z += 0.02f;
			transform.position.x += 0.02f;
		}
	}
	// ��3
	if (0 <= Tpt3 && Tpt3 <= 1) {
		if (distance03.Length() <= 0.5f) {
			transform.position.y -= 0.02f;
			transform.position.z += 0.02f;
		}
	}
	// ��4
	if (0 <= Tpt4 && Tpt4 <= 1) {
		if (distance04.Length() <= 0.5f) {
			transform.position.y -= 0.02f;
			transform.position.x -= 0.02f;
		}
	}
	// ��5
	if (0 <= Tpt5 && Tpt5 <= 1) {
		if (distance05.Length() <= 0.5f) {
			transform.position.y += 0.02f;
			transform.position.x -= 0.02f;
		}
	}
	// ��6
	if (0 <= Tpt6 && Tpt6 <= 1) {
		if (distance06.Length() <= 0.5f) {
			transform.position.y += 0.02f;
			transform.position.x += 0.02f;
		}
	}
	// ��7
	if (0 <= Tpt7 && Tpt7 <= 1) {
		if (distance07.Length() <= 0.5f) {
			transform.position.y -= 0.02f;
			transform.position.x += 0.02f;
		}
	}
	// ��8
	if (0 <= Tpt8 && Tpt8 <= 1) {
		if (distance08.Length() <= 0.5f) {
			transform.position.z -= 0.02f;
			transform.position.x -= 0.02f;
		}
	}
	// ��9
	if (0 <= Tpt9 && Tpt9 <= 1) {
		if (distance09.Length() <= 0.5f) {
			transform.position.y += 0.02f;
			transform.position.z -= 0.02f;
		}
	}
	// ��10
	if (0 <= Tpt10 && Tpt10 <= 1) {
		if (distance10.Length() <= 0.5f) {
			transform.position.z -= 0.02f;
			transform.position.x += 0.02f;
		}
	}
	// ��11
	if (0 <= Tpt11 && Tpt11 <= 1) {
		if (distance11.Length() <= 0.5f) {
			transform.position.y -= 0.02f;
			transform.position.z -= 0.02f;
		}
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

