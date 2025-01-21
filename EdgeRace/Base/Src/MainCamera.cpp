#include "MainCamera.h"

MainCamera::MainCamera()
{
	transform.position = VECTOR3(0, 5, -15);
	lookPosition = transform.position + VECTOR3(0, -3, 10);
	direction = VECTOR3(0, 0, 0);
	moveSpeed = 0.5f;
	rotationSpeed = 1.2f;
}

MainCamera::~MainCamera()
{
}

void MainCamera::Update()
{
	direction = VECTOR3(0, 0, 0);	// �������Z�b�g
	MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Y�̉�]�s��
	MATRIX4X4 rot = XMMatrixRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z);

	// �ړ����x�A���_���x�̐ݒ�
	ImGui::SetNextWindowPos(ImVec2(WINDOW_WIDTH - 300, 50));
	ImGui::SetNextWindowSize(ImVec2(280, 100));
	ImGui::Begin("SETTING");
	ImGui::SliderFloat("MoveSpeed", &moveSpeed, 0.1f, 1.0f, "%.1f");
	ImGui::SliderFloat("CameraSpeed", &rotationSpeed, 0.1f, 2.0f, "%.1f");
	ImGui::End();

	// ���_�ړ�
	// �J�[�\����1�t���[���O�̍��W�Ƃ̍��Ŏ��_�𓮂���
	// �E�N���b�N���Ă鎞�����ړ�����\�i�Q�l�FUnity�̃G�f�B�^�j
	if (pDI->CheckMouse(KD_DAT, 1))
	{
		mousePos1 = pDI->GetMousePos();

		float diffX = mousePos1.x - mousePos0.x;
		float diffY = mousePos1.y - mousePos0.y;
		transform.rotation.y += (diffX / 10) * rotationSpeed / 180.0f * XM_PI;
		transform.rotation.x += (diffY / 10) * rotationSpeed / 180.0f * XM_PI;

		// �J�����ړ�����
		if (pDI->CheckKey(KD_DAT, DIK_W))
		{
			direction += VECTOR3(0, 0, 1) * rot;
		}
		else if (pDI->CheckKey(KD_DAT, DIK_S))
		{
			direction += VECTOR3(0, 0, -1) * rot;
		}
		if (pDI->CheckKey(KD_DAT, DIK_D))
		{
			direction += VECTOR3(1, 0, 0) * rot;
		}
		else if (pDI->CheckKey(KD_DAT, DIK_A))
		{
			direction += VECTOR3(-1, 0, 0) * rot;
		}
		if (pDI->CheckKey(KD_DAT, DIK_E))
		{
			direction += VECTOR3(0, 1, 0) * rot;
		}
		else if (pDI->CheckKey(KD_DAT, DIK_Q))
		{
			direction += VECTOR3(0, -1, 0) * rot;
		}
	}

	VECTOR3 noramalVelocity = normalize(direction);	// �ړ������Ɉړ����x��������
	transform.position += noramalVelocity * moveSpeed;	// �ړ�
	lookPosition = transform.position + VECTOR3(0, -3, 10) * rot;	// �����_�����Ƀ}�g���N�X��������
	mousePos0 = pDI->GetMousePos();	// 1�t���[���O�̃}�E�X���W�ۊ�
}
