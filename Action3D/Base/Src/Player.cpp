#include "Player.h"
#include "../Libs/Imgui/imgui.h"
#include "Box.h"
#include "MoveBox.h"
#include "Ball.h"

namespace { // ����cpp�ȊO�ł͎g���Ȃ�
	static const float Gravity = 9.8f * 3; // �d�͉����x(���̒l)
	// C++�̒萔��`�i�^���t���j
	static const float JumpPower = 0.3f;
	static const float RotationSpeed = 3.0f; // ��]���x(�x)
	static const float MoveSpeed = 0.8f;
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

	// �T�C�Y����
	//pObj.radius *= transform.scale.x;

	pObj.isPlayer = true;
	pObj.pNum = playerNum;
}

Player::~Player()
{
	if (meshCol != nullptr) {
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
	// ���x��������̎��l������
	// �ʂ̖@���ɐ����ȃx�N�g�������ɐi��
	pObj.center += pObj.velocity * SceneManager::DeltaTime();
	transform.position = pObj.center;

	if (isPhysic)
	{
		// �����ŋ�ԕ������g���ď������ׂ̌y��������H
		// ��ԕ����悭�킩���ĂȂ�

		// �eBox�Ƃ̏Փ˔���
		std::list<Object3D*> objes = ObjectManager::FindGameObjectsWithTag<Object3D>("STAGEOBJ");
		for (Object3D* obj : objes) {
			VECTOR3 refVec = VECTOR3(0, 0, 0);
			VECTOR3 pushVec = VECTOR3(0, 0, 0);
			pushVec = obj->HitSphereToCubeplane(this->pObj, refVec);
			if (pushVec.Length() > 0)
			{
				PushVec(-pushVec, refVec);
			}
		}
		// Ball
		std::list<Ball*> balles = ObjectManager::FindGameObjects<Ball>();
		for (Ball* ball : balles) {
			VECTOR3 refVec = VECTOR3(0, 0, 0);
			//VECTOR3 pushVec = VECTOR3(0, 0, 0);
			if (ball->HitSphereToSphere(this->pObj)) {
				ball->SetPosition(ball->pObj.center);
				transform.position = pObj.center;
			}
		}
		// �����ȊO��Player�ƏՓ˔���
		std::list<Player*> otherPlayers = ObjectManager::FindGameObjects<Player>();
		for (Player* otherplayer : otherPlayers)
		{
			if (otherplayer != this)
			{
				VECTOR3 refVec = VECTOR3(0, 0, 0);
				//VECTOR3 pushVec = VECTOR3(0, 0, 0);
				if (otherplayer->HitSphereToSphere(this->pObj)) {
					otherplayer->SetPosition(otherplayer->pObj.center);
					transform.position = pObj.center;
				}
			}
		}


		//animator->Update(); // ���t���[���AUpdate���Ă�
		switch (state) {
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

		ImGui::Begin("VELOCITY");
		ImGui::InputFloat("X", &pObj.velocity.x);
		ImGui::InputFloat("Y", &pObj.velocity.y);
		ImGui::InputFloat("Z", &pObj.velocity.z);
		ImGui::End();

		ImGui::Begin("SUMVELOCITY");
		ImGui::InputFloat("X", &sumVelocity.x);
		ImGui::InputFloat("Y", &sumVelocity.y);
		ImGui::InputFloat("Z", &sumVelocity.z);
		ImGui::End();

	}


	// �_���T�[�Ƃ߂荞�܂Ȃ��悤�ɂ���
#if 0
	// Dancer�ɂ߂荞�܂Ȃ��悤�ɂ���
	// �����̍��W�́Atransform.position
	// Dancer�̍��W��m��
	std::list<Dancer*> dancers = 
			ObjectManager::FindGameObjects<Dancer>();
	// �@�C�e���[�^�[��
	//for (std::list<Dancer*>::iterator itr = dancers.begin();
	//				itr != dancers.end(); itr++) {
	//	Dancer* dancer = *itr;
	// �A�C�e���[�^�̕ύX
	//for (auto itr = dancers.begin(); itr != dancers.end(); itr++) {
	//	Dancer* dancer = *itr;
	// �Bfor(:)�ŉ�
	for (Dancer* dancer : dancers) {
	// �ȏ�R�o�[�W����
		SphereCollider tCol = dancer->Collider();
		SphereCollider pCol = Collider();
		VECTOR3 pushVec = pCol.center - tCol.center;
		float rSum = pCol.radius + tCol.radius;
		if (pushVec.LengthSquare() < rSum * rSum) { // ���̓����蔻��
			// �������Ă�
			// �����o��������pushVec
			// �����o�����������߂�
			float pushLen = rSum - pushVec.Length();
			pushVec = XMVector3Normalize(pushVec); // pushVec�̒������P�ɂ���
			transform.position += pushVec * pushLen;
		}
	}
#endif
	// �X�e�[�W�I�u�W�F�N�g�ƏՓ˔���
#if 0
	std::list<Object3D*> objects = ObjectManager::FindGameObjectsWithTag<Object3D>("STAGEOBJ"); // �h�A�̃I�u�W�F�N�g��������
	for (auto object : objects) {
		SphereCollider coll;
		coll.center = transform.position + VECTOR3(0, 0, 0); // �����̋������
		coll.radius = 0.5f;
		VECTOR3 push;
		if (object->HitSphereToMesh(coll, &push)) {
			transform.position += push;
			sphere.center = transform.position;
		}
	}

#endif
	// �v���C���[���m�̏Փ�(����Ȃ�����)
#if 0
	// �v���C���[���m�̏Փ˔���
	std::list<Ball*> players = ObjectManager::FindGameObjects<Ball>();
	for (auto player : players) {
			Sphere tSph = player->sphere;
			VECTOR3 nVec = tSph.center - this->sphere.center;
			float rsum = tSph.radius + this->sphere.radius;
			// �Փ�
		if (nVec.LengthSquare() <= rsum * rsum) {
			normalize(nVec);
			// �߂荞�݉���
			VECTOR3 pushVec = nVec * (this->sphere.radius + tSph.radius - (tSph.center - this->sphere.center).Length());
			this->sphere.center -= pushVec / 2.0f;
			tSph.center += pushVec / 2.0f;
			player->SetPosition(tSph.center);
			//b
			VECTOR3 refNormal = dot(tSph.velocity, nVec) * nVec;
			// �@�������ɔ����W����������
			VECTOR3 refSessen = sphere.velocity - refNormal;

			//c
			VECTOR3 CrefNormal = dot(this->sphere.velocity, nVec) * nVec;
			// �@�������ɔ����W����������
			VECTOR3 CrefSessen = tSph.velocity - CrefNormal;

			// �ӂ��̔����W���Ɩ��C�W���̕���
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
	
				// �@������
				VECTOR3 tPushVecNormal = dot(tSph.velocity, nVec) * nVec;
				VECTOR3 tRefNormal = dot(this->sphere.velocity, nVec) * nVec - tPushVecNormal;
				VECTOR3 tRefSessen = this->sphere.velocity - tRefNormal;

				VECTOR3 thisPushVecNormal = dot(this->sphere.velocity, nVec) * nVec;
				VECTOR3 thisRefNormal = dot(tSph.velocity, nVec) * nVec - thisPushVecNormal;
				VECTOR3 thisRefSessen = tSph.velocity - thisRefNormal;

				// �ӂ��̔����W���Ɩ��C�W���̕���
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
	Object3D::Draw(); // �p�����̊֐����Ă�
//	MATRIX4X4 tip = XMMatrixRotationRollPitchYawFromVector(VECTOR3(-33, 82, 0) * DegToRad);
//	VECTOR3 tipPos = VECTOR3(0, 0, 1.2f) * tip;
//	VECTOR3 tipPos = VECTOR3(0.9966, 0.6536, 0.140);
//	MATRIX4X4 mat = transform.matrix(); // ���E�i���[���h�j�̒��ŁA�v���C���[�̈ʒu�ƌ���
//	MATRIX4X4 bone = mesh->GetFrameMatrices(34); // �v���C���[�̌��_����̎��̈ʒu(34�͎��)
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
	pObj.center += pushVec;
	transform.position = pObj.center;
	if (RefVec.Length() > 0) {
		pObj.velocity = RefVec;
	}
	return;
}

void Player::UpdateNormal()
{
	auto pDI = GameDevice()->m_pDI;
	if(playerNum == 0)
	{

		if (pDI->CheckKey(KD_DAT, DIK_W)) {
			// �s��ł��ꍇ
			VECTOR3 forward = VECTOR3(0, 0, MoveSpeed); // ��]���ĂȂ����̈ړ���
			MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Y�̉�]�s��
			pObj.velocity += forward * rotY; // �L�����̌����Ă���ւ̈ړ����x
		}
		else if (pDI->CheckKey(KD_DAT, DIK_S)) {
			// �s��ł��ꍇ
			VECTOR3 forward = VECTOR3(0, 0, MoveSpeed); // ��]���ĂȂ����̈ړ���
			MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Y�̉�]�s��
			pObj.velocity += -forward * rotY; // �L�����̌����Ă���ւ̈ړ����x

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
	else if (playerNum == 1)
	{
		if (pDI->CheckKey(KD_DAT, DIK_UP)) {
			// �s��ł��ꍇ
			VECTOR3 forward = VECTOR3(0, 0, MoveSpeed); // ��]���ĂȂ����̈ړ���
			MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Y�̉�]�s��
			pObj.velocity += forward * rotY; // �L�����̌����Ă���ւ̈ړ����x
		}
		else if (pDI->CheckKey(KD_DAT, DIK_DOWN)) {
			// �s��ł��ꍇ
			VECTOR3 forward = VECTOR3(0, 0, MoveSpeed); // ��]���ĂȂ����̈ړ���
			MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Y�̉�]�s��
			pObj.velocity += -forward * rotY; // �L�����̌����Ă���ւ̈ړ����x

		}

		if (pDI->CheckKey(KD_DAT, DIK_LEFT)) {
			transform.rotation.y -= RotationSpeed / 180.0f * XM_PI;
		}
		if (pDI->CheckKey(KD_DAT, DIK_RIGHT)) {
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


}

void Player::UpdateJump()
{
	pObj.velocity.y = 10.0f;
	state = sNormal();
}
