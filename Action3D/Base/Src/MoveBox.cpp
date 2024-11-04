#include "MoveBox.h"
#include "Player.h"
// ���s�ړ�Box�I�u�W�F�N�g

MoveBox::MoveBox(VECTOR3 size, VECTOR3 rot, VECTOR3 move, VECTOR3 moveSpeed)
{
	SetTag("STAGEOBJ");
	pObj.name = "MoveBox";
	mesh = new CFbxMesh();
	mesh->Load("Data/Object/box00.mesh");

	vPos = VECTOR3(size.x / 2, size.y / 2, size.z / 2);
	transform.scale = size;

	// ��]�p�����W�A���ɕϊ����A��]�s����쐬
	transform.rotation.x += rot.x / 180.0f * XM_PI;
	transform.rotation.y += rot.y / 180.0f * XM_PI;
	transform.rotation.z += rot.z / 180.0f * XM_PI;
	rotationMatrix = XMMatrixRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z);

	this->move = moveSpeed;

	moveMax = transform.position + move;
	moveMin = transform.position - move;

	pushVec = VECTOR3(0, 0, 0);
	refVec = VECTOR3(0, 0, 0);
	pObj.velocity = VECTOR3(0, 0, 0);
}

MoveBox::~MoveBox()
{
}

void MoveBox::Start()
{
	pObj.center = transform.position;
}

void MoveBox::Update()
{
	pObj.center += pObj.velocity * SceneManager::DeltaTime();
	transform.position = pObj.center;

	// ������XYZ���Ɛ����܂œ�����
	if(moveMax.x > 0)
	{
		if (transform.position.x >= moveMax.x) {
			move.x *= -1;
		}
		else if (transform.position.x <= moveMin.x) {
			move.x *= -1;
		}
	}
	if (moveMax.y > 0)
	{
		if (transform.position.y >= moveMax.y) {
			move.y *= -1;
		}
		else if (transform.position.y <= moveMin.y) {
			move.y *= -1;
		}
	}
	if (moveMax.z > 0)
	{
		if (transform.position.z >= moveMax.z) {
			move.z *= -1;
		}
		else if (transform.position.z <= moveMin.z) {
			move.z *= -1;
		}
	}
	pObj.velocity = move;

	Box::CubeSize(vPos.x, vPos.y, vPos.z);		// �����̂̃T�C�Y�ƈʒu

#if 0
	std::list<Player*> playeres =
		ObjectManager::FindGameObjects<Player>();
	for (Player* player : playeres) {
		VECTOR3 refVec = VECTOR3(0, 0, 0);
		pushVec = HitSphereToCubeplane(player->sphere, refVec);	// ��->��->���_�̏Փ˔���
		player->PushVec(-pushVec, refVec);	// �v���C���[���߂荞�񂾗ʂ������ǂ�
	}
#endif
}

VECTOR3 MoveBox::ReflectionVec(PhysicsObject& pOgj, VECTOR3 normal)
{
	// �@���x�N�g���̕����̑��x���l������
	VECTOR3 pushVecNormal = dot(pObj.velocity, normal) * normal;
	// �@�������ɔ����W����������
	VECTOR3 refNormal = dot(pObj.velocity, normal) * normal - pushVecNormal;
	VECTOR3 refSessen = pObj.velocity - refNormal;
	float e2 = (this->pObj.e + pObj.e) / 2;
	float f2 = (this->pObj.f + pObj.f) / 2;
	VECTOR3 b = -refNormal * e2 + refSessen * f2;
	// ���Ԃ̏C��
	// ���ߍ��݂�����->����	�Z
	// ����->���ߍ��݉���		�~
	return VECTOR3(b);
}
