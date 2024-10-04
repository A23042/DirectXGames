#include "MoveBox.h"
#include "Player.h"

MoveBox::MoveBox(VECTOR3 size, VECTOR3 rot, VECTOR3 move, VECTOR3 moveSpeed)
{
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

	e = 0.6f;	// �����W��	1�Ō����Ȃ�
	f = 0.99f;	// ���C		1�Ō����Ȃ�
}

MoveBox::~MoveBox()
{
}

void MoveBox::Update()
{
	transform.position += move;
	// ������XYZ���Ɛ����܂œ�����
	if (transform.position.x >= moveMax.x) {
		move.x *= -1;
	}
	else if(transform.position.x <= moveMin.x){
		move.x *= -1;
	}

#if 1
	std::list<Player*> playeres =
		ObjectManager::FindGameObjects<Player>();
	for (Player* player : playeres) {
		VECTOR3 refVec = VECTOR3(0, 0, 0);
		Box::CubeSize(vPos.x, vPos.y, vPos.z);		// �����̂̃T�C�Y�ƈʒu
		pushVec = Box::HitSphereToCubeplane(player->sphere, refVec);	// ��->��->���_�̏Փ˔���
		player->PushVec(-pushVec, refVec);	// �v���C���[���߂荞�񂾗ʂ������ǂ�
	}
#endif
}
