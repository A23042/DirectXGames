#include "ScoreArea.h"

ScoreArea::ScoreArea()
{
}

ScoreArea::~ScoreArea()
{
}

void ScoreArea::Update()
{
	transform.position = pObj.center;
	vPos = transform.scale / 2;

	// �����̂̊e���_���W
	vertex[0] = VECTOR3(vPos.x, vPos.y, -vPos.z);
	vertex[1] = VECTOR3(vPos.x, -vPos.y, -vPos.z);
	vertex[2] = VECTOR3(-vPos.x, -vPos.y, -vPos.z);
	vertex[3] = VECTOR3(-vPos.x, vPos.y, -vPos.z);
	vertex[4] = VECTOR3(vPos.x, vPos.y, vPos.z);
	vertex[5] = VECTOR3(vPos.x, -vPos.y, vPos.z);
	vertex[6] = VECTOR3(-vPos.x, -vPos.y, vPos.z);
	vertex[7] = VECTOR3(-vPos.x, vPos.y, vPos.z);


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

}

void ScoreArea::Draw()
{
	mesh->Render(transform.matrix());

	// �e�ӂ̒��_�p�[�c
	int edgePoint[12][2] = {
		{0, 1}, {1, 2}, {2, 3}, {3, 0},//���ʁF�E�A�@���A�@���A�@��
		{0, 4}, {1, 5}, {2, 6}, {3, 7},//���ʁF�E��A�E���A�����A����
		{4, 5}, {5, 6}, {6, 7}, {7, 4} //�w�ʁF�E�A�@���A�@���A�@���A
	};

	// �I������Ă���ꍇ���g�̃A�E�g���C��\��������
	if (editObj.isSelect)
	{
		vPos = transform.scale / 2;
		CubeSize(vPos.x, vPos.y, vPos.z);

		for (int i = 0; i < 12; i++)
		{
			spr->DrawLine3D(vertex[edgePoint[i][1]], vertex[edgePoint[i][0]], RGB(0, 255, 50), 1.0f);
		}
	}
}

bool ScoreArea::CheckSphereAABBCollision(PhysicsObject& tObj)
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

	// �͈͓��ł����true
	return distance <= tObj.radius;
}

ScoreArea1::ScoreArea1(VECTOR3 size, VECTOR3 rot)
{
	SetTag("SCOREAREA");
	editObj.name = "scoreArea1";
	mesh = new CFbxMesh;
	mesh->Load("Data/Gizmo/Area1.mesh");
	meshCol = new MeshCollider;
	meshCol->MakeFromMesh(mesh);

	vPos = size / 2;
	transform.scale = size;

	// ��]�p�����W�A���ɕϊ����A��]�s����쐬
	transform.rotation.x += rot.x / 180.0f * XM_PI;
	transform.rotation.y += rot.y / 180.0f * XM_PI;
	transform.rotation.z += rot.z / 180.0f * XM_PI;

	rotationMatrix = XMMatrixRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z);

	SetDrawOrder(-1);
}

ScoreArea1::~ScoreArea1()
{
}

void ScoreArea1::Update()
{
	transform.position = pObj.center;
	vPos = transform.scale / 2;
	CubeSize(vPos.x, vPos.y, vPos.z);
}

ScoreArea2::ScoreArea2(VECTOR3 size, VECTOR3 rot)
{
	SetTag("SCOREAREA");
	editObj.name = "scoreArea2";
	mesh = new CFbxMesh;
	mesh->Load("Data/Gizmo/Area2.mesh");
	meshCol = new MeshCollider;
	meshCol->MakeFromMesh(mesh);

	vPos = size / 2;
	transform.scale = size;

	// ��]�p�����W�A���ɕϊ����A��]�s����쐬
	transform.rotation.x += rot.x / 180.0f * XM_PI;
	transform.rotation.y += rot.y / 180.0f * XM_PI;
	transform.rotation.z += rot.z / 180.0f * XM_PI;

	rotationMatrix = XMMatrixRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z);

	SetDrawOrder(-1);
}

ScoreArea2::~ScoreArea2()
{
}
void ScoreArea2::Update()
{
	transform.position = pObj.center;
	vPos = transform.scale / 2;
	CubeSize(vPos.x, vPos.y, vPos.z);

}
ScoreArea3::ScoreArea3(VECTOR3 size, VECTOR3 rot)
{
	SetTag("SCOREAREA");
	editObj.name = "scoreArea3";
	mesh = new CFbxMesh;
	mesh->Load("Data/Gizmo/Area3.mesh");
	meshCol = new MeshCollider;
	meshCol->MakeFromMesh(mesh);

	vPos = size / 2;
	transform.scale = size;

	// ��]�p�����W�A���ɕϊ����A��]�s����쐬
	transform.rotation.x += rot.x / 180.0f * XM_PI;
	transform.rotation.y += rot.y / 180.0f * XM_PI;
	transform.rotation.z += rot.z / 180.0f * XM_PI;

	rotationMatrix = XMMatrixRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z);

	SetDrawOrder(-1);
}

ScoreArea3::~ScoreArea3()
{
}

void ScoreArea3::Update()
{
	transform.position = pObj.center;
	vPos = transform.scale / 2;
	CubeSize(vPos.x, vPos.y, vPos.z);

}

