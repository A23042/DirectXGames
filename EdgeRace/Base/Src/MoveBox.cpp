#include "MoveBox.h"
#include "Player.h"
// 平行移動Boxオブジェクト

MoveBox::MoveBox(VECTOR3 size, VECTOR3 rot, VECTOR3 move, VECTOR3 moveSpeed) : vPos(size / 2)
{
	SetTag("STAGEOBJ");
	editObj.name = "MoveBox";
	mesh = new CFbxMesh();
	mesh->Load("Data/Object/box00.mesh");

	transform.scale = size;

	// 回転角をラジアンに変換し、回転行列を作成
	transform.rotation.x += rot.x / 180.0f * XM_PI;
	transform.rotation.y += rot.y / 180.0f * XM_PI;
	transform.rotation.z += rot.z / 180.0f * XM_PI;
	rotationMatrix = XMMatrixRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z);

	this->moveSpeed = moveSpeed;

	moveMax = transform.position + move;
	moveMin = transform.position - move;

	pushVec = VECTOR3(0, 0, 0);
	refVec = VECTOR3(0, 0, 0);
	pObj.velocity = VECTOR3(0, 0, 0);
}

MoveBox::~MoveBox()
{
	if (mesh != nullptr)
	{
		delete mesh;
		mesh = nullptr;
	}
	if (meshCol != nullptr)
	{
		delete meshCol;
		meshCol = nullptr;
	}
}

void MoveBox::Start()
{
	pObj.center = transform.position;
}

void MoveBox::Update()
{
	pObj.center += pObj.velocity * SceneManager::DeltaTime();
	transform.position = pObj.center;

	// ここでXYZごと制限まで動かす
	if(moveMax.x > 0)
	{
		if (transform.position.x >= moveMax.x) {
			moveSpeed.x *= -1;
		}
		else if (transform.position.x <= moveMin.x) {
			moveSpeed.x *= -1;
		}
	}
	if (moveMax.y > 0)
	{
		if (transform.position.y >= moveMax.y) {
			moveSpeed.y *= -1;
		}
		else if (transform.position.y <= moveMin.y) {
			moveSpeed.y *= -1;
		}
	}
	if (moveMax.z > 0)
	{
		if (transform.position.z >= moveMax.z) {
			moveSpeed.z *= -1;
		}
		else if (transform.position.z <= moveMin.z) {
			moveSpeed.z *= -1;
		}
	}
	pObj.velocity = moveSpeed;

	Box::CubeSize(vPos.x, vPos.y, vPos.z);		// 直方体のサイズと位置

#if 0
	std::list<Player*> playeres =
		ObjectManager::FindGameObjects<Player>();
	for (Player* player : playeres) {
		VECTOR3 refVec = VECTOR3(0, 0, 0);
		pushVec = HitSphereToCubeplane(player->sphere, refVec);	// 面->辺->頂点の衝突判定
		player->PushVec(-pushVec, refVec);	// プレイヤーをめり込んだ量だけもどす
	}
#endif
}

VECTOR3 MoveBox::ReflectionVec(PhysicsObject& pOgj, VECTOR3 normal)
{
	// 法線ベクトルの方向の速度を考慮する
	VECTOR3 pushVecNormal = dot(pObj.velocity, normal) * normal;
	// 法線方向に反発係数をかける
	VECTOR3 refNormal = dot(pObj.velocity, normal) * normal - pushVecNormal;
	VECTOR3 refSessen = pObj.velocity - refNormal;
	float e2 = (this->pObj.e + pObj.e) / 2;
	float f2 = (this->pObj.f + pObj.f) / 2;
	VECTOR3 b = -refNormal * e2 + refSessen * f2;
	// 順番の修正
	// 埋め込みを解除->反射	〇
	// 反射->埋め込み解除		×
	return VECTOR3(b);
}
