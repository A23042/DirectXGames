#include "Player.h"
#include "../Libs/Imgui/imgui.h"
#include "Box.h"
#include "MoveBox.h"
#include "Score.h"
#include "OutlineBall.h"
#include "PlayScene.h"

namespace
{	// このcpp以外では使えない
	// C++の定数定義（型が付く）

	static const float Gravity = 9.8f * 4.0f;	// 重力加速度(正の値)
	static const float JumpPower = 12.0f;		// ジャンプ力
	static const float RotationSpeed = 1.2f;	// 回転速度(度)
	static const float MoveSpeed = 0.3f;		// 移動速度
	static const float MaxPushTime = 1.0f;		// 長押し上限
	static const float StopSpeed = 0.04f;		// 速度が遅くなったら0にするための基準
	static const float RDeadZone = 0.1;			// Rスティックのデッドゾーン
	static const float LDeadZone = 0.2f;			// Lスティックのデッドゾーン
	static const int Power = 180;				// 長押し発射の威力基準
	static const int RestShot = 3;				// 打てる回数
};

Player::Player(int num, bool isPhysic) : playerNum(num), isPhysic(isPhysic)
{
	SetTag("PLAYER");
	editObj.name = "Player";

	mesh = new CFbxMesh();
	if (playerNum == 0)
	{
		isMyTurn = true;
		mesh->Load("Data/Object/blueBall.mesh");
	}
	else
	{
		isMyTurn = false;
		mesh->Load("Data/Object/orengeBall.mesh");
	}
	new OutlineBall(this);
	meshCol = new MeshCollider();
	meshCol->MakeFromMesh(mesh);

	state = sNormal;

	pObj.center = transform.position;
	pObj.radius = 0.5f;
	pObj.velocity = VECTOR3(0, 0, 0);

	pObj.isPlayer = true;
	pObj.pNum = playerNum;

	myE = 0;
	myF = 0;
	restShot = RestShot;
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
	StartPos = transform.position;
	myE = pObj.e;
	myF = pObj.f;
	restShot = RestShot;

	// Update()内でFindGameObjectを極力しない
	//objes = ObjectManager::FindGameObjectsWithTag<Object3D>("STAGEOBJ");
	//areaes = ObjectManager::FindGameObjectsWithTag<ScoreArea>("SCOREAREA");
	lines = ObjectManager::FindGameObjects<Line>();
	collManager = ObjectManager::FindGameObject<CollisonManager>();
	loadStage = ObjectManager::FindGameObject<LoadStage>();
}

void Player::Update()
{
	// すでに他プレイヤー情報を持っていたらなんども探さない
	if (otherplayer == nullptr)
	{
		otherplayer = ObjectManager::FindOtherGameObject<Player>(this);
	}
	if (playerNum == 0)
	{
		ImGui::SetNextWindowPos(ImVec2(WINDOW_WIDTH / 5, WINDOW_HEIGHT - WINDOW_HEIGHT / 5));
		ImGui::SetNextWindowSize(ImVec2(150, 60));
		ImGui::Begin("PUSH0");
		ImGui::InputFloat("P0", &pushTime[0]);
	}
	else if (playerNum == 1)
	{
		ImGui::SetNextWindowPos(ImVec2(WINDOW_WIDTH - (WINDOW_WIDTH / 5), WINDOW_HEIGHT - WINDOW_HEIGHT / 5));
		ImGui::SetNextWindowSize(ImVec2(150, 60));
		ImGui::Begin("PUSH1");
		ImGui::InputFloat("P1", &pushTime[1]);
	}
	ImGui::End();

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
		for (Object3D* obj : collManager->GetBoxes())
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

		// ステータスで行う処理分け
		switch (state)
		{
		case sNormal:
			UpdateNormal();
			break;
		case sWait:
			UpdateWait();
			break;
		case sJump:
			UpdateJump();
			break;
		}
		//ImGui::Begin("POSITION");
		//ImGui::InputFloat("X", &transform.position.x);
		//ImGui::InputFloat("Y", &transform.position.y);
		//ImGui::InputFloat("Z", &transform.position.z);
		//ImGui::End();
	}
}

void Player::Draw()
{
	Object3D::Draw(); // 継承元の関数を呼ぶ
}

void Player::UpdateWait()
{
#if 0
	// 自分以外のPlayerと衝突判定
	//if (otherplayer != this)
	if (otherplayer != nullptr)
	{
		VECTOR3 refVec = VECTOR3(0, 0, 0);
		if (otherplayer->HitSphereToSphere(this->pObj))
		{
			otherplayer->SetPosition(otherplayer->pObj.center);
			transform.position = pObj.center;
		}
	}
#endif
	pObj.e = myE;
	pObj.f = myF;

	VECTOR3 sumVec = VECTOR3();

	// XZの速度がStopSpeed以下になったらXZ速度を0にする
	if (fabs(pObj.velocity.x) + fabs(pObj.velocity.z) <= StopSpeed)
	{
		pObj.velocity.x = 0;
		pObj.velocity.z = 0;
	}
	sumVec += pObj.velocity;

	// Ballとの衝突判定
	// BallのリストはCollisionManagerで管理している

	for (Ball* ball : collManager->GetBalls())
	{
		if (ball->HitSphereToSphere(this->pObj))
		{
			ball->SetPosition(ball->pObj.center);
			transform.position = pObj.center;
		}
		// XZの速度がStopSpeed以下になったらXZ速度を0にする
		if (fabs(ball->pObj.velocity.x) + fabs(ball->pObj.velocity.z) <= StopSpeed)
		{
			ball->pObj.velocity.x = 0;
			ball->pObj.velocity.z = 0;
		}
		sumVec += ball->pObj.velocity;
	}

	if (fabs(sumVec.x) + fabs(sumVec.z) == 0)
	{
		SetStartPos();
		return;
	}

	// スコアエリアの中にいるか
	for (ScoreArea* area : collManager->GetAreaes())
	{
		if (area->CheckSphereAABBCollision(this->pObj))
		{
			area->ScoreCount(this->pObj);
			break;
		}
		else
		{
			pObj.score = 0;
		}
	}

	auto pDI = GameDevice()->m_pDI;
	// DirectInput
	// コントローラーのLスティックの入力状態を取る
	float LX = pDI->GetJoyState(playerNum).lX / 1000.0f;
	float LY = -pDI->GetJoyState(playerNum).lY / 1000.0f;
	// コントローラーのRスティックの入力状態を取る
	float RX = pDI->GetJoyState(playerNum).lZ / 32768.0f - 1;
	float RY = -pDI->GetJoyState(playerNum).lRz / 32768.0f - 1;

	// コントローラーの接続数
	switch (pDI->GetJoyNum())
	{
	case 0:
		if (playerNum == 0)
		{
			if (pDI->CheckKey(KD_DAT, DIK_A))
			{
				tempRotSpeed.y = -1;
			}
			if (pDI->CheckKey(KD_DAT, DIK_D))
			{
				tempRotSpeed.y = 1;
			}
		}
		else if (playerNum == 1)
		{
			if (pDI->CheckKey(KD_DAT, DIK_LEFT))
			{
				tempRotSpeed.y = -1;
			}
			if (pDI->CheckKey(KD_DAT, DIK_RIGHT))
			{
				tempRotSpeed.y = 1;
			}
		}
		Move(VECTOR3(), tempRotSpeed);
		break;
	case 1:

		if (playerNum == 0)
		{
			if (fabs(RX) > RDeadZone)
			{
				tempRotSpeed.y = RX;
			}
		}

		if (playerNum == 1)
		{

			if (pDI->CheckKey(KD_DAT, DIK_A))
			{
				tempRotSpeed.y = -1;
			}
			if (pDI->CheckKey(KD_DAT, DIK_D))
			{
				tempRotSpeed.y = 1;
			}
		}
		Move(VECTOR3(), tempRotSpeed);
		break;
	case 2:
		if (fabs(RX) > RDeadZone)
		{
			tempRotSpeed.y = RX;
		}
		Move(VECTOR3(), tempRotSpeed);
		break;
	}
	tempRotSpeed = VECTOR3();
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

void Player::SetStartPos(bool isFall)
{
	restShot--;		// 残りショット数マイナス1

	// 残りショット数がなければリザルト画面へ
	if (otherplayer->restShot <= 0 && restShot <= 0)
	{
		Score* sc = ObjectManager::FindGameObject<Score>();
		sc->CountScore();
		//SceneManager::ChangeScene("ResultScene");
		loadStage->Load(2);
		return;
	}
	// 落下でなければその場にBallを設置する
	if (!isFall)
	{
		// 自分の場所を保存し、StartPosに戻してからBallに位置を入れる
		VECTOR3 temp = pObj.center;
		pObj.center = StartPos;

		// 速度と回転のリセット
		pObj.velocity = VECTOR3();
		transform.rotation = VECTOR3();

		// 設置したBallに情報を渡す
		myBall = new Ball(true, playerNum);
		collManager->AddBall(myBall);
		myBall->pObj.center = temp;
		myBall->SetPosition(myBall->pObj.center);
		myBall->pObj.e = pObj.e;
		myBall->pObj.f = pObj.f;

		pObj.score = 0;
		state = sNormal;
	}
	else
	{
		pObj.center = StartPos;
		pObj.velocity = VECTOR3();
		transform.rotation = VECTOR3();
		state = sNormal;
	}

}

void Player::Reset()
{
	if (playerNum == 0)
	{
		isMyTurn = true;
	}
	else
	{
		isMyTurn = false;
	}

	// 速度と回転のリセット
	pObj.velocity = VECTOR3();
	transform.rotation = VECTOR3();
	
	StartPos = transform.position;
	myE = pObj.e;
	myF = pObj.f;
	restShot = RestShot;

	//areaes = ObjectManager::FindGameObjectsWithTag<ScoreArea>("SCOREAREA");

	state = sNormal;
}

void Player::Move(VECTOR3 moveSpeed, VECTOR3 rotSpeed)
{
	VECTOR3 forward = VECTOR3(MoveSpeed * moveSpeed.x, MoveSpeed * moveSpeed.y, MoveSpeed * moveSpeed.z); // 回転してない時の移動量
	MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Yの回転行列
	pObj.velocity += forward * rotY; // キャラの向いてる方への移動速度

	transform.rotation.y += (RotationSpeed * rotSpeed.y) / 180.0f * XM_PI;
}

// Playerの操作はコントローラーに変える
void Player::UpdateNormal()
{
	pObj.e = 0.5;
	pObj.f = 0.08;

	// ショット前だけLineとの衝突判定
	for (Line* line : lines)
	{
		// 先にAABBと簡易的な衝突判定をして衝突していればHitSphereCubeplaneを回す
		if (line->CheckSphereAABBCollision(this->pObj))
		{
			VECTOR3 refVec = VECTOR3(0, 0, 0);
			VECTOR3 pushVec = VECTOR3(0, 0, 0);
			pushVec = line->HitSphereToCubeplane(this->pObj, refVec);
			if (pushVec.Length() > 0)
			{
				PushVec(-pushVec, VECTOR3());
			}
		}
	}

	auto pDI = GameDevice()->m_pDI;
	// DirectInput
	// コントローラーのLスティックの入力状態を取る
	float LX = pDI->GetJoyState(playerNum).lX / 1000.0f;
	float LY = -pDI->GetJoyState(playerNum).lY / 1000.0f;
	// コントローラーのRスティックの入力状態を取る
	float RX = pDI->GetJoyState(playerNum).lZ / 32768.0f - 1;
	float RY = -pDI->GetJoyState(playerNum).lRz / 32768.0f - 1;

	if (otherplayer->GetState() == 1)
	{
		isWait = true;
	}
	else
	{
		isWait = false;
	}

#if 0
	// XInput
	XInputGetState(playerNum, &m_state);

	// 左スティックの生の値
	SHORT lx = m_state.Gamepad.sThumbLX;
	SHORT ly = m_state.Gamepad.sThumbLY;
	// 右スティックの生の値
	SHORT rx = m_state.Gamepad.sThumbRX;
	SHORT ry = m_state.Gamepad.sThumbRY;

	// 正規化（-32768 ～ 32767 を -1.0 ～ 1.0 に変換）
	float Lx = lx / 32768.0f;
	float Ly = ly / 32768.0f;
	float Rx = rx / 32768.0f;
	float Ry = ry / 32768.0f;

	ImGui::Begin("JoyL");
	ImGui::InputFloat("LX", &Lx);
	ImGui::InputFloat("LY", &Ly);
	ImGui::InputFloat("RX", &Rx);
	ImGui::InputFloat("RY", &Ry);
	ImGui::End();
#endif

	// 振動のやり方
	// 有効な値の範囲は 0 ~ 65,535 です。
	//XINPUT_VIBRATION a;
	//a.wLeftMotorSpeed = 300;
	//XInputSetState(0, &a);

	//XInputGetKeystroke(playerNum, 0, &m_keystroke);

	// コントローラーの接続数
	switch (pDI->GetJoyNum())
	{
		// コントローラー接続数0の場合
		// Player0はWASD
		// Player1は十字
	case 0:
		if (playerNum == 0)
		{
			if (pDI->CheckKey(KD_DAT, DIK_W))
			{
				tempMoveSpeed.z = 1;
			}
			else if (pDI->CheckKey(KD_DAT, DIK_S))
			{
				tempMoveSpeed.z = -1;
			}
			if (pDI->CheckKey(KD_DAT, DIK_A))
			{
				tempRotSpeed.y = -1;
			}
			if (pDI->CheckKey(KD_DAT, DIK_D))
			{
				tempRotSpeed.y = 1;
			}
			else if (pDI->CheckKey(KD_TRG, DIK_LSHIFT))
			{
				state = sJump;
			}
			// 待ちではない　自分のターン　残り回数有
			if (!isWait && isMyTurn && restShot > 0)
			{
				if (pDI->CheckKey(KD_DAT, DIK_LCONTROL))
				{
					pushTime[playerNum] += SceneManager::DeltaTime();	// 押してる時間カウント
					// 最大を超えないように
					if (pushTime[playerNum] > MaxPushTime)
					{
						pushTime[playerNum] = MaxPushTime;
					}
				}
				// 離して放つ
				if (pDI->CheckKey(KD_UTRG, DIK_LCONTROL))
				{
					VECTOR3 forward = VECTOR3(0, 0, MoveSpeed * Power * pushTime[playerNum]); // 回転してない時の移動量
					MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Yの回転行列
					pObj.velocity += forward * rotY; // キャラの向いてる方への移動速度
					pushTime[playerNum] = 0;	// タイムカウントリセット
					isMyTurn = false;		// 自分のターン終了
					otherplayer->SetTurn();	// 相手のターン
					state = sWait;
				}
			}
		}
		else if (playerNum == 1)
		{
			if (pDI->CheckKey(KD_DAT, DIK_UP))
			{
				tempMoveSpeed.z = 1;
			}
			else if (pDI->CheckKey(KD_DAT, DIK_DOWN))
			{
				tempMoveSpeed.z = -1;
			}

			if (pDI->CheckKey(KD_DAT, DIK_LEFT))
			{
				tempRotSpeed.y = -1;
			}
			if (pDI->CheckKey(KD_DAT, DIK_RIGHT))
			{
				tempRotSpeed.y = 1;
			}
			else if (pDI->CheckKey(KD_TRG, DIK_RSHIFT))
			{
				state = sJump;
			}
			if (!isWait && isMyTurn && restShot > 0)
			{
				if (pDI->CheckKey(KD_DAT, DIK_RCONTROL))
				{
					pushTime[playerNum] += SceneManager::DeltaTime();
					if (pushTime[playerNum] > MaxPushTime)
					{
						pushTime[playerNum] = MaxPushTime;
					}
				}
				if (pDI->CheckKey(KD_UTRG, DIK_RCONTROL))
				{
					VECTOR3 forward = VECTOR3(0, 0, MoveSpeed * Power * pushTime[playerNum]); // 回転してない時の移動量
					MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Yの回転行列
					pObj.velocity += forward * rotY; // キャラの向いてる方への移動速度
					pushTime[playerNum] = 0;
					isMyTurn = false;		// 自分のターン終了
					otherplayer->SetTurn();	// 相手のターン
					state = sWait;
				}
			}
		}
		Move(tempMoveSpeed, tempRotSpeed);
		break;
		// コントローラー接続数1の場合
		// Player0はコントローラー
		// Player1はWASD
	case 1:
#if 0
		ImGui::Begin("JoyR");
		ImGui::InputFloat("RX", &RX);
		ImGui::InputFloat("RY", &RY);
		ImGui::InputFloat("LX", &LX);
		ImGui::InputFloat("LY", &LY);
		ImGui::End();

		if (playerNum == 0)
		{

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
				VECTOR3 forward = VECTOR3(0, 0, MoveSpeed * 60); // 回転してない時の移動量
				MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Yの回転行列
				pObj.velocity += forward * rotY; // キャラの向いてる方への移動速度
			}
			if (pDI->CheckJoy(KD_TRG, 1, playerNum))
			{
				state = sJump;
			}
		}
#endif

		if (playerNum == 0)
		{
			if (fabs(LX) > LDeadZone || fabs(LY) > LDeadZone)
			{
				tempMoveSpeed.z = LY;
				tempMoveSpeed.x = LX;
			}
			if (fabs(RX) > RDeadZone)
			{
				tempRotSpeed.y = RX;
			}
			if (!isWait && isMyTurn && restShot > 0)
			{
				if (pDI->CheckJoy(KD_DAT, DIJ_B, playerNum))
				{
					pushTime[playerNum] += SceneManager::DeltaTime();
					if (pushTime[playerNum] > MaxPushTime)
					{
						pushTime[playerNum] = MaxPushTime;
					}
				}
				if (pDI->CheckJoy(KD_UTRG, DIJ_B, playerNum))
				{
					VECTOR3 forward = VECTOR3(0, 0, MoveSpeed * Power * pushTime[playerNum]); // 回転してない時の移動量
					MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Yの回転行列
					pObj.velocity += forward * rotY; // キャラの向いてる方への移動速度
					pushTime[playerNum] = 0;
					isMyTurn = false;		// 自分のターン終了
					otherplayer->SetTurn();	// 相手のターン
					state = sWait;
				}
			}
			if (pDI->CheckJoy(KD_TRG, DIJ_A, playerNum))
			{
				state = sJump;
			}
		}

		if (playerNum == 1)
		{

			if (pDI->CheckKey(KD_DAT, DIK_W))
			{
				tempMoveSpeed.z = 1;
			}
			else if (pDI->CheckKey(KD_DAT, DIK_S))
			{
				tempMoveSpeed.z = -1;
			}

			if (pDI->CheckKey(KD_DAT, DIK_A))
			{
				tempRotSpeed.y = -1;
			}
			if (pDI->CheckKey(KD_DAT, DIK_D))
			{
				tempRotSpeed.y = 1;
			}
			else if (pDI->CheckKey(KD_TRG, DIK_LSHIFT))
			{
				state = sJump;
			}
			if (!isWait && isMyTurn && restShot > 0)
			{
				if (pDI->CheckKey(KD_DAT, DIK_LCONTROL))
				{
					pushTime[playerNum] += SceneManager::DeltaTime();
					if (pushTime[playerNum] > MaxPushTime)
					{
						pushTime[playerNum] = MaxPushTime;
					}
				}
				if (pDI->CheckKey(KD_UTRG, DIK_LCONTROL))
				{
					VECTOR3 forward = VECTOR3(0, 0, MoveSpeed * Power * pushTime[playerNum]); // 回転してない時の移動量
					MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Yの回転行列
					pObj.velocity += forward * rotY; // キャラの向いてる方への移動速度
					pushTime[playerNum] = 0;
					isMyTurn = false;		// 自分のターン終了
					otherplayer->SetTurn();	// 相手のターン
					state = sWait;
				}
			}
		}
		Move(tempMoveSpeed, tempRotSpeed);
		break;
		// コントローラー接続数2の場合
		//両Playerはコントローラー
	case 2:
		ImGui::Begin("JoyR");
		ImGui::InputFloat("RX", &RX);
		ImGui::InputFloat("RY", &RY);
		ImGui::InputFloat("LX", &LX);
		ImGui::InputFloat("LY", &LY);
		ImGui::End();


		if (fabs(LX) > LDeadZone || fabs(LY) > LDeadZone)
		{
			tempMoveSpeed.z = LY;
			tempMoveSpeed.x = LX;
		}
		if (fabs(RX) > RDeadZone)
		{
			tempRotSpeed.y = RX;
		}
		if (!isWait && isMyTurn && restShot > 0)
		{
			if (pDI->CheckJoy(KD_DAT, DIJ_B, playerNum))
			{
				pushTime[playerNum] += SceneManager::DeltaTime();
				if (pushTime[playerNum] > MaxPushTime)
				{
					pushTime[playerNum] = MaxPushTime;
				}
			}
			if (pDI->CheckJoy(KD_UTRG, DIJ_B, playerNum))
			{
				VECTOR3 forward = VECTOR3(0, 0, MoveSpeed * Power * pushTime[playerNum]); // 回転してない時の移動量
				MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Yの回転行列
				pObj.velocity += forward * rotY; // キャラの向いてる方への移動速度
				pushTime[playerNum] = 0;
				isMyTurn = false;		// 自分のターン終了
				otherplayer->SetTurn();	// 相手のターン
				state = sWait;
			}
		}

		if (pDI->CheckJoy(KD_TRG, DIJ_A, playerNum))
		{
			state = sJump;
		}
		Move(tempMoveSpeed, tempRotSpeed);
		break;
	}
	tempMoveSpeed = VECTOR3();
	tempRotSpeed = VECTOR3();
}

void Player::UpdateJump()
{
	//pObj.velocity.y = JumpPower;
	state = sNormal();
}
