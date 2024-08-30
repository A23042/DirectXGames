#include "Box.h"
#include "Player.h"

Box::Box()
{
	SetTag("STAGEOBJ");
	mesh = new CFbxMesh();
	mesh->Load("Data/Object/box00.mesh");
	//mesh->Load("Data/Object/ball.mesh");

	meshCol = new MeshCollider();
	meshCol->MakeFromMesh(mesh);

	ten0 = VECTOR3(transform.position.x + 0.5f, transform.position.y + 0.5f, transform.position.z - 0.5f);
	ten1 = VECTOR3(transform.position.x + 0.5f, transform.position.y - 0.5f, transform.position.z - 0.5f);
	ten2 = VECTOR3(transform.position.x - 0.5f, transform.position.y - 0.5f, transform.position.z - 0.5f);
	ten3 = VECTOR3(transform.position.x - 0.5f, transform.position.y + 0.5f, transform.position.z - 0.5f);
	ten4 = VECTOR3(transform.position.x + 0.5f, transform.position.y + 0.5f, transform.position.z + 0.5f);
	ten5 = VECTOR3(transform.position.x + 0.5f, transform.position.y - 0.5f, transform.position.z + 0.5f);
	ten6 = VECTOR3(transform.position.x - 0.5f, transform.position.y - 0.5f, transform.position.z + 0.5f);
	ten7 = VECTOR3(transform.position.x - 0.5f, transform.position.y + 0.5f, transform.position.z + 0.5f);

}

void Box::Update()
{
#if 1
	// �����̂̊e���_���W
	ten0 = VECTOR3(transform.position.x + 0.5f, transform.position.y + 0.5f, transform.position.z - 0.5f);
	ten1 = VECTOR3(transform.position.x + 0.5f, transform.position.y - 0.5f, transform.position.z - 0.5f);
	ten2 = VECTOR3(transform.position.x - 0.5f, transform.position.y - 0.5f, transform.position.z - 0.5f);
	ten3 = VECTOR3(transform.position.x - 0.5f, transform.position.y + 0.5f, transform.position.z - 0.5f);
	ten4 = VECTOR3(transform.position.x + 0.5f, transform.position.y + 0.5f, transform.position.z + 0.5f);
	ten5 = VECTOR3(transform.position.x + 0.5f, transform.position.y - 0.5f, transform.position.z + 0.5f);
	ten6 = VECTOR3(transform.position.x - 0.5f, transform.position.y - 0.5f, transform.position.z + 0.5f);
	ten7 = VECTOR3(transform.position.x - 0.5f, transform.position.y + 0.5f, transform.position.z + 0.5f);
	// �e�Ӄx�N�g���𒷂��P�ō��
	edge0  = normalize(ten1 - ten0);
	edge1  = normalize(ten2 - ten1);
	edge2  = normalize(ten3 - ten2);
	edge3  = normalize(ten0 - ten3);
	edge4  = normalize(ten0 - ten4);
	edge5  = normalize(ten1 - ten5);
	edge6  = normalize(ten2 - ten6);
	edge7  = normalize(ten3 - ten7);
	edge8  = normalize(ten5 - ten4);
	edge9  = normalize(ten6 - ten5);
	edge10 = normalize(ten7 - ten6);
	edge11 = normalize(ten4 - ten7);

	// �R�̒��_���畽�ʂ̖@���x�N�g��(���ʂɐ����ȃx�N�g��)�����
	//��1
	v1 = ten1 - ten0;
	v2 = ten2 - ten0;
	VECTOR3 normal0 = cross(v1, v2);	// �O��
	normalize(normal0);
	float d0 = dot(-normal0, ten0);
	//��2
	v3 = ten5 - ten4;
	v4 = ten1 - ten4;
	VECTOR3 normal1 = cross(v3, v4);
	normalize(normal1);
	float d1 = dot(-normal1, ten4);
	//��3
	v5 = ten2 - ten3;
	v6 = ten6 - ten3;
	VECTOR3 normal2 = cross(v5, v6);
	normalize(normal2);
	float d2 = dot(-normal2, ten3);
	//��4
	v7 = ten5 - ten4;
	v8 = ten6 - ten4;
	VECTOR3 normal3 = cross(v7, v8);
	normalize(normal3);
	float d3 = dot(-normal3, ten4);
	//��5
	v9 = ten3 - ten0;
	v10 = ten7 - ten0;
	VECTOR3 normal4 = cross(v9, v10);
	normalize(normal4);
	float d4 = dot(-normal4, ten0);
	//��6
	v11 = ten2 - ten1;
	v12 = ten5 - ten1;
	VECTOR3 normal5 = cross(v11, v12);
	normalize(normal5);
	float d5 = dot(-normal5, ten1);


	std::list<Player*> playeres =
		ObjectManager::FindGameObjects<Player>();
	for (Player* player : playeres) {
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

		// ���ʂƂ̋����v�Z
		float distance0 = abs(dot(normal0, player->Position()) + d0) / normal0.Length();
		float distance1 = abs(dot(normal1, player->Position()) + d1) / normal1.Length();
		float distance2 = abs(dot(normal2, player->Position()) + d2) / normal2.Length();
		float distance3 = abs(dot(normal3, player->Position()) + d3) / normal3.Length();
		float distance4 = abs(dot(normal4, player->Position()) + d4) / normal4.Length();
		float distance5 = abs(dot(normal5, player->Position()) + d5) / normal5.Length();

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



	}
#endif
}

Box::~Box()
{
	if (mesh != nullptr) {
		delete mesh;
		mesh = nullptr;
	}
}

