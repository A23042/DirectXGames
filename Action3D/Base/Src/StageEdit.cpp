#include "StageEdit.h"
#include "Box.h"
#include "Ball.h"

#define EXTENDED_DISTANCE 50;	// Ray�̒���
#define EXTENDED_OBJ_DISTANCE 20;	// (��)�I�u�W�F�N�g�̈ړ��ꏊ

// �}�E�X�̃h���b�O�A���h�h���b�v�ŃX�e�[�W�I�u�W�F�N�g�̔z�u�����z
// Tank�̃��e�B�N���̉�ʍ��W�����[���h���W�ɕϊ�����̂��g�������H
// �I�u�W�F�N�g��z�u�A�폜������csv�̍s���l�߂���
StageEdit::StageEdit()
{
	
	GameDevice()->m_mView = XMMatrixLookAtLH(
		VECTOR3(15, 20, -15),	// �J�����ʒu
		VECTOR3(0, 0, 0),	// �����_
		VECTOR3(0, 1, 0));	// ��x�N�g��
	nState = sNone;
}

StageEdit::~StageEdit()
{
}

void StageEdit::Update()
{
	switch (nState)
	{
	case StageEdit::sNone:
		NoneUpdate();
		break;
	case StageEdit::sHas:
		HasUpdate();
		break;
	default:
		break;
	}
	GetWorldPos();
	int x = (int)mousePos.x;
	int y = (int)mousePos.y;
	ImGui::Begin("MOUSEPOS");
	ImGui::InputInt("X", &x);
	ImGui::InputInt("Y", &y);
	ImGui::End();
	
}
#if 1
void StageEdit::Draw()
{
	//CSprite cs;
	//cs.DrawLine3D(nearWorldPos, extendedFarWorldPos, RGB(0, 255, 0), 1.0f);
}
#endif
void StageEdit::NoneUpdate()
{
	if (GameDevice()->m_pDI->CheckMouse(KD_TRG, 0))
	{
		std::list<Object3D*> objs = ObjectManager::FindGameObjects<Object3D>();
		for (Object3D* ob : objs)
		{
			VECTOR3 hit;
			if (ob->HitLineToMesh(nearWorldPos, extendedFarWorldPos, &hit))
			{
				SetObj(ob);
				return;	// �ȉ��R�[�h�ȗ�
			}
		}
	}
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_1))
	{
		SetObj(new Ball());
		//new Ball();
	}
	else if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_2))
	{
		SetObj(new Box(VECTOR3(1, 1, 1), VECTOR3(0, 0, 0)));
	}
}

void StageEdit::HasUpdate()
{
	// �I������
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_N))
	{
		getObj = nullptr;
		nState = sNone;
		return;	// �ȉ��R�[�h�ȗ�
	}
	// �}�E�X���N���b�N
	if (GameDevice()->m_pDI->CheckMouse(KD_TRG, 0))
	{
		// �I�u�W�F�N�g�T��
		std::list<Object3D*> objs = ObjectManager::FindGameObjects<Object3D>();
		for (Object3D* ob : objs)
		{
			VECTOR3 hit;
			// �J�[�\���̃��[���h���W�̋ߎ��_���牓���_�̋�����L�΂����_�܂ł�Ray���΂���
			if (ob->HitLineToMesh(nearWorldPos, extendedFarWorldPos, &hit))
			{
				// ���ݑI����Ԃ̃I�u�W�F�N�g�̏ꍇ�������Ȃ�
				if (ob == getObj)
				{
					break;
				}
				// �Ⴄ�I�u�W�F�N�g���N���b�N���ꂽ��I���I�u�W�F�N�g�ύX
				SetObj(ob);
				return;	// �ȉ��R�[�h�ȗ�
			}
			else
			{
				//getObj = nullptr;
				//nState = sNone;
				//return;
			}
			 
		}
	}

	// ImGui�ŏꏊ�A��]�A�X�P�[����ς���
	// InputFloat�ł̐��l���͂��ł��Ȃ�����Ƃ肠����Int�^
	// �ꏊ
	ImGui::Begin("OBJINFO");
	ImGui::InputInt("PositionX", &objPosX);
	ImGui::InputInt("PositionY", &objPosY);
	ImGui::InputInt("PositionZ", &objPosZ);
	// ��]
	ImGui::InputInt("RotateX", &objRotX);
	ImGui::InputInt("RotateY", &objRotY);
	ImGui::InputInt("RotateZ", &objRotZ);
	// �X�P�[��
	ImGui::InputInt("ScaleX", &objScaleX);
	ImGui::InputInt("scaleY", &objScaleY);
	ImGui::InputInt("ScaleZ", &objScaleZ);
	ImGui::End();

	// ImGui�œ��͂��ꂽ�l���I�u�W�F�N�g�ɓK�p
	// �ꏊ��Object�������Ă���\���̂�Center�ɓK�p������
	// <-���ꂼ��̃I�u�W�F�N�g��Update��Center��transform.positinon�ɓK�p�����Ă邩��
	getObj->pObj.center = VECTOR3(objPosX, objPosY, objPosZ);	// �ꏊ

	// ��]�p�x�����W�A������x���ɕϊ�
	VECTOR3 Rot = getObj->Rotation();
	Rot.x = objRotX / 180.0f * XM_PI;
	Rot.y = objRotY / 180.0f * XM_PI;
	Rot.z = objRotZ / 180.0f * XM_PI;
	getObj->SetRotation(Rot);	// ��]
	getObj->SetScale(VECTOR3(objScaleX, objScaleY, objScaleZ));	// �X�P�[��

}

// �I�u�W�F�N�g�̑I��
void StageEdit::SetObj(Object3D* ob)
{
	// �I������Ă�I�u�W�F�N�g�̕ۑ�
	getObj = ob;

	// ���ꂼ��̒l��ImGui�œ��͂ł���悤��Int�^�ɕϊ�
	objPos = getObj->pObj.center;
	objPosX = (int)objPos.x;
	objPosY = (int)objPos.y;
	objPosZ = (int)objPos.z;

	objRot = getObj->Rotation();
	objRotX = (int)objRot.x;
	objRotY = (int)objRot.y;
	objRotZ = (int)objRot.z;

	objScale = getObj->Scale();
	objScaleX = (int)objScale.x;
	objScaleY = (int)objScale.y;
	objScaleZ = (int)objScale.z;

	nState = sHas;
}

// �}�E�X�J�[�\���̃��[���h���W�̎擾
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
	direction = XMVector3Normalize(farWorldPos - nearWorldPos);
	extendedFarWorldPos = nearWorldPos + direction * EXTENDED_DISTANCE;  // EXTENDED_DISTANCE�ŉ��΂�

	return farWorldPos;
}
