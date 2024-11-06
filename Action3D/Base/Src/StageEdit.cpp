#include "StageEdit.h"
#include "Box.h"
#include "Ball.h"
#include "MoveBox.h"
#include "Player.h"
#include "Gizmo3D.h"
#include "GizmoXYZ.h"
#include <fstream>

namespace
{
	static const int EXTENDED_DISTANCE = 50;	// Ray�̒���
	static const int EXTENDED_OBJ_DISTANCE = 20;	// (��)�I�u�W�F�N�g�̈ړ��ꏊ
}

// �}�E�X�̃h���b�O�A���h�h���b�v�ŃX�e�[�W�I�u�W�F�N�g�̔z�u�����z
// Tank�̃��e�B�N���̉�ʍ��W�����[���h���W�ɕϊ�����̂��g�������H
// �I�u�W�F�N�g��z�u�A�폜������csv�̍s���l�߂���
StageEdit::StageEdit()
{
	// ����Gizmo������
	gizmoObj = new Gizmo3D();
	gizmoObj->SetScale(VECTOR3(0.0015f, 0.0015f, 0.0015f));
	gizmoObj->SetRotation(VECTOR3(0, 180.0f / 180 * XM_PI, 0));

	// �eGizmo�ƈړ��A��]�T�C�YGizmo�̏�����
	gizmoC = new GizmoXYZ();
	posGizmoX = new PosGizmoX(gizmoC);
	posGizmoY = new PosGizmoY(gizmoC);
	posGizmoZ = new PosGizmoZ(gizmoC);

	//rotGizmoX = new RotGizmoX(gizmoC);
	//rotGizmoY = new RotGizmoY(gizmoC);
	//rotGizmoZ = new RotGizmoZ(gizmoC);

	scaleGizmoX = new ScaleGizmoX(gizmoC);
	scaleGizmoY = new ScaleGizmoY(gizmoC);
	scaleGizmoZ = new ScaleGizmoZ(gizmoC);

	nState = sNone;
	gState = sNoneGizmo;
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
	case sNone:
		NoneUpdate();
		break;
	case sHas:
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
		SelectObj(new Box());
	}
	if (ImGui::Button("MoveBox"))
	{
		SelectObj(new MoveBox());
	}
	if (ImGui::Button("Ball"))
	{
		SelectObj(new Ball(false));
	}
	if(pNum == 0)
	{
		if (ImGui::Button("Player"))
		{
			SelectObj(new Player(pNum,false));
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
				SelectObj(ob);
				return;	// �ȉ��R�[�h�ȗ�
			}
		}
	}
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_1))
	{
		SelectObj(new Ball(false));
	}
	else if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_2))
	{
		SelectObj(new Box());
	}
}

void StageEdit::HasUpdate()
{
	// �I������
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_N))
	{
		DeselectObj();
		return;	// �ȉ��R�[�h�ȗ�
	}
	// �I�u�W�F�N�g�폜
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_DELETE))
	{
		getObj->DestroyMe();
		DeselectObj();
		return;
	}
	// �}�E�X���N���b�N
	if (GameDevice()->m_pDI->CheckMouse(KD_TRG, 0))
	{
		bool isHit = false;
		// �I�u�W�F�N�g�T��
		// ���Gizmo�����Փ˔�����Ƃ�
		std::list<Object3D*> gizmos = ObjectManager::FindGameObjectsWithTag<Object3D>("Gizmo");
		for (Object3D* gizmo : gizmos)
		{
			VECTOR3 hit;
			if (gizmo->HitLineToMesh(nearWorldPos, farWorldPos, &hit))
			{
				getGizmo = gizmo;
				isHit = true;
			}
		}
		// Gizmo�ɓ������ĂȂ���ΏՓ˔�����Ƃ�
		if(!isHit)
		{
			// Gizmo�ȊO�̃I�u�W�F�N�g�𒲂ׂ�
			std::list<Object3D*> objs = ObjectManager::FindGameObjectsWithOutTag<Object3D>("Gizmo");
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
					else
					{
						// �Ⴄ�I�u�W�F�N�g���N���b�N���ꂽ��I���I�u�W�F�N�g�ύX
						SelectObj(ob);
						return;	// �ȉ��R�[�h�ȗ�
					}
				}
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
	getObj->SetRotation(objRot / 180.0f * XM_PI);	// ��]
	getObj->SetScale(objScale);	// �X�P�[��

	// ����ύX
	if (!pDI->CheckMouse(KD_DAT, 1))
	{
		if (pDI->CheckKey(KD_TRG, DIK_W))
		{
			ObjectManager::SetVisible(posGizmoX, true);
			ObjectManager::SetVisible(posGizmoY, true);
			ObjectManager::SetVisible(posGizmoZ, true);
			ObjectManager::SetVisible(rotGizmoX, false);
			ObjectManager::SetVisible(rotGizmoY, false);
			ObjectManager::SetVisible(rotGizmoZ, false);
			ObjectManager::SetVisible(scaleGizmoX, false);
			ObjectManager::SetVisible(scaleGizmoY, false);
			ObjectManager::SetVisible(scaleGizmoZ, false);
			gState = sPosGizmo;
		}
		else if (pDI->CheckKey(KD_TRG, DIK_E))
		{
			ObjectManager::SetVisible(posGizmoX, false);
			ObjectManager::SetVisible(posGizmoY, false);
			ObjectManager::SetVisible(posGizmoZ, false);
			ObjectManager::SetVisible(rotGizmoX, true);
			ObjectManager::SetVisible(rotGizmoY, true);
			ObjectManager::SetVisible(rotGizmoZ, true);
			ObjectManager::SetVisible(scaleGizmoX, false);
			ObjectManager::SetVisible(scaleGizmoY, false);
			ObjectManager::SetVisible(scaleGizmoZ, false);
			gState = sScaleGizmo;
		}
		else if (pDI->CheckKey(KD_TRG, DIK_R))
		{
			ObjectManager::SetVisible(posGizmoX, false);
			ObjectManager::SetVisible(posGizmoY, false);
			ObjectManager::SetVisible(posGizmoZ, false);
			ObjectManager::SetVisible(rotGizmoX, false);
			ObjectManager::SetVisible(rotGizmoY, false);
			ObjectManager::SetVisible(rotGizmoZ, false);
			ObjectManager::SetVisible(scaleGizmoX, true);
			ObjectManager::SetVisible(scaleGizmoY, true);
			ObjectManager::SetVisible(scaleGizmoZ, true);
			gState = sRotGizmo;
		}
	}

	// �M�Y���̕\����ԂŃX�e�[�^�X����
	switch (gState)
	{
	case sPosGizmo:
		PosGizmoUpdate();
		break;
	case sRotGizmo:
		RotGizmoUpdate();
		break;
	case sScaleGizmo:
		ScaleGizmoUpdate();
		break;
	default:
		break;
	}
}
//
// Gizmo���N���b�N�o���Ȃ���蔭����
// �����͂����炭Gizmo�̈ʒu��Ray�̔��ˈʒu������nearWorldPos�ɂȂ��Ă��邩��H
// 
// 
// Position�ړ�Gizmo�\����
void StageEdit::PosGizmoUpdate()
{
	if (getGizmo != nullptr)
	{
		// �I�u�W�F�N�g�̈ʒu�܂ŐL�΂�
		float exDistance = (nearWorldPos - getObj->Position()).Length();
		VECTOR3 nearWorldPosEx = nearWorldPos + direction * exDistance;  // exDistance�ŉ��΂�
		// ������Gizmo��G���ăI�u�W�F�N�g�𓮂���
		VECTOR3 diff = nearWorldPosEx - prevMousePos;
		if (diff.Length() != 0)
		{
			if (pDI->CheckMouse(KD_DAT, 0))
			{
				if (getGizmo->pObj.name == "posGizmoX")
				{
					objPos.x = getObj->Position().x + diff.x;
					getObj->pObj.center.x = getObj->Position().x + diff.x;
				}
				else if (getGizmo->pObj.name == "posGizmoY")
				{
					objPos.y = getObj->Position().y + diff.y;
					getObj->pObj.center.y = getObj->Position().y + diff.y;
				}
				else if (getGizmo->pObj.name == "posGizmoZ")
				{
					objPos.z = getObj->Position().z + diff.z;
					getObj->pObj.center.z = getObj->Position().z + diff.z;
				}
			}
		}
		exDistance = (nearWorldPos - getObj->Position()).Length();
		nearWorldPosEx = nearWorldPos + direction * exDistance;  // exDistance�ŉ��΂�
		prevMousePos = nearWorldPosEx;
	}
	// getObj�̃X�N���[�����W
	VECTOR3 objScreenPos = XMVector3Project(getObj->Position(), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	// getObj�̃X�N���[�����W�����[���h���W�ɕϊ�
	VECTOR3 gizmoWorldPos = XMVector3Unproject(VECTOR3(objScreenPos.x, objScreenPos.y, 0.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	gizmoC->SetPosition(gizmoWorldPos);
}

void StageEdit::RotGizmoUpdate()
{
	// ������Gizmo��G���ăI�u�W�F�N�g�𓮂���

	// getObj�̃X�N���[�����W
	VECTOR3 objScreenPos = XMVector3Project(getObj->Position(), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	// getObj�̃X�N���[�����W�����[���h���W�ɕϊ�
	VECTOR3 gizmoWorldPos = XMVector3Unproject(VECTOR3(objScreenPos.x, objScreenPos.y, 0.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	gizmoC->SetPosition(gizmoWorldPos);
}

void StageEdit::ScaleGizmoUpdate()
{
	// ������Gizmo��G���ăI�u�W�F�N�g�𓮂���

	// getObj�̃X�N���[�����W
	VECTOR3 objScreenPos = XMVector3Project(getObj->Position(), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	// getObj�̃X�N���[�����W�����[���h���W�ɕϊ�
	VECTOR3 gizmoWorldPos = XMVector3Unproject(VECTOR3(objScreenPos.x, objScreenPos.y, 0.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	gizmoC->SetPosition(gizmoWorldPos);
}

void StageEdit::SelectObj(Object3D* ob)
{
	// �I������Ă�I�u�W�F�N�g�̕ۑ�
	getObj = ob;

	// �I������Ă�I�u�W�F�N�g��Gizmo�\��
	// ���߂�Gizmo���o��ꍇposGizmo���o��
	if (gState == sNoneGizmo)
	{
		gState = sPosGizmo;
	}
	// �X�e�[�^�X�ɂ���ĕ\������Gizmo��ς���
	if (gState == sPosGizmo)
	{
		ObjectManager::SetVisible(posGizmoX, true);
		ObjectManager::SetVisible(posGizmoY, true);
		ObjectManager::SetVisible(posGizmoZ, true);
	}
	else if (gState == sRotGizmo)
	{
		ObjectManager::SetVisible(rotGizmoX, true);
		ObjectManager::SetVisible(rotGizmoY, true);
		ObjectManager::SetVisible(rotGizmoZ, true);
	}
	else if (gState == sScaleGizmo)
	{
		ObjectManager::SetVisible(scaleGizmoX, true);
		ObjectManager::SetVisible(scaleGizmoY, true);
		ObjectManager::SetVisible(scaleGizmoZ, true);
	}

	// ���ꂼ��̒l��ImGui�p�̕ϐ��ɕۊ�
	objPos = getObj->pObj.center;
	objRot = getObj->Rotation() * 180.0f / XM_PI;
	objScale = getObj->Scale();

	nState = sHas;
}

void StageEdit::DeselectObj()
{
	getObj = nullptr;
	nState = sNone;
	ObjectManager::SetVisible(gizmoC, false);
	ObjectManager::SetVisible(posGizmoX, false);
	ObjectManager::SetVisible(posGizmoY, false);
	ObjectManager::SetVisible(posGizmoZ, false);

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
