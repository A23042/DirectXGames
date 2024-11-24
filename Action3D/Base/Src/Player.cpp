#include "Player.h"
#include "../Libs/Imgui/imgui.h"
#include "Box.h"
#include "MoveBox.h"
#include "Score.h"
#include "OutlineBall.h"
#include "PlayScene.h"

namespace
{	// ����cpp�ȊO�ł͎g���Ȃ�
	// C++�̒萔��`�i�^���t���j

	static const float Gravity = 9.8f * 4.0f;	// �d�͉����x(���̒l)
	static const float JumpPower = 12.0f;		// �W�����v��
	static const float RotationSpeed = 1.2f;	// ��]���x(�x)
	static const float MoveSpeed = 0.3f;		// �ړ����x
	static const float MaxPushTime = 1.0f;		// ���������
	static const float StopSpeed = 0.04f;		// ���x���x���Ȃ�����0�ɂ��邽�߂̊
	static const float RDeadZone = 0.1;			// R�X�e�B�b�N�̃f�b�h�]�[��
	static const float LDeadZone = 0.2f;			// L�X�e�B�b�N�̃f�b�h�]�[��
	static const int Power = 180;				// ���������˂̈З͊
	static const int RestShot = 3;				// �łĂ��
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

	// Update()����FindGameObject���ɗ͂��Ȃ�
	//objes = ObjectManager::FindGameObjectsWithTag<Object3D>("STAGEOBJ");
	//areaes = ObjectManager::FindGameObjectsWithTag<ScoreArea>("SCOREAREA");
	lines = ObjectManager::FindGameObjects<Line>();
	collManager = ObjectManager::FindGameObject<CollisonManager>();
	loadStage = ObjectManager::FindGameObject<LoadStage>();
}

void Player::Update()
{
	// ���łɑ��v���C���[���������Ă�����Ȃ�ǂ��T���Ȃ�
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
	// ���x��������̎��l������
	// �ʂ̖@���ɐ����ȃx�N�g�������ɐi��
	pObj.center += pObj.velocity * SceneManager::DeltaTime();
	transform.position = pObj.center;

	if (isPhysic)
	{
		// �eBox�Ƃ̏Փ˔���
		for (Object3D* obj : collManager->GetBoxes())
		{
			// ���AABB�ƊȈՓI�ȏՓ˔�������ďՓ˂��Ă����HitSphereCubeplane����
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

		// �X�e�[�^�X�ōs����������
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
	Object3D::Draw(); // �p�����̊֐����Ă�
}

void Player::UpdateWait()
{
#if 0
	// �����ȊO��Player�ƏՓ˔���
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

	// XZ�̑��x��StopSpeed�ȉ��ɂȂ�����XZ���x��0�ɂ���
	if (fabs(pObj.velocity.x) + fabs(pObj.velocity.z) <= StopSpeed)
	{
		pObj.velocity.x = 0;
		pObj.velocity.z = 0;
	}
	sumVec += pObj.velocity;

	// Ball�Ƃ̏Փ˔���
	// Ball�̃��X�g��CollisionManager�ŊǗ����Ă���

	for (Ball* ball : collManager->GetBalls())
	{
		if (ball->HitSphereToSphere(this->pObj))
		{
			ball->SetPosition(ball->pObj.center);
			transform.position = pObj.center;
		}
		// XZ�̑��x��StopSpeed�ȉ��ɂȂ�����XZ���x��0�ɂ���
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

	// �X�R�A�G���A�̒��ɂ��邩
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
	// �R���g���[���[��L�X�e�B�b�N�̓��͏�Ԃ����
	float LX = pDI->GetJoyState(playerNum).lX / 1000.0f;
	float LY = -pDI->GetJoyState(playerNum).lY / 1000.0f;
	// �R���g���[���[��R�X�e�B�b�N�̓��͏�Ԃ����
	float RX = pDI->GetJoyState(playerNum).lZ / 32768.0f - 1;
	float RY = -pDI->GetJoyState(playerNum).lRz / 32768.0f - 1;

	// �R���g���[���[�̐ڑ���
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
	restShot--;		// �c��V���b�g���}�C�i�X1

	// �c��V���b�g�����Ȃ���΃��U���g��ʂ�
	if (otherplayer->restShot <= 0 && restShot <= 0)
	{
		Score* sc = ObjectManager::FindGameObject<Score>();
		sc->CountScore();
		//SceneManager::ChangeScene("ResultScene");
		loadStage->Load(2);
		return;
	}
	// �����łȂ���΂��̏��Ball��ݒu����
	if (!isFall)
	{
		// �����̏ꏊ��ۑ����AStartPos�ɖ߂��Ă���Ball�Ɉʒu������
		VECTOR3 temp = pObj.center;
		pObj.center = StartPos;

		// ���x�Ɖ�]�̃��Z�b�g
		pObj.velocity = VECTOR3();
		transform.rotation = VECTOR3();

		// �ݒu����Ball�ɏ���n��
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

	// ���x�Ɖ�]�̃��Z�b�g
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
	VECTOR3 forward = VECTOR3(MoveSpeed * moveSpeed.x, MoveSpeed * moveSpeed.y, MoveSpeed * moveSpeed.z); // ��]���ĂȂ����̈ړ���
	MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Y�̉�]�s��
	pObj.velocity += forward * rotY; // �L�����̌����Ă���ւ̈ړ����x

	transform.rotation.y += (RotationSpeed * rotSpeed.y) / 180.0f * XM_PI;
}

// Player�̑���̓R���g���[���[�ɕς���
void Player::UpdateNormal()
{
	pObj.e = 0.5;
	pObj.f = 0.08;

	// �V���b�g�O����Line�Ƃ̏Փ˔���
	for (Line* line : lines)
	{
		// ���AABB�ƊȈՓI�ȏՓ˔�������ďՓ˂��Ă����HitSphereCubeplane����
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
	// �R���g���[���[��L�X�e�B�b�N�̓��͏�Ԃ����
	float LX = pDI->GetJoyState(playerNum).lX / 1000.0f;
	float LY = -pDI->GetJoyState(playerNum).lY / 1000.0f;
	// �R���g���[���[��R�X�e�B�b�N�̓��͏�Ԃ����
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

	// ���X�e�B�b�N�̐��̒l
	SHORT lx = m_state.Gamepad.sThumbLX;
	SHORT ly = m_state.Gamepad.sThumbLY;
	// �E�X�e�B�b�N�̐��̒l
	SHORT rx = m_state.Gamepad.sThumbRX;
	SHORT ry = m_state.Gamepad.sThumbRY;

	// ���K���i-32768 �` 32767 �� -1.0 �` 1.0 �ɕϊ��j
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

	// �U���̂���
	// �L���Ȓl�͈̔͂� 0 ~ 65,535 �ł��B
	//XINPUT_VIBRATION a;
	//a.wLeftMotorSpeed = 300;
	//XInputSetState(0, &a);

	//XInputGetKeystroke(playerNum, 0, &m_keystroke);

	// �R���g���[���[�̐ڑ���
	switch (pDI->GetJoyNum())
	{
		// �R���g���[���[�ڑ���0�̏ꍇ
		// Player0��WASD
		// Player1�͏\��
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
			// �҂��ł͂Ȃ��@�����̃^�[���@�c��񐔗L
			if (!isWait && isMyTurn && restShot > 0)
			{
				if (pDI->CheckKey(KD_DAT, DIK_LCONTROL))
				{
					pushTime[playerNum] += SceneManager::DeltaTime();	// �����Ă鎞�ԃJ�E���g
					// �ő�𒴂��Ȃ��悤��
					if (pushTime[playerNum] > MaxPushTime)
					{
						pushTime[playerNum] = MaxPushTime;
					}
				}
				// �����ĕ���
				if (pDI->CheckKey(KD_UTRG, DIK_LCONTROL))
				{
					VECTOR3 forward = VECTOR3(0, 0, MoveSpeed * Power * pushTime[playerNum]); // ��]���ĂȂ����̈ړ���
					MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Y�̉�]�s��
					pObj.velocity += forward * rotY; // �L�����̌����Ă���ւ̈ړ����x
					pushTime[playerNum] = 0;	// �^�C���J�E���g���Z�b�g
					isMyTurn = false;		// �����̃^�[���I��
					otherplayer->SetTurn();	// ����̃^�[��
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
					VECTOR3 forward = VECTOR3(0, 0, MoveSpeed * Power * pushTime[playerNum]); // ��]���ĂȂ����̈ړ���
					MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Y�̉�]�s��
					pObj.velocity += forward * rotY; // �L�����̌����Ă���ւ̈ړ����x
					pushTime[playerNum] = 0;
					isMyTurn = false;		// �����̃^�[���I��
					otherplayer->SetTurn();	// ����̃^�[��
					state = sWait;
				}
			}
		}
		Move(tempMoveSpeed, tempRotSpeed);
		break;
		// �R���g���[���[�ڑ���1�̏ꍇ
		// Player0�̓R���g���[���[
		// Player1��WASD
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
				VECTOR3 forward = VECTOR3(0, 0, MoveSpeed * -LY); // ��]���ĂȂ����̈ړ���
				MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Y�̉�]�s��
				pObj.velocity += forward * rotY; // �L�����̌����Ă���ւ̈ړ����x

				VECTOR3 side = VECTOR3(MoveSpeed * LX, 0, 0); // ��]���ĂȂ����̈ړ���
				pObj.velocity += side * rotY; // �L�����̌����Ă���ւ̈ړ����x
			}
			if (fabs(RX) > 0)
			{
				transform.rotation.y += RotationSpeed * RX / 180.0f * XM_PI;
			}

			if (pDI->CheckJoy(KD_TRG, 0, playerNum))
			{
				VECTOR3 forward = VECTOR3(0, 0, MoveSpeed * 60); // ��]���ĂȂ����̈ړ���
				MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Y�̉�]�s��
				pObj.velocity += forward * rotY; // �L�����̌����Ă���ւ̈ړ����x
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
					VECTOR3 forward = VECTOR3(0, 0, MoveSpeed * Power * pushTime[playerNum]); // ��]���ĂȂ����̈ړ���
					MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Y�̉�]�s��
					pObj.velocity += forward * rotY; // �L�����̌����Ă���ւ̈ړ����x
					pushTime[playerNum] = 0;
					isMyTurn = false;		// �����̃^�[���I��
					otherplayer->SetTurn();	// ����̃^�[��
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
					VECTOR3 forward = VECTOR3(0, 0, MoveSpeed * Power * pushTime[playerNum]); // ��]���ĂȂ����̈ړ���
					MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Y�̉�]�s��
					pObj.velocity += forward * rotY; // �L�����̌����Ă���ւ̈ړ����x
					pushTime[playerNum] = 0;
					isMyTurn = false;		// �����̃^�[���I��
					otherplayer->SetTurn();	// ����̃^�[��
					state = sWait;
				}
			}
		}
		Move(tempMoveSpeed, tempRotSpeed);
		break;
		// �R���g���[���[�ڑ���2�̏ꍇ
		//��Player�̓R���g���[���[
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
				VECTOR3 forward = VECTOR3(0, 0, MoveSpeed * Power * pushTime[playerNum]); // ��]���ĂȂ����̈ړ���
				MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Y�̉�]�s��
				pObj.velocity += forward * rotY; // �L�����̌����Ă���ւ̈ړ����x
				pushTime[playerNum] = 0;
				isMyTurn = false;		// �����̃^�[���I��
				otherplayer->SetTurn();	// ����̃^�[��
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
