#include "StageEdit.h"
#include "Box.h"

// �}�E�X�̃h���b�O�A���h�h���b�v�ŃX�e�[�W�I�u�W�F�N�g�̔z�u�����z
// Tank�̃��e�B�N���̉�ʍ��W�����[���h���W�ɕϊ�����̂��g�������H
// �I�u�W�F�N�g��z�u�A�폜������csv�̍s���l�߂���
StageEdit::StageEdit()
{
	obj =new Box(VECTOR3(1,1,1), VECTOR3(0,0,0));

	GameDevice()->m_mView = XMMatrixLookAtLH(
		VECTOR3(0, 20, -10),	// �J�����ʒu
		VECTOR3(0, 0, 0),	// �����_
		VECTOR3(0, 1, 1));	// ��x�N�g��
}

StageEdit::~StageEdit()
{
}

void StageEdit::Update()
{
	GetWorldPos();
	//obj->SetPosition(VECTOR3(0, 0, 0));
	obj->SetPosition(VECTOR3(worldPos.x,0,worldPos.z));

	ImGui::Begin("WORLDPOS");
	ImGui::InputFloat("X", &worldPos.x);
	ImGui::InputFloat("Y", &worldPos.y);
	ImGui::InputFloat("Z", &worldPos.z);
	ImGui::End();
	ImGui::Begin("MOUSEPOS");
	ImGui::InputFloat("X", &mPos.x);
	ImGui::InputFloat("Y", &mPos.y);
	//ImGui::InputFloat("Z", &worldPos.z);
	ImGui::End();
}

void StageEdit::Draw()
{
}

VECTOR3 StageEdit::GetWorldPos()
{
	MATRIX4X4 mView = GameDevice()->m_mView;
	MATRIX4X4 mInv = XMMatrixInverse(nullptr, mView);
	// ��View�̋t�s��
	// 
	// �}�E�X���W�擾�@���_�̓E�B���h�E�̍���
	POINT mousePos;
	GetCursorPos(&mousePos);

	ScreenToClient(GameDevice()->m_pMain->m_hWnd, &mousePos);
	
	// �J��������}�E�X���W��Ray���΂���LineToMesh�ŏՓ˂����I�u�W�F�N�g�� Object* obj �ɓ����
	// �}�E�X�𓮂�������Ǐ]������
	
	return worldPos;
}
