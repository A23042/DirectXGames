#include "Player.h"
#include "../Libs/Imgui/imgui.h"
#include "Dancer.h"
#include "Door.h"
#include "Box.h"
#include "MoveBox.h"
#include "Ball.h"

namespace { // このcpp以外では使えない
	static const float Gravity = 9.8f * 3; // 重力加速度(正の値)
	// C++の定数定義（型が付く）
	static const float JumpPower = 0.3f;
	static const float RotationSpeed = 3.0f; // 回転速度(度)
	static const float MoveSpeed = 0.8f;
};

Player::Player(int num)
{
	playerNum = num;

	animator = new Animator(); // インスタンスを作成

	mesh = new CFbxMesh();
	//mesh->Load("Data/Player/PlayerChara.mesh");
	mesh->Load("Data/Object/ball01.mesh");

	/*
	mesh->LoadAnimation(aIdle, "Data/Player/Idle.anmx", true);
	mesh->LoadAnimation(aRun, "Data/Player/Run.anmx", true);
	mesh->LoadAnimation(aAttack1, "Data/Player/attack1.anmx", false);
	mesh->LoadAnimation(aAttack2, "Data/Player/attack2.anmx", false);
	mesh->LoadAnimation(aAttack3, "Data/Player/attack3.anmx", false);
	animator->SetModel(mesh); // このモデルでアニメーションする
	animator->Play(aRun);
	animator->SetPlaySpeed(1.0f);
	*/

	state = sNormal;
	//speedY = 0;

	sphere.center = transform.position;
	sphere.radius = 0.5f;
	sphere.velocity = VECTOR3(0, 0, 0);

	// サイズ調整
	//transform.scale = VECTOR3(2, 2, 2);
	sphere.radius *= transform.scale.x;

	sphere.isPlayer = true;
}

Player::~Player()
{
	if (meshCol != nullptr) {
		delete meshCol;
		meshCol = nullptr;
	}
	if (animator != nullptr) {
		delete animator;
		animator = nullptr;
	}
}

void Player::Start()
{
	sphere.center = transform.position;
}

void Player::Update()
{
	sphere.velocity.y -= Gravity * SceneManager::DeltaTime();
	// 速度成分を坂の時考慮する
	// 面の法線に垂直なベクトル成分に進む
	sphere.center += sphere.velocity * SceneManager::DeltaTime();
	transform.position = sphere.center;

	// ここで空間分割を使って処理負荷の軽減をする？
	// 空間分割よくわかってない

	// 各Boxとの衝突判定
	std::list<Object3D*> objes = ObjectManager::FindGameObjects<Object3D>();
	for (Object3D* obj : objes) {
		VECTOR3 refVec = VECTOR3(0, 0, 0);
		VECTOR3 pushVec = VECTOR3(0, 0, 0);
		pushVec = obj->HitSphereToCubeplane(this->sphere, refVec);
		PushVec(-pushVec, refVec);
	}
	// Ball
	std::list<Ball*> balles = ObjectManager::FindGameObjects<Ball>();
	for (Ball* ball : balles) {
		VECTOR3 refVec = VECTOR3(0, 0, 0);
		VECTOR3 pushVec = VECTOR3(0, 0, 0);
		//refVec = ball->HitPlayerToSphere(this->sphere, pushVec);
		if (ball->HitPlayerToSphere(this->sphere, pushVec)) {
			ball->sphere.center -= pushVec / 2;
			ball->SetPosition(ball->sphere.center);
			sphere.center += pushVec / 2;
			transform.position = sphere.center;
		}
		//transform.position = sphere.center;

		sumVelocity.x = abs(this->sphere.velocity.x) + abs(ball->sphere.velocity.x);
		sumVelocity.y = abs(this->sphere.velocity.y) + abs(ball->sphere.velocity.y);
		sumVelocity.z = abs(this->sphere.velocity.z) + abs(ball->sphere.velocity.z);

		//PushVec(pushVec, refVec);
		//ball->PushVec(-pushVec, -refVec);
	}

	//animator->Update(); // 毎フレーム、Updateを呼ぶ
	switch (state) {
	case sNormal:
		UpdateNormal();
		break;
	case sJump:
		UpdateJump();
		break;
	case sAttack:
		UpdateAttack();
		break;
	}
	ImGui::Begin("POSITION");
	ImGui::InputFloat("X", &transform.position.x);
	ImGui::InputFloat("Y", &transform.position.y);
	ImGui::InputFloat("Z", &transform.position.z);
	ImGui::End();

	ImGui::Begin("VELOCITY");
	ImGui::InputFloat("X", &sphere.velocity.x);
	ImGui::InputFloat("Y", &sphere.velocity.y);
	ImGui::InputFloat("Z", &sphere.velocity.z);
	ImGui::End();

	ImGui::Begin("SUMVELOCITY");
	ImGui::InputFloat("X", &sumVelocity.x);
	ImGui::InputFloat("Y", &sumVelocity.y);
	ImGui::InputFloat("Z", &sumVelocity.z);
	ImGui::End();

	// ダンサーとめり込まないようにする
#if 0
	// Dancerにめり込まないようにする
	// 自分の座標は、transform.position
	// Dancerの座標を知る
	std::list<Dancer*> dancers = 
			ObjectManager::FindGameObjects<Dancer>();
	// ①イテレーター版
	//for (std::list<Dancer*>::iterator itr = dancers.begin();
	//				itr != dancers.end(); itr++) {
	//	Dancer* dancer = *itr;
	// ②イテレータの変更
	//for (auto itr = dancers.begin(); itr != dancers.end(); itr++) {
	//	Dancer* dancer = *itr;
	// ③for(:)で回す
	for (Dancer* dancer : dancers) {
	// 以上３バージョン
		SphereCollider tCol = dancer->Collider();
		SphereCollider pCol = Collider();
		VECTOR3 pushVec = pCol.center - tCol.center;
		float rSum = pCol.radius + tCol.radius;
		if (pushVec.LengthSquare() < rSum * rSum) { // 球の当たり判定
			// 当たってる
			// 押し出す方向はpushVec
			// 押し出す長さを求める
			float pushLen = rSum - pushVec.Length();
			pushVec = XMVector3Normalize(pushVec); // pushVecの長さを１にする
			transform.position += pushVec * pushLen;
		}
	}
#endif
	// ステージオブジェクトと衝突判定
#if 0
	std::list<Object3D*> objects = ObjectManager::FindGameObjectsWithTag<Object3D>("STAGEOBJ"); // ドアのオブジェクトを見つける
	for (auto object : objects) {
		SphereCollider coll;
		coll.center = transform.position + VECTOR3(0, 0, 0); // 自分の球を作る
		coll.radius = 0.5f;
		VECTOR3 push;
		if (object->HitSphereToMesh(coll, &push)) {
			move = push;
			//transform.position += move;
			//sphere.center = transform.position;
		}
	}

#endif
	// プレイヤー同士の衝突(いらないかも)
#if 0
	// プレイヤー同士の衝突判定
	std::list<Ball*> players = ObjectManager::FindGameObjects<Ball>();
	for (auto player : players) {
			Sphere tSph = player->sphere;
			VECTOR3 nVec = tSph.center - this->sphere.center;
			float rsum = tSph.radius + this->sphere.radius;
			// 衝突
		if (nVec.LengthSquare() <= rsum * rsum) {
			normalize(nVec);
			// めり込み解消
			VECTOR3 pushVec = nVec * (this->sphere.radius + tSph.radius - (tSph.center - this->sphere.center).Length());
			this->sphere.center -= pushVec / 2.0f;
			tSph.center += pushVec / 2.0f;
			player->SetPosition(tSph.center);
			//b
			VECTOR3 refNormal = dot(tSph.velocity, nVec) * nVec;
			// 法線方向に反発係数をかける
			VECTOR3 refSessen = sphere.velocity - refNormal;

			//c
			VECTOR3 CrefNormal = dot(this->sphere.velocity, nVec) * nVec;
			// 法線方向に反発係数をかける
			VECTOR3 CrefSessen = tSph.velocity - CrefNormal;

			// ふたつの反発係数と摩擦係数の平均
			float e2 = (this->sphere.e + tSph.e) / 2;
			float f2 = (1.0f - this->sphere.f + 1.0f - tSph.f) / 2;

			VECTOR3 b = -refNormal *e2 + refSessen * f2;
			VECTOR3 c = -CrefNormal * e2 + CrefSessen * f2;
			//this->sphere.velocity = b;
			tSph.velocity = c;
#if 0
				normalize(nVec);
				VECTOR3 pushVec = normalize(tSph.center - this->sphere.center) * (this->sphere.radius + tSph.radius - nVec.Length());
				this->sphere.center -= pushVec;
				transform.position = this->sphere.center;
	
				// 法線方向
				VECTOR3 tPushVecNormal = dot(tSph.velocity, nVec) * nVec;
				VECTOR3 tRefNormal = dot(this->sphere.velocity, nVec) * nVec - tPushVecNormal;
				VECTOR3 tRefSessen = this->sphere.velocity - tRefNormal;

				VECTOR3 thisPushVecNormal = dot(this->sphere.velocity, nVec) * nVec;
				VECTOR3 thisRefNormal = dot(tSph.velocity, nVec) * nVec - thisPushVecNormal;
				VECTOR3 thisRefSessen = tSph.velocity - thisRefNormal;

				// ふたつの反発係数と摩擦係数の平均
				float e2 = (this->sphere.e + tSph.e) / 2;
				float f2 = (1.0f - this->sphere.f + 1.0f - tSph.f) / 2;

				VECTOR3 b = -tRefNormal * e2 - tRefSessen * f2;
				VECTOR3 c = -thisRefNormal * e2 - thisRefSessen * f2;

				this->sphere.velocity = b;
				tSph.velocity = c;


			}
#endif
		}
	}
#endif
}

void Player::Draw()
{
	Object3D::Draw(); // 継承元の関数を呼ぶ
//	MATRIX4X4 tip = XMMatrixRotationRollPitchYawFromVector(VECTOR3(-33, 82, 0) * DegToRad);
//	VECTOR3 tipPos = VECTOR3(0, 0, 1.2f) * tip;
//	VECTOR3 tipPos = VECTOR3(0.9966, 0.6536, 0.140);
//	MATRIX4X4 mat = transform.matrix(); // 世界（ワールド）の中で、プレイヤーの位置と向き
//	MATRIX4X4 bone = mesh->GetFrameMatrices(34); // プレイヤーの原点からの手首の位置(34は手首)
//	VECTOR3 start = VECTOR3(0, 0, 0) * bone * mat;
//	VECTOR3 end = tipPos * bone * mat;

//	CSprite spr;
	//spr.DrawLine3D(start, end, RGB(255, 0, 0), 0);
}

SphereCollider Player::Collider()
{
	SphereCollider col;
	col.center = transform.position;
	col.radius = 0.5f;
	return col;
}

void Player::PushVec(VECTOR3 pushVec, VECTOR3 RefVec)
{	
	sphere.center += pushVec;
	transform.position = sphere.center;
	if (RefVec.Length() > 0) {
		sphere.velocity = RefVec;
	}
	return;
}

void Player::UpdateNormal()
{
	auto pDI = GameDevice()->m_pDI;

	if (pDI->CheckKey(KD_DAT, DIK_W)) {
		// 行列でやる場合
		VECTOR3 forward = VECTOR3(0, 0, MoveSpeed); // 回転してない時の移動量
		MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Yの回転行列
		sphere.velocity += forward * rotY; // キャラの向いてる方への移動速度
	} else if (pDI->CheckKey(KD_DAT, DIK_S)) {
		// 行列でやる場合
		VECTOR3 forward = VECTOR3(0, 0, MoveSpeed); // 回転してない時の移動量
		MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Yの回転行列
		sphere.velocity += -forward * rotY; // キャラの向いてる方への移動速度

	}
	else {
		animator->MergePlay(aIdle);
	}
	if (pDI->CheckKey(KD_DAT, DIK_A)) {
		transform.rotation.y -= RotationSpeed / 180.0f * XM_PI;
	}
	if (pDI->CheckKey(KD_DAT, DIK_D)) {
		transform.rotation.y += RotationSpeed / 180.0f * XM_PI;
	}
	//if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_SPACE)) {
	if (pDI->CheckKey(KD_TRG, DIK_SPACE)) {
		//transform.position.y += 0.1f;
		//sphere.center = transform.position;
		//speedY = JumpPower;
		state = sJump;
		//sphere.velocity.y += 15.0f * SceneManager::DeltaTime();
	}
	else if (pDI->CheckKey(KD_TRG, DIK_LSHIFT)) {
		//transform.position.y -= 0.1f;
		//sphere.center = transform.position;
		//speedY = JumpPower;
		//state = sJump;
		//sphere.velocity.y -= 10.0f * SceneManager::DeltaTime();
	}


}

void Player::UpdateJump()
{
	sphere.velocity.y = 10.0f;
	state = sNormal();
}

void Player::UpdateAttack()
{
	// 敵に攻撃を当てる
	std::list<Dancer*> dancers = ObjectManager::FindGameObjects<Dancer>();

	VECTOR3 tipPos = VECTOR3(0.9966, 0.6536, 0.140);
	MATRIX4X4 mat = transform.matrix(); // 世界（ワールド）の中で、プレイヤーの位置と向き
	MATRIX4X4 bone = mesh->GetFrameMatrices(34); // プレイヤーの原点からの手首の位置(34は手首)
	VECTOR3 start = VECTOR3(0, 0, 0) * bone * mat;
	VECTOR3 end = tipPos * bone * mat;

	for (Dancer* d : dancers) {
		VECTOR3 hit;
		if (d->HitLineToMesh(start, end, &hit)) {
			d->AddDamage(10, transform.position); // 敵に当てた
		}
	}
	if (animator->Finished()) {
		state = sNormal;
	}
}
