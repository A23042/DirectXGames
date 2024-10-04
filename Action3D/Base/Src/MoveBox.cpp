#include "MoveBox.h"
#include "Player.h"

MoveBox::MoveBox(VECTOR3 size, VECTOR3 rot, VECTOR3 move, VECTOR3 moveSpeed)
{
	mesh = new CFbxMesh();
	mesh->Load("Data/Object/box00.mesh");

	vPos = VECTOR3(size.x / 2, size.y / 2, size.z / 2);
	transform.scale = size;

	// 回転角をラジアンに変換し、回転行列を作成
	transform.rotation.x += rot.x / 180.0f * XM_PI;
	transform.rotation.y += rot.y / 180.0f * XM_PI;
	transform.rotation.z += rot.z / 180.0f * XM_PI;
	rotationMatrix = XMMatrixRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z);

	this->move = moveSpeed;

	moveMax = transform.position + move;
	moveMin = transform.position - move;

	pushVec = VECTOR3(0, 0, 0);
	refVec = VECTOR3(0, 0, 0);

	e = 0.6f;	// 反発係数	1で減衰なし
	f = 0.99f;	// 摩擦		1で減衰なし
}

MoveBox::~MoveBox()
{
}

void MoveBox::Update()
{
	transform.position += move;
	// ここでXYZごと制限まで動かす
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
		Box::CubeSize(vPos.x, vPos.y, vPos.z);		// 直方体のサイズと位置
		pushVec = Box::HitSphereToCubeplane(player->sphere, refVec);	// 面->辺->頂点の衝突判定
		player->PushVec(-pushVec, refVec);	// プレイヤーをめり込んだ量だけもどす
	}
#endif
}
