#include "Dancer.h"
#include "Player.h"
#include "Score.h"

Dancer::Dancer()
{
	animator = new Animator(); // インスタンスを作成

	mesh = new CFbxMesh();
	mesh->Load("Data/Dancer/Character.mesh");
	mesh->LoadAnimation(0, "Data/Dancer/Dancing.anmx", true);
	animator->SetModel(mesh); // このモデルでアニメーションする
	animator->Play(0);
	animator->SetPlaySpeed(1.0f);

	meshCol = new MeshCollider();
	meshCol->MakeFromMesh(mesh);

	transform.position = VECTOR3(20, 0, 0);
	transform.rotation = VECTOR3(0, 0, 0);

	state = sNormal;
	hitPoint = 30;

	aroundID = 0;
}

Dancer::~Dancer()
{
}

void Dancer::Start()
{
	areaCenter = transform.position;
}

void Dancer::Update()
{
	switch (state) {
	case sNormal:
		animator->Update();
		updateNormal();
		break;
	case sReach:
		updateReach();
		break;
	case sBack:
		updateBack();
		break;
	case sDead:
		updateDead();
		break;
	}
}

SphereCollider Dancer::Collider()
{
	SphereCollider col;
	col.center = transform.position;
	col.radius = 0.5f;
	return col;
}

void Dancer::AddDamage(float damage, VECTOR3 pPos)
{
	if (hitPoint <= 0) {	//すでにhitpointがなければ以下の処理を行わない
		return;
	}
	hitPoint -= damage;
	if (hitPoint > 0) {
		// プレイヤーの座標を手に入れる
		VECTOR3 push = transform.position - pPos;	// 飛ぶ方向のベクトルを作る
		push.y = 0;
		push = XMVector3Normalize(push) * 0.2f;	// そのベクトルの長さを0.2にする
		transform.position += push;	// transform.positionにベクトルを加える
		transform.rotation.y = atan2f(-push.x, -push.z);
	}
	else {
		Score* sc = ObjectManager::FindGameObject<Score>();
		sc->AddScore(100);

		VECTOR3 push = transform.position - pPos;	// 飛ぶ方向のベクトルを作る
		transform.rotation.y = atan2f(-push.x, -push.z);
		deadTimer = 0.0f;
		state = sDead;
	}
}

void Dancer::updateNormal()
{
	// 巡回ルート
	std::vector<VECTOR3> around = {
		VECTOR3(0,0,0),
		VECTOR3(0,0,2),
		VECTOR3(2,0,2),
		VECTOR3(0,0,2),
	};
	VECTOR3 target = around[aroundID] + areaCenter;
	moveToTarget(target);
	if ((target - transform.position).Length() < 0.5f) {
//		if (aroundID == around.size()-1)
//			aroundID = 0;
//		else
//			aroundID++;

//		if (++aroundID >= around.size())
//			aroundID = 0;

		aroundID = (aroundID + 1) % around.size();
	}

	// 自分の視野に入ったら、プレイヤーに向かってくる
	Player* player = ObjectManager::FindGameObject<Player>();
	VECTOR3 toPlayer = player->Position() - transform.position;
	if (toPlayer.Length() < 10.0f) {
		// 自分の正面のベクトルを作る
		VECTOR3 front = VECTOR3(0, 0, 1) * XMMatrixRotationY(transform.rotation.y);
		// プレイヤーへのベクトルの長さ１のもの
		VECTOR3 toPlayer1 = XMVector3Normalize(toPlayer);
		// 内積を取る →これがcosθ
		float ip = Dot(front, toPlayer1);
		if (ip >= cosf(60.0f * DegToRad)) {
			// 視野に入った
			state = sReach;
		}
	}
}

void Dancer::updateReach()
{
	Player* player = ObjectManager::FindGameObject<Player>();
	moveToTarget(player->Position());

	// エリアの外に出たら、stateをBackに変える→次からはupdateBackが呼ばれる
	VECTOR3 diff = areaCenter - transform.position;
	if (diff.Length() > 6.0f) {
		state = sBack;
	}
}

void Dancer::updateBack()
{
	// エリアの中心に向かって移動する
	moveToTarget(areaCenter);

	// エリアの中心に近づいたらstateをNormalに変える
	VECTOR3 diff = areaCenter - transform.position;
	if (diff.Length() < 1.0f) {
		state = sNormal;
	}
}

void Dancer::updateDead()
{
	deadTimer += SceneManager::DeltaTime();
	if (deadTimer <= 2.0f) {
		float rate = deadTimer / 2.0f;
		rate = rate * rate * rate;
		// 徐々に倒れる
		transform.rotation.x = -90.0f * DegToRad * rate;
	}
	else if (deadTimer >= 4.0f) {
		DestroyMe();
	}
}

void Dancer::moveToTarget(VECTOR3 target)
{
	VECTOR3 toTarget = target - transform.position;

	// 自分の正面のベクトルを作る
	MATRIX4X4 myRot = XMMatrixRotationY(transform.rotation.y);
	VECTOR3 front = VECTOR3(0, 0, 1) * myRot;
	transform.position += front * 0.01f; // とりあえず
	VECTOR3 right = VECTOR3(1, 0, 0) * myRot;

	// プレイヤーへのベクトルの長さ１のもの
	VECTOR3 toTarget1 = XMVector3Normalize(toTarget);
	// 内積を取る →これがcosθ
	float ip = Dot(front, toTarget1);
	float rSpeed = 3.0f * DegToRad;
	if (ip >= cosf(rSpeed)) {
		transform.rotation.y = atan2f(toTarget.x, toTarget.z);
	}
	else if (Dot(right, toTarget) > 0) {
		transform.rotation.y += rSpeed; // 右に曲げる
	}
	else {
		transform.rotation.y -= rSpeed; // 左に曲げる
	}
}
