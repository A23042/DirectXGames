#include "Player.h"
#include "../Libs/Imgui/imgui.h"
#include "Box.h"
#include "MoveBox.h"
#include "Ball.h"

namespace 
{	// このcpp以外では使えない
	static const float Gravity = 9.8f * 4.0f; // 重力加速度(正の値)
	// C++の定数定義（型が付く）
	static const float JumpPower = 12.0f;
	static const float RotationSpeed = 2.0f; // 回転速度(度)
	static const float MoveSpeed = 0.4f;
};

Player::Player(int num, bool isPhysic) : playerNum(num), isPhysic(isPhysic)
{
	//SetTag("STAGEOBJ");
	editObj.name = "Player";

	mesh = new CFbxMesh();
	mesh->Load("Data/Object/ball01.mesh");
	meshCol = new MeshCollider();
	meshCol->MakeFromMesh(mesh);

	state = sNormal;

	pObj.center = transform.position;
	pObj.radius = 0.5f;
	pObj.velocity = VECTOR3(0, 0, 0);

	pObj.isPlayer = true;
	pObj.pNum = playerNum;
}

Player::~Player()
{
	if (meshCol != nullptr) 
	{
		delete meshCol;
		meshCol = nullptr;
	}
}

void Player::Start()
{
	pObj.center = transform.position;
}

void Player::Update()
{
	if (isPhysic)
	{
		pObj.velocity.y -= Gravity * SceneManager::DeltaTime();
	}
	// 速度成分を坂の時考慮する
	// 面の法線に垂直なベクトル成分に進む
	pObj.center += pObj.velocity * SceneManager::DeltaTime();
	transform.position = pObj.center;

	if (isPhysic)
	{
		// 各Boxとの衝突判定
		std::list<Object3D*> objes = ObjectManager::FindGameObjectsWithTag<Object3D>("STAGEOBJ");
		for (Object3D* obj : objes) 
		{
			// 先にAABBと簡易的な衝突判定をして衝突していればHitSphereCubeplaneを回す
			if (obj->CheckSphereAABBCollision(this->pObj))
			{
				VECTOR3 refVec = VECTOR3(0, 0, 0);
				VECTOR3 pushVec = VECTOR3(0, 0, 0);
				pushVec = obj->HitSphereToCubeplane(this->pObj, refVec);
				if (pushVec.Length() > 0)
				{
					PushVec(-pushVec, refVec);
				}
			}
		}
		// Ball
		std::list<Ball*> balles = ObjectManager::FindGameObjects<Ball>();
		for (Ball* ball : balles) 
		{
			VECTOR3 refVec = VECTOR3(0, 0, 0);
			//VECTOR3 pushVec = VECTOR3(0, 0, 0);
			if (ball->HitSphereToSphere(this->pObj)) 
			{
				ball->SetPosition(ball->pObj.center);
				transform.position = pObj.center;
			}
		}
		// 自分以外のPlayerと衝突判定
		std::list<Player*> otherPlayers = ObjectManager::FindGameObjects<Player>();
		for (Player* otherplayer : otherPlayers)
		{
			if (otherplayer != this)
			{
				VECTOR3 refVec = VECTOR3(0, 0, 0);
				//VECTOR3 pushVec = VECTOR3(0, 0, 0);
				if (otherplayer->HitSphereToSphere(this->pObj)) 
				{
					otherplayer->SetPosition(otherplayer->pObj.center);
					transform.position = pObj.center;
				}
			}
		}


		//animator->Update(); // 毎フレーム、Updateを呼ぶ
		switch (state) 
		{
		case sNormal:
			UpdateNormal();
			break;
		case sJump:
			UpdateJump();
			break;
		}
		ImGui::Begin("POSITION");
		ImGui::InputFloat("X", &transform.position.x);
		ImGui::InputFloat("Y", &transform.position.y);
		ImGui::InputFloat("Z", &transform.position.z);
		ImGui::End();

		float velocity = pObj.velocity.Length();
		ImGui::Begin("VELOCITY");
		ImGui::InputFloat("Velocity", &velocity);
		//ImGui::InputFloat("X", &pObj.velocity.x);
		//ImGui::InputFloat("Y", &pObj.velocity.y);
		//ImGui::InputFloat("Z", &pObj.velocity.z);
		ImGui::End();

	}

	// ステージオブジェクトと衝突判定
#if 0
	std::list<Object3D*> objects = ObjectManager::FindGameObjectsWithTag<Object3D>("STAGEOBJ"); // ドアのオブジェクトを見つける
	for (auto object : objects) 
	{
		SphereCollider coll;
		coll.center = transform.position + VECTOR3(0, 0, 0); // 自分の球を作る
		coll.radius = 0.5f;
		VECTOR3 push;
		if (object->HitSphereToMesh(coll, &push)) 
		{
			transform.position += push;
			sphere.center = transform.position;
		}
	}

#endif
	// プレイヤー同士の衝突(いらないかも)
#if 0
	// プレイヤー同士の衝突判定
	std::list<Ball*> players = ObjectManager::FindGameObjects<Ball>();
	for (auto player : players) 
	{
			Sphere tSph = player->sphere;
			VECTOR3 nVec = tSph.center - this->sphere.center;
			float rsum = tSph.radius + this->sphere.radius;
			// 衝突
		if (nVec.LengthSquare() <= rsum * rsum)
		{
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
}

void Player::PushVec(VECTOR3 pushVec, VECTOR3 RefVec)
{	
	pObj.center += pushVec;
	transform.position = pObj.center;
	if (RefVec.Length() > 0) 
	{
		pObj.velocity = RefVec;
	}
	return;
}

// Playerの操作はコントローラーに変える
void Player::UpdateNormal()
{
	auto pDI = GameDevice()->m_pDI;

	// コントローラーのLスティックの入力状態を取る
	float LX = pDI->GetJoyState(playerNum).lX / 1000.0f;
	float LY = pDI->GetJoyState(playerNum).lY / 1000.0f;

	float RX = pDI->GetJoyState(playerNum).lZ / 32768.0f -1;
	float RY = pDI->GetJoyState(playerNum).lRz / 32768.0f -1;


	ImGui::Begin("JoyR");
	ImGui::InputFloat("RX", &RX);
	ImGui::InputFloat("RY", &RY);
	ImGui::InputFloat("LX", &LX);
	ImGui::InputFloat("LY", &LY);
	ImGui::End();


	if (fabs(LX) > 0 || fabs(LY) > 0)
	{
		VECTOR3 forward = VECTOR3(0, 0, MoveSpeed * -LY); // 回転してない時の移動量
		MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Yの回転行列
		pObj.velocity += forward * rotY; // キャラの向いてる方への移動速度

		VECTOR3 side = VECTOR3(MoveSpeed * LX, 0, 0); // 回転してない時の移動量
		pObj.velocity += side * rotY; // キャラの向いてる方への移動速度
	}
	if (fabs(RX) > 0)
	{
		transform.rotation.y += RotationSpeed * RX / 180.0f * XM_PI;
	}

	if (pDI->CheckJoy(KD_TRG, 0, playerNum))
	{
		VECTOR3 forward = VECTOR3(0, 0, MoveSpeed * 40); // 回転してない時の移動量
		MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Yの回転行列
		pObj.velocity += forward * rotY; // キャラの向いてる方への移動速度
	}
	if (pDI->CheckJoy(KD_TRG, 1, playerNum))
	{
		state = sJump;
	}
	
#if 0
	if(playerNum == 0)
	{

		if (pDI->CheckKey(KD_DAT, DIK_W))
		{
			// 行列でやる場合
			VECTOR3 forward = VECTOR3(0, 0, MoveSpeed); // 回転してない時の移動量
			MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Yの回転行列
			pObj.velocity += forward * rotY; // キャラの向いてる方への移動速度
		}
		else if (pDI->CheckKey(KD_DAT, DIK_S)) 
		{
			// 行列でやる場合
			VECTOR3 forward = VECTOR3(0, 0, MoveSpeed); // 回転してない時の移動量
			MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Yの回転行列
			pObj.velocity += -forward * rotY; // キャラの向いてる方への移動速度

		}

		if (pDI->CheckKey(KD_DAT, DIK_A)) 
		{
			transform.rotation.y -= RotationSpeed / 180.0f * XM_PI;
		}
		if (pDI->CheckKey(KD_DAT, DIK_D))
		{
			transform.rotation.y += RotationSpeed / 180.0f * XM_PI;
		}
		//if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_SPACE)) {
		if (pDI->CheckKey(KD_TRG, DIK_SPACE)) 
		{
			//transform.position.y += 0.1f;
			//sphere.center = transform.position;
			//speedY = JumpPower;
			//state = sJump;
			//sphere.velocity.y += 15.0f * SceneManager::DeltaTime();
		}
		else if (pDI->CheckKey(KD_TRG, DIK_LSHIFT)) 
		{
			state = sJump;
		}
		else if (pDI->CheckKey(KD_TRG, DIK_LCONTROL))
		{
			VECTOR3 forward = VECTOR3(0, 0, MoveSpeed * 40); // 回転してない時の移動量
			MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Yの回転行列
			pObj.velocity += forward * rotY; // キャラの向いてる方への移動速度
		}
	}
	else if (playerNum == 1)
	{
		if (pDI->CheckKey(KD_DAT, DIK_UP)) 
		{
			// 行列でやる場合
			VECTOR3 forward = VECTOR3(0, 0, MoveSpeed); // 回転してない時の移動量
			MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Yの回転行列
			pObj.velocity += forward * rotY; // キャラの向いてる方への移動速度
		}
		else if (pDI->CheckKey(KD_DAT, DIK_DOWN)) 
		{
			// 行列でやる場合
			VECTOR3 forward = VECTOR3(0, 0, MoveSpeed); // 回転してない時の移動量
			MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Yの回転行列
			pObj.velocity += -forward * rotY; // キャラの向いてる方への移動速度

		}

		if (pDI->CheckKey(KD_DAT, DIK_LEFT)) 
		{
			transform.rotation.y -= RotationSpeed / 180.0f * XM_PI;
		}
		if (pDI->CheckKey(KD_DAT, DIK_RIGHT)) 
		{
			transform.rotation.y += RotationSpeed / 180.0f * XM_PI;
		}
		//if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_SPACE)) {
		if (pDI->CheckKey(KD_TRG, DIK_SPACE)) 
		{
			//transform.position.y += 0.1f;
			//sphere.center = transform.position;
			//speedY = JumpPower;
			//state = sJump;
			//sphere.velocity.y += 15.0f * SceneManager::DeltaTime();
		}
		else if (pDI->CheckKey(KD_TRG, DIK_RSHIFT))
		{
			state = sJump;
		}
		else if (pDI->CheckKey(KD_TRG, DIK_RCONTROL))
		{
			VECTOR3 forward = VECTOR3(0, 0, MoveSpeed * 40); // 回転してない時の移動量
			MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Yの回転行列
			pObj.velocity += forward * rotY; // キャラの向いてる方への移動速度
		}
	}
#endif

}

void Player::UpdateJump()
{
	pObj.velocity.y = JumpPower;
	state = sNormal();
}
