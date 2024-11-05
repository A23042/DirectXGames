#include "StageEdit.h"
#include "Box.h"
#include "Ball.h"
#include "MoveBox.h"
#include "Player.h"
#include "Gizmo3D.h"
#include "GizmoXYZ.h"
#include <fstream>

#define EXTENDED_DISTANCE 50;	// Ray�̒���
#define EXTENDED_OBJ_DISTANCE 20;	// (��)�I�u�W�F�N�g�̈ړ��ꏊ

// �}�E�X�̃h���b�O�A���h�h���b�v�ŃX�e�[�W�I�u�W�F�N�g�̔z�u�����z
// Tank�̃��e�B�N���̉�ʍ��W�����[���h���W�ɕϊ�����̂��g�������H
// �I�u�W�F�N�g��z�u�A�폜������csv�̍s���l�߂���
StageEdit::StageEdit()
{
	gizmoObj = new Gizmo3D();
	gizmoObj->SetScale(VECTOR3(0.0015f, 0.0015f, 0.0015f));
	gizmoObj->SetRotation(VECTOR3(0, 180.0f / 180 * XM_PI, 0));
	//GameDevice()->m_mView = XMMatrixLookAtLH(
	//	VECTOR3(10, 20, -15),	// �J�����ʒu
	//	VECTOR3(0, 0, 0),	// �����_
	//	VECTOR3(0, 1, 0));	// ��x�N�g��
	nState = sNone;
}

StageEdit::~StageEdit()
{
}

void StageEdit::Update()
{
	mView = GameDevice()->m_mView;
	mPrj = GameDevice()->m_mProj;
	identity = XMMatrixIdentity();
	GetWorldPos();

	// 3DGizmo�\���ʒu
	// Window�̍���
	VECTOR3 windowWorldPos = XMVector3Unproject(VECTOR3(100, WINDOW_HEIGHT - 100, 0.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	gizmoObj->SetPosition(windowWorldPos);
	
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

	// �I�u�W�F�N�g�쐬�{�^��ImGui
	ImGui::SetNextWindowPos(ImVec2(30, 30));
	ImGui::SetNextWindowSize(ImVec2(130, 130));
	ImGui::Begin("NEWOBJ");
	if (ImGui::Button("Box"))
	{
		SetObj(new Box(VECTOR3(1,1,1)));
	}
	if (ImGui::Button("MoveBox"))
	{
		SetObj(new MoveBox(VECTOR3(1, 1, 1)));
	}
	if (ImGui::Button("Ball"))
	{
		SetObj(new Ball(false));
	}
	if(pNum == 0)
	{
		if (ImGui::Button("Player"))
		{
			SetObj(new Player(pNum,false));
			pNum++;
		}
	}
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
	// �I�u�W�F�N�g�폜
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_DELETE))
	{
		getObj->DestroyMe();
		getObj = nullptr;
		nState = sNone;
		return;
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

	// Ctrl + D �I�u�W�F�N�g����
	if (pDI->CheckKey(KD_DAT, DIK_LCONTROL) && pDI->CheckKey(KD_TRG, DIK_D))
	{
		DupeObj(getObj);
		return;	// �ȉ��R�[�h�ȗ�
	}

	// ImGui�ŏꏊ�A��]�A�X�P�[����ς���
	ImGui::SetNextWindowPos(ImVec2(30, 170));
	ImGui::SetNextWindowSize(ImVec2(290, 310));
	ImGui::Begin("OBJINFO");
	// �����W���E���C�E����
	ImGui::SliderFloat("e", &e, 0.0f, 1.0f);
	ImGui::SliderFloat("f", &f, 0.0f, 1.0f);
	ImGui::InputFloat("mass", &mass, 0.5f, 1.0f);
	// �ꏊ
	ImGui::InputFloat("PositionX", &objPos.x, 0.5f);
	ImGui::InputFloat("PositionY", &objPos.y, 0.5f);
	ImGui::InputFloat("PositionZ", &objPos.z, 0.5f);
	// ��]
	ImGui::InputFloat("RotateX", &objRot.x, 0.5f);
	ImGui::InputFloat("RotateY", &objRot.y, 0.5f);
	ImGui::InputFloat("RotateZ", &objRot.z, 0.5f);
	// �X�P�[��
	ImGui::InputFloat("ScaleX", &objScale.x, 0.5f);
	ImGui::InputFloat("scaleY", &objScale.y, 0.5f);
	ImGui::InputFloat("ScaleZ", &objScale.z, 0.5f);
	ImGui::End();

	// ImGui�œ��͂��ꂽ�l���I�u�W�F�N�g�ɓK�p
	// �ꏊ��Object�������Ă���\���̂�Center�ɓK�p������
	// <-���ꂼ��̃I�u�W�F�N�g��Update��Center��transform.positinon�ɓK�p�����Ă邩��
	getObj->pObj.center = objPos;	// �ꏊ

	// ��]�p�x�����W�A������x���ɕϊ�
	VECTOR3 Rot = getObj->Rotation();
	//objRot = objRot / 180.0f * XM_PI;
	getObj->SetRotation(objRot / 180.0f * XM_PI);	// ��]
	getObj->SetScale(objScale);	// �X�P�[��

}

void StageEdit::SetObj(Object3D* ob)
{
	// �I������Ă�I�u�W�F�N�g�̕ۑ�
	getObj = ob;

	//gizmoXYZ = new GizmoXYZ();

	// ���ꂼ��̒l��ImGui�p�̕ϐ��ɕۊ�
	objPos = getObj->pObj.center;
	objRot = getObj->Rotation() * 180.0f / XM_PI;
	objScale = getObj->Scale();
	
	nState = sHas;
}

void StageEdit::DupeObj(Object3D* ob)
{
	if (ob->pObj.name == "Box")
	{
		getObj = new Box();
	}
	else if (ob->pObj.name == "MoveBax")
	{
		getObj = new MoveBox();
	}
	else if (ob->pObj.name == "Ball")
	{
		getObj = new Ball();
	}
	else if (ob->pObj.name == "Player")
	{
		getObj = new Player(1);
	}
	getObj->SetPosition(ob->Position());
	getObj->SetRotation(ob->Rotation());
	getObj->SetScale(ob->Scale());
}

void StageEdit::Save(int n)
{
	char name[64];
	sprintf_s<64>(name, "data/Stage%02d.csv", n);
	// �t�@�C�����J��
	std::ofstream ofs(name); // �����Ƀt�@�C����
	// �f�[�^������
	// �Z�[�u���邽�߂ɃI�u�W�F�N�g�T��
	std::list<Object3D*> objs = ObjectManager::FindGameObjects<Object3D>();
	for (Object3D* ob : objs)
	{
		// Box
		if (ob->pObj.name == "Box")
		{
			ofs << "1" << "," << "BOX";
			
		}
		// MoveBox
		else if (ob->pObj.name == "MoveBox")
		{

		}
		// Player
		else if (ob->pObj.name == "Player")
		{

		}
		// ���s
		ofs << std::endl;
	}
	// �t�@�C�������
	ofs.close();
}

void StageEdit::Load(int n)
{
	char name[64];
	sprintf_s<64>(name, "data/Stage%02d.csv", n);

	CsvReader* csv = new CsvReader(name);
	
	delete csv;
}

// �}�E�X�J�[�\���̃��[���h���W�̎擾
void StageEdit::GetWorldPos()
{
	// �}�E�X���W�擾
	mousePos = GameDevice()->m_pDI->GetMousePos();
	
	// �ߎ��_(0)�Ɖ����_(1)
	nearWorldPos = XMVector3Unproject(VECTOR3(mousePos.x, mousePos.y, 0.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	farWorldPos = XMVector3Unproject(VECTOR3(mousePos.x, mousePos.y, 1.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	
	// �����x�N�g���𐳋K�����Ē��������΂�
	direction = XMVector3Normalize(farWorldPos - nearWorldPos);
	extendedFarWorldPos = nearWorldPos + direction * EXTENDED_DISTANCE;  // EXTENDED_DISTANCE�ŉ��΂�
	return;
}
