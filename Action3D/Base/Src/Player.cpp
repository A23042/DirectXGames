#include "Player.h"
#include "../Libs/Imgui/imgui.h"
#include "Dancer.h"
#include "Door.h"
#include "Box.h"

namespace { // ����cpp�ȊO�ł͎g���Ȃ�
	static const float Gravity = 9.8f * 3; // �d�͉����x(���̒l)
	// C++�̒萔��`�i�^���t���j
	static const float JumpPower = 0.3f;
	static const float RotationSpeed = 3.0f; // ��]���x(�x)
	static const float MoveSpeed = 0.8f;
};

Player::Player()
{
	animator = new Animator(); // �C���X�^���X���쐬

	mesh = new CFbxMesh();
	//mesh->Load("Data/Player/PlayerChara.mesh");
	mesh->Load("Data/Object/ball01.mesh");

	/*
	mesh->LoadAnimation(aIdle, "Data/Player/Idle.anmx", true);
	mesh->LoadAnimation(aRun, "Data/Player/Run.anmx", true);
	mesh->LoadAnimation(aAttack1, "Data/Player/attack1.anmx", false);
	mesh->LoadAnimation(aAttack2, "Data/Player/attack2.anmx", false);
	mesh->LoadAnimation(aAttack3, "Data/Player/attack3.anmx", false);
	animator->SetModel(mesh); // ���̃��f���ŃA�j���[�V��������
	animator->Play(aRun);
	animator->SetPlaySpeed(1.0f);
	*/

	state = sOnGround;
	//speedY = 0;

	sphere.center = transform.position;
	sphere.radius = 0.5f;
	sphere.velocity = VECTOR3(0, 0, 0);

	// �T�C�Y����
	//transform.scale = VECTOR3(2, 2, 2);
	sphere.radius *= transform.scale.x;
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

// �v���C���[���m�̏Փ˔����������
void Player::Update()
{
	sphere.velocity.y -= Gravity * SceneManager::DeltaTime();

	sphere.center += sphere.velocity * SceneManager::DeltaTime();
	transform.position = sphere.center;

	//animator->Update(); // ���t���[���AUpdate���Ă�
	switch (state) {
	case sOnGround:
		UpdateOnGround();
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
#if 0
	std::list<Object3D*> objects = ObjectManager::FindGameObjectsWithTag<Object3D>("STAGEOBJ"); // �h�A�̃I�u�W�F�N�g��������
	for (auto object : objects) {
		SphereCollider coll;
		coll.center = transform.position + VECTOR3(0, 0, 0); // �����̋������
		coll.radius = 0.5f;
		VECTOR3 push;
		if (object->HitSphereToMesh(coll, &push)) {
			move = push;
			//transform.position += move;
			//sphere.center = transform.position;
		}
	}

#endif

	// �v���C���[���m�̏Փ˔���
	std::list<Player*> players = ObjectManager::FindGameObjects<Player>();
	for (auto player : players) {
		Sphere tSph = player->sphere;
		VECTOR3 nVec = tSph.center - sphere.center;
		// �Փ�
		if (nVec.LengthSquare() <= tSph.radius + sphere.radius) {

			VECTOR3 pushVec = normalize(tSph.center - sphere.center) * (sphere.radius + tSph.radius - nVec.Length());
			sphere.center -= pushVec;
			transform.position = sphere.center;
			VECTOR3 refNormal = dot(sphere.velocity, nVec) * nVec;
			VECTOR3 refSessen = sphere.velocity - refNormal;
			VECTOR3 b = -refNormal + refSessen;
			sphere.velocity = b;
			//tSph.velocity = -b / 2;
		}
	}
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
	sphere.center += pushVec;
	transform.position = sphere.center;
	if (RefVec.Length() > 0) {
		sphere.velocity = RefVec;
	}
	return;
}

void Player::UpdateOnGround()
{
	//transform.position += move;
	if (GameDevice()->m_pDI->CheckKey(KD_DAT, DIK_W)) {
		// �O�p�֐��ł��ꍇ
//		position.z += cosf(rotation.y) * 0.1;
//		position.x += sinf(rotation.y) * 0.1;
		// �s��ł��ꍇ
		VECTOR3 forward = VECTOR3(0, 0, MoveSpeed); // ��]���ĂȂ����̈ړ���
		MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Y�̉�]�s��
		sphere.velocity += forward * rotY; // �L�����̌����Ă���ւ̈ړ����x
	} else if (GameDevice()->m_pDI->CheckKey(KD_DAT, DIK_S)) {
		// �O�p�֐��ł��ꍇ
//		position.z -= cosf(rotation.y) * 0.1;
//		position.x -= sinf(rotation.y) * 0.1;
		// �s��ł��ꍇ
		VECTOR3 forward = VECTOR3(0, 0, MoveSpeed); // ��]���ĂȂ����̈ړ���
		MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Y�̉�]�s��
		sphere.velocity += -forward * rotY; // �L�����̌����Ă���ւ̈ړ����x

	}
	else {
		animator->MergePlay(aIdle);
	}
	if (GameDevice()->m_pDI->CheckKey(KD_DAT, DIK_A)) {
		transform.rotation.y -= RotationSpeed / 180.0f * XM_PI;
	}
	if (GameDevice()->m_pDI->CheckKey(KD_DAT, DIK_D)) {
		transform.rotation.y += RotationSpeed / 180.0f * XM_PI;
	}
	//if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_SPACE)) {
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_SPACE)) {
		//transform.position.y += 0.1f;
		//sphere.center = transform.position;
		//speedY = JumpPower;
		state = sJump;
		//sphere.velocity.y += 15.0f * SceneManager::DeltaTime();
	}
	else if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_LSHIFT)) {
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
	state = sOnGround();
}

void Player::UpdateAttack()
{
	// �G�ɍU���𓖂Ă�
	std::list<Dancer*> dancers = ObjectManager::FindGameObjects<Dancer>();

	VECTOR3 tipPos = VECTOR3(0.9966, 0.6536, 0.140);
	MATRIX4X4 mat = transform.matrix(); // ���E�i���[���h�j�̒��ŁA�v���C���[�̈ʒu�ƌ���
	MATRIX4X4 bone = mesh->GetFrameMatrices(34); // �v���C���[�̌��_����̎��̈ʒu(34�͎��)
	VECTOR3 start = VECTOR3(0, 0, 0) * bone * mat;
	VECTOR3 end = tipPos * bone * mat;

	for (Dancer* d : dancers) {
		VECTOR3 hit;
		if (d->HitLineToMesh(start, end, &hit)) {
			d->AddDamage(10, transform.position); // �G�ɓ��Ă�
		}
	}
	if (animator->Finished()) {
		state = sOnGround;
	}
}
