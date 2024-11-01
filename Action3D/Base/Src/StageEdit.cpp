#include "StageEdit.h"
#include "Box.h"
#include "Ball.h"

#define EXTENDED_DISTANCE 50;

// �}�E�X�̃h���b�O�A���h�h���b�v�ŃX�e�[�W�I�u�W�F�N�g�̔z�u�����z
// Tank�̃��e�B�N���̉�ʍ��W�����[���h���W�ɕϊ�����̂��g�������H
// �I�u�W�F�N�g��z�u�A�폜������csv�̍s���l�߂���
StageEdit::StageEdit()
{
	//obj =new Box(VECTOR3(1,1,1), VECTOR3(0,0,0));
	new Ball;
	obj = new Ball();
	GameDevice()->m_mView = XMMatrixLookAtLH(
		VECTOR3(0, 20, -10),	// �J�����ʒu
		VECTOR3(0, 0, 0),	// �����_
		VECTOR3(0, 1, 1));	// ��x�N�g��
	GameDevice()->m_vEyePt = VECTOR3(0, 20, -10);
}

StageEdit::~StageEdit()
{
}

void StageEdit::Update()
{
	GetWorldPos();
	int x = (int)mousePos.x;
	int y = (int)mousePos.y;
	ImGui::Begin("MOUSEPOS");
	ImGui::InputInt("X", &x);
	ImGui::InputInt("Y", &y);
	//ImGui::InputFloat("Z", &worldPos.z);
	ImGui::End();
}

void StageEdit::Draw()
{
	CSprite cs;
	cs.DrawLine3D(nearWorldPos, extendedFarWorldPos, RGB(0, 255, 0), 1.0f);
	//cs.DrawLine3D(GameDevice()->m_vEyePt, farWorldPos, RGB(0, 255, 0), 1.0f);
	obj->SetPosition(extendedFarWorldPos);
}

VECTOR3 StageEdit::GetWorldPos()
{
	// �}�E�X���W�擾
	mousePos = GameDevice()->m_pDI->GetMousePos();

	// �e�s��擾
	MATRIX4X4 mView = GameDevice()->m_mView;
	MATRIX4X4 mPrj = GameDevice()->m_mProj;
	MATRIX4X4 identity = XMMatrixIdentity();
	
	// �ߎ��_(0)�Ɖ����_(1)
	nearWorldPos = XMVector3Unproject(VECTOR3(mousePos.x, mousePos.y, 0.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	farWorldPos = XMVector3Unproject(VECTOR3(mousePos.x, mousePos.y, 1.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	
	// �����x�N�g���𐳋K�����Ē��������΂�
	VECTOR3 direction = XMVector3Normalize(farWorldPos - nearWorldPos);
	extendedFarWorldPos = nearWorldPos + direction * EXTENDED_DISTANCE;  // EXTENDED_DISTANCE�ŉ��΂�


	// �J��������}�E�X���W��Ray���΂���LineToMesh�ŏՓ˂����I�u�W�F�N�g�� Object* obj �ɓ����
	// �}�E�X�𓮂�������Ǐ]������
	std::list<Object3D*> objs = ObjectManager::FindGameObjects<Object3D>();
	for (Object3D* ob : objs)
	{
		VECTOR3 hit;
		if (GameDevice()->m_pDI->CheckMouse(KD_TRG, 0))
		{
			if (ob->HitLineToMesh(nearWorldPos, extendedFarWorldPos, &hit))
			{
				ob->DestroyMe();
			}
		}
	}
	

	return farWorldPos;
}
