// 2024.10.06~ S.Matsunaga	�ҏW��

#include "StageEdit.h"
#include "Box.h"
#include "Ball.h"
#include "MoveBox.h"
#include "Player.h"
#include "Gizmo3D.h"
#include "GizmoXYZ.h"
#include <fstream>

// �}�E�X�̃h���b�O�A���h�h���b�v�ŃX�e�[�W�I�u�W�F�N�g�̔z�u�����z
// Tank�̃��e�B�N���̉�ʍ��W�����[���h���W�ɕϊ�����̂��g�������H
// �I�u�W�F�N�g��z�u�A�폜������csv�̍s���l�߂���
StageEdit::StageEdit()
{
	// ����Gizmo������
	gizmoObj = new Gizmo3D();
	gizmoObj->SetScale(VECTOR3(0.15f, 0.15f, 0.15f));
	gizmoObj->SetRotation(VECTOR3(0, 180.0f / 180 * XM_PI, 0));

	// �eGizmo�ƈړ��A��]�T�C�YGizmo�̏�����
	gizmoC = new GizmoXYZ();
	posGizmoX = new PosGizmoX(gizmoC);
	posGizmoY = new PosGizmoY(gizmoC);
	posGizmoZ = new PosGizmoZ(gizmoC);
	
	rotGizmoX = new RotGizmoX(gizmoC);
	rotGizmoY = new RotGizmoY(gizmoC);
	rotGizmoZ = new RotGizmoZ(gizmoC);

	scaleGizmoX = new ScaleGizmoX(gizmoC);
	scaleGizmoY = new ScaleGizmoY(gizmoC);
	scaleGizmoZ = new ScaleGizmoZ(gizmoC);

	nState = sNone;
	gState = sNoneGizmo;

	e = 0.8;
	f = 0.98;
	mass = 1;
}

StageEdit::~StageEdit()
{
}

void StageEdit::Update()
{
	// �e�s��ƃJ�[�\���̃��[���h���W�擾
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
	// Player�͂Q�܂�
	if(pNum <= 1)
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
	//CSprite* spr = new CSprite;
	//spr->DrawLine3D(nearWorldPos, farWorldPos, RGB(255, 0, 0), 1);
}
#endif

void StageEdit::NoneUpdate()
{
	if (GameDevice()->m_pDI->CheckMouse(KD_TRG, 0))
	{
		list<Object3D*> objs = ObjectManager::FindGameObjects<Object3D>();
		for (Object3D* obj : objs)
		{
			VECTOR3 hit;
			if (obj->HitLineToMesh(nearWorldPos, farWorldPos, &hit))
			{
				SelectObj(obj);
				return;	// �ȉ��R�[�h�ȗ�
			}
		}
	}
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_1))
	{
		SelectObj(new Box());
	}
	else if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_2))
	{
		SelectObj(new Player(false));
	}

	// Stage�ǂݏ����p
	ImGui::SetNextWindowPos(ImVec2(170, 30));
	ImGui::SetNextWindowSize(ImVec2(200, 130));
	ImGui::Begin("MENU");
	ImGui::InputInt("Stage", &stageNum);
	if (ImGui::Button("SAVE")) {
		Save(stageNum);
	}
	if (ImGui::Button("LOAD")) {
		Load(stageNum);
	}
	ImGui::End();

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
		// ��ɕ\������Gizmo�����Փ˔�����Ƃ�
		list<GizmoXYZ*> gizmos = ObjectManager::FindGameObjectsVisible<GizmoXYZ>();
		for (GizmoXYZ* gizmo : gizmos)
		{
			// �T�����ꂽ�ŏ��̃I�u�W�F�N�g��
			bool firstFlag = true;
			// naerWorldPos���瓖�������ꏊ�܂ł̋���
			float distance = 0.0f;
			// ���������I�u�W�F�N�g�̂Ȃ��ł̍ŒZ����
			float minDistance = 0.0f;
			VECTOR3 hit;
			if (gizmo->HitLineToMesh(nearWorldPos, farWorldPos, &hit))
			{
				// ���������ꏊ�ւ̋��������߂Ĉ�ԋ߂��I�u�W�F�N�g���i�[����
				distance = (hit - nearWorldPos).Length();
				float exDistance = (nearWorldPos - getObj->Position()).Length();
				extendedNearWorldPos = nearWorldPos + direction * exDistance;  // exDistance�ŉ��΂�
				prevMousePos = extendedNearWorldPos;
				// �Ⴄ�I�u�W�F�N�g���N���b�N���ꂽ��I���I�u�W�F�N�g�ύX
				if (firstFlag)
				{
					minDistance = distance;
					firstFlag = false;
					//float exDistance = (nearWorldPos - getObj->Position()).Length();
					//extendedNearWorldPos = nearWorldPos + direction * exDistance;  // exDistance�ŉ��΂�
					//prevMousePos = extendedNearWorldPos;
					getGizmo = gizmo;
				}
				else
				{
					if (minDistance > distance)
					{
						minDistance = distance;
						//float exDistance = (nearWorldPos - getObj->Position()).Length();
						//extendedNearWorldPos = nearWorldPos + direction * exDistance;  // exDistance�ŉ��΂�
						//prevMousePos = extendedNearWorldPos;
						getGizmo = gizmo;
					}
				}					
				isHit = true;
			}
		}
		// Gizmo�ɓ������ĂȂ���ΏՓ˔�����Ƃ�
		if(!isHit)
		{
			// ��ԋ߂��I�u�W�F�N�g�̊i�[
			Object3D* temp = nullptr;
			// �T�����ꂽ�ŏ��̃I�u�W�F�N�g��
			bool firstFlag = true;
			// naerWorldPos���瓖�������ꏊ�܂ł̋���
			float distance = 0.0f;
			// ���������I�u�W�F�N�g�̂Ȃ��ł̍ŒZ����
			float minDistance = 0.0f;
			// Gizmo�ȊO�̃I�u�W�F�N�g�𒲂ׂ�
			list<Object3D*> objs = ObjectManager::FindGameObjectsWithOutTag<Object3D>("Gizmo");
			for (Object3D* obj : objs)
			{
				VECTOR3 hit;
				// �J�[�\���̃��[���h���W�̋ߎ��_���牓���_�܂ł�Ray���΂�
				if (obj->HitLineToMesh(nearWorldPos, farWorldPos, &hit))
				{
					// ���������ꏊ�ւ̋��������߂Ĉ�ԋ߂��I�u�W�F�N�g���i�[����
					distance = (hit - nearWorldPos).Length();
					// �Ⴄ�I�u�W�F�N�g���N���b�N���ꂽ��I���I�u�W�F�N�g�ύX
					if (firstFlag)
					{
						temp = obj;
						minDistance = distance;
						firstFlag = false;
					}
					// ���ڈȍ~�������߂����
					else if(minDistance > distance)
					{
						if(obj != temp)
						{
							temp = obj;
							minDistance = distance;
						}
					}
				}
			}
			if (temp != nullptr)
			{
				SelectObj(temp);
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
	if (getObj->editObj.name != "MoveBox")
	{
		ImGui::SetNextWindowSize(ImVec2(290, 310));
	}
	else
	{
		ImGui::SetNextWindowSize(ImVec2(290, 400));
	}
	string name = "OBJINFO:" + getObj->editObj.name;
	//sprintf_s<64>(name, "data/Stage%02d.csv", n);
	ImGui::Begin(name.c_str());
	// �����W���E���C�E����
	ImGui::SliderFloat("e", &e, 0.0f, 1.0f, "%.1f");
	ImGui::SliderFloat("f", &f, 0.0f, 1.0f, "%.2f");
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
	// MoveBox�̏ꍇ�ړ����ƈړ���
	if (getObj->editObj.name == "MoveBox")
	{
		ImGui::InputFloat("MoveSpeed", &moveSpeed, 0.5f);
		ImGui::InputFloat("MovementX", &moveVolumu.x, 0.5f);
		ImGui::InputFloat("MovementY", &moveVolumu.y, 0.5f);
		ImGui::InputFloat("MovementZ", &moveVolumu.z, 0.5f);
	}
	ImGui::End();

	// ImGui�œ��͂��ꂽ�l���I�u�W�F�N�g�ɓK�p
	// �ꏊ��Object�������Ă���\���̂�Center�ɓK�p������
	// <-���ꂼ��̃I�u�W�F�N�g��Update��Center��transform.positinon�ɓK�p�����Ă邩��
	getObj->pObj.center = objPos;	// �ꏊ
	// ��]�p�x�����W�A������x���ɕϊ�
	VECTOR3 Rot = getObj->Rotation();
	getObj->SetRotation(objRot / 180.0f * XM_PI);	// ��]
	// �X�P�[���̍ŏ��l��0.1
	if(objScale.x <= 0.1f)
	{
		objScale.x = 0.1f;
	}
	if (objScale.y <= 0.1f)
	{
		objScale.y = 0.1f;
	}
	if (objScale.z <= 0.1f)
	{
		objScale.z = 0.1f;
	}
	getObj->SetScale(objScale);	// �X�P�[��

	if (getObj->editObj.name == "MoveBox")
	{
		// ������MoveBox�ňړ����x�A�ړ��ʂ�n��
	}

	// ����ύX
	if (!pDI->CheckMouse(KD_DAT, 1))
	{
		if (pDI->CheckKey(KD_TRG, DIK_W))
		{
			gState = sPosGizmo;
			SetGizmo(gState);
		}
		else if (pDI->CheckKey(KD_TRG, DIK_E))
		{	
			gState = sScaleGizmo;
			SetGizmo(gState);
		}
		else if (pDI->CheckKey(KD_TRG, DIK_R))
		{	
			gState = sRotGizmo;
			SetGizmo(gState);
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
// Gizmo�̃T�C�Y�����������ă��b�V���R���C�_�[�����܂�����Ă��Ȃ��H
// 
// Position�ړ�Gizmo�\����
void StageEdit::PosGizmoUpdate()
{
	if (getGizmo != nullptr)
	{
		// �I������Ă���Gizmo�̂ݕ\��
		if (getGizmo != posGizmoX)
		{
			ObjectManager::SetVisible(posGizmoX,false);
		}
		if (getGizmo != posGizmoY)
		{
			ObjectManager::SetVisible(posGizmoY, false);
		}
		if (getGizmo != posGizmoZ)
		{
			ObjectManager::SetVisible(posGizmoZ, false);
		}
		// �I�u�W�F�N�g�̈ʒu�܂ŐL�΂�
		float exDistance = (nearWorldPos - getObj->Position()).Length();	// �I�u�W�F�N�g�܂ł̋���
		extendedNearWorldPos = nearWorldPos + direction * exDistance;  // exDistance�ŉ��΂�
		// ������Gizmo��G���ăI�u�W�F�N�g�𓮂���
		VECTOR3 diff = extendedNearWorldPos - prevMousePos;
		if (diff.Length() != 0)
		{
			if (pDI->CheckMouse(KD_DAT, 0))
			{
				if (getGizmo->editObj.name == "posGizmoX")
				{
					objPos.x +=  diff.x;
					getObj->pObj.center.x += diff.x;
				}
				else if (getGizmo->editObj.name == "posGizmoY")
				{
					objPos.y += diff.y;
					getObj->pObj.center.y += diff.y;
				}
				else if (getGizmo->editObj.name == "posGizmoZ")
				{
					objPos.z += diff.z;
					getObj->pObj.center.z += diff.z;
				}
			}
		}
		prevMousePos = extendedNearWorldPos;
	}
	// getObj�̃X�N���[�����W
	VECTOR3 objScreenPos = XMVector3Project(getObj->Position(), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	// getObj�̃X�N���[�����W�����[���h���W�ɕϊ�
	VECTOR3 gizmoWorldPos = XMVector3Unproject(VECTOR3(objScreenPos.x, objScreenPos.y, 0.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	gizmoC->SetPosition(gizmoWorldPos);
	if (pDI->CheckMouse(KD_UTRG, 0))
	{
		getGizmo = nullptr;
		ObjectManager::SetVisible(posGizmoX, true);
		ObjectManager::SetVisible(posGizmoY, true);
		ObjectManager::SetVisible(posGizmoZ, true);
	}
}

void StageEdit::RotGizmoUpdate()
{
	// ������Gizmo��G���ăI�u�W�F�N�g�𓮂���
	if (getGizmo != nullptr)
	{
		// �I������Ă���Gizmo�̂ݕ\��
		if (getGizmo != rotGizmoX)
		{
			ObjectManager::SetVisible(rotGizmoX, false);
		}
		if (getGizmo != rotGizmoY)
		{
			ObjectManager::SetVisible(rotGizmoY, false);
		}
		if (getGizmo != rotGizmoZ)
		{
			ObjectManager::SetVisible(rotGizmoZ, false);
		}
		// �I�u�W�F�N�g�̈ʒu�܂ŐL�΂�
		float exDistance = (nearWorldPos - getObj->Position()).Length();
		VECTOR3 nearWorldPosEx = nearWorldPos + direction * exDistance;  // exDistance�ŉ��΂�
		// ������Gizmo��G���ăI�u�W�F�N�g�𓮂���
		VECTOR3 diff = nearWorldPosEx - prevMousePos;
		if (diff.Length() != 0)
		{
			if (pDI->CheckMouse(KD_DAT, 0))
			{
				if (getGizmo->editObj.name == "rotGizmoX")
				{
					//objRot.x = (objRot.x / 180.0f * XM_PI) + (diff.x * 10 / 180.0f * XM_PI);
					//getObj->SetRotation(VECTOR3((objRot.x / 180.0f * XM_PI) + (diff.x * 10 / 180.0f * XM_PI), getObj->Rotation().y, getObj->Rotation().z));
				}
				else if (getGizmo->editObj.name == "rotGizmoY")
				{
					objRot.y = (objRot.y / 180.0f * XM_PI) + (diff.y * 10 / 180.0f * XM_PI);
					getObj->SetRotation(VECTOR3(objRot.x, (objRot.y / 180.0f * XM_PI) + (diff.y * 10 / 180.0f * XM_PI), objRot.z));
				}
				else if (getGizmo->editObj.name == "rotGizmoZ")
				{
					//objRot.z = diff.z / 180.0f * XM_PI;
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
	if (pDI->CheckMouse(KD_UTRG, 0))
	{
		getGizmo = nullptr;
		ObjectManager::SetVisible(rotGizmoX, true);
		ObjectManager::SetVisible(rotGizmoY, true);
		ObjectManager::SetVisible(rotGizmoZ, true);
	}
}

void StageEdit::ScaleGizmoUpdate()
{
	// ������Gizmo��G���ăI�u�W�F�N�g�𓮂���
	if (getGizmo != nullptr)
	{
		// �I������Ă���Gizmo�̂ݕ\��
		if (getGizmo != scaleGizmoX)
		{
			ObjectManager::SetVisible(scaleGizmoX, false);
		}
		if (getGizmo != scaleGizmoY)
		{
			ObjectManager::SetVisible(scaleGizmoY, false);
		}
		if (getGizmo != scaleGizmoZ)
		{
			ObjectManager::SetVisible(scaleGizmoZ, false);
		}
		// �I�u�W�F�N�g�̈ʒu�܂ŐL�΂�
		float exDistance = (nearWorldPos - getObj->Position()).Length();
		VECTOR3 nearWorldPosEx = nearWorldPos + direction * exDistance;  // exDistance�ŉ��΂�
		// ������Gizmo��G���ăI�u�W�F�N�g�𓮂���
		VECTOR3 diff = nearWorldPosEx - prevMousePos;
		if (diff.Length() != 0)
		{
			if (pDI->CheckMouse(KD_DAT, 0))
			{
				if (getGizmo->editObj.name == "scaleGizmoX")
				{
					objScale.x += diff.x * 2;
				}
				else if (getGizmo->editObj.name == "scaleGizmoY")
				{
					objScale.y += diff.y * 2;
				}
				else if (getGizmo->editObj.name == "scaleGizmoZ")
				{
					objScale.z += diff.z * 2;
				}
				else if (getGizmo->editObj.name == "gizmoCenter")
				{
					objScale += diff * 2;
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
	if (pDI->CheckMouse(KD_UTRG, 0))
	{
		getGizmo = nullptr;
		ObjectManager::SetVisible(scaleGizmoX, true);
		ObjectManager::SetVisible(scaleGizmoY, true);
		ObjectManager::SetVisible(scaleGizmoZ, true);
	}
}

void StageEdit::SetGizmo(int gState)
{
	switch (gState)
	{
	case sPosGizmo:
		ObjectManager::SetVisible(gizmoC, true);
		ObjectManager::SetVisible(posGizmoX, true);
		ObjectManager::SetVisible(posGizmoY, true);
		ObjectManager::SetVisible(posGizmoZ, true);
		ObjectManager::SetVisible(rotGizmoC, false);
		ObjectManager::SetVisible(rotGizmoX, false);
		ObjectManager::SetVisible(rotGizmoY, false);
		ObjectManager::SetVisible(rotGizmoZ, false);
		ObjectManager::SetVisible(scaleGizmoX, false);
		ObjectManager::SetVisible(scaleGizmoY, false);
		ObjectManager::SetVisible(scaleGizmoZ, false);
		break;
	case sRotGizmo:
		ObjectManager::SetVisible(gizmoC, true);
		ObjectManager::SetVisible(posGizmoX, false);
		ObjectManager::SetVisible(posGizmoY, false);
		ObjectManager::SetVisible(posGizmoZ, false);
		ObjectManager::SetVisible(rotGizmoC, true);
		ObjectManager::SetVisible(rotGizmoX, true);
		ObjectManager::SetVisible(rotGizmoY, true);
		ObjectManager::SetVisible(rotGizmoZ, true);
		ObjectManager::SetVisible(scaleGizmoX, false);
		ObjectManager::SetVisible(scaleGizmoY, false);
		ObjectManager::SetVisible(scaleGizmoZ, false);
		break;
	case sScaleGizmo:
		ObjectManager::SetVisible(gizmoC, true);
		ObjectManager::SetVisible(posGizmoX, false);
		ObjectManager::SetVisible(posGizmoY, false);
		ObjectManager::SetVisible(posGizmoZ, false);
		ObjectManager::SetVisible(rotGizmoC, false);
		ObjectManager::SetVisible(rotGizmoX, false);
		ObjectManager::SetVisible(rotGizmoY, false);
		ObjectManager::SetVisible(rotGizmoZ, false);
		ObjectManager::SetVisible(scaleGizmoX, true);
		ObjectManager::SetVisible(scaleGizmoY, true);
		ObjectManager::SetVisible(scaleGizmoZ, true);
		break;
	default:
		ObjectManager::SetVisible(gizmoC, false);
		ObjectManager::SetVisible(posGizmoX, false);
		ObjectManager::SetVisible(posGizmoY, false);
		ObjectManager::SetVisible(posGizmoZ, false);
		ObjectManager::SetVisible(rotGizmoC, false);
		ObjectManager::SetVisible(rotGizmoX, false);
		ObjectManager::SetVisible(rotGizmoY, false);
		ObjectManager::SetVisible(rotGizmoZ, false);
		ObjectManager::SetVisible(scaleGizmoX, false);
		ObjectManager::SetVisible(scaleGizmoY, false);
		ObjectManager::SetVisible(scaleGizmoZ, false);
		break;
	}
}

void StageEdit::SelectObj(Object3D* ob)
{
	// �I������Ă�I�u�W�F�N�g�̕ۑ�
	if (getObj != ob)
	{
		if (getObj != nullptr)DeselectObj();
		getObj = ob;
	}
	// �I������Ă�I�u�W�F�N�g��Gizmo�\��
	// ���߂�Gizmo���o��ꍇposGizmo���o��
	if (gState == sNoneGizmo)
	{
		gState = sPosGizmo;
	}
	// �X�e�[�^�X�ɂ���ĕ\������Gizmo��ς���
	SetGizmo(gState);

	// ���ꂼ��̒l��ImGui�p�̕ϐ��ɕۊ�
	objPos = getObj->pObj.center;
	objRot = getObj->Rotation() * 180.0f / XM_PI;
	objScale = getObj->Scale();
	
	getObj->editObj.isSelect = true;
	nState = sHas;
}

void StageEdit::DeselectObj()
{
	getObj->editObj.isSelect = false;
	getObj = nullptr;
	nState = sNone;
	SetGizmo(sNoneGizmo);
}

void StageEdit::DupeObj(Object3D* ob)
{
	if (ob->editObj.name == "Box")
	{
		SelectObj(new Box());
	}
	else if (ob->editObj.name == "MoveBax")
	{
		SelectObj(new MoveBox());
	}
	else if (ob->editObj.name == "Ball")
	{
		SelectObj(new Ball(false));
	}
	else if (ob->editObj.name == "Player")
	{
		if (pNum <= 1)
		{
			SelectObj(new Player(pNum, false));
			pNum++;
		}
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
	ofstream ofs(name); // �����Ƀt�@�C����
	// �f�[�^������
	// �Z�[�u���邽�߂ɃI�u�W�F�N�g�T��
	list<Object3D*> objs = ObjectManager::FindGameObjects<Object3D>();
	for (Object3D* ob : objs)
	{
		// Box
		if (ob->editObj.name == "Box")
		{
			ofs << "1" << "," << "BOX" << ",";
			ofs << ob->Position().x << "," << ob->Position().y << "," << ob->Position().z << ",";
			ofs << ob->Scale().x << "," << ob->Scale().y << "," << ob->Scale().z << ",";
			ofs << ob->Rotation().x * 180.0f / XM_PI << "," << ob->Rotation().y * 180.0f / XM_PI << "," << ob->Rotation().z * 180.0f / XM_PI << ",";
			ofs << ob->pObj.e << "," << ob->pObj.f;		
		}
		// MoveBox
		else if (ob->editObj.name == "MoveBox")
		{
			ofs << "1" << "," << "MBox" << ",";
			ofs << ob->Position().x << "," << ob->Position().y << "," << ob->Position().z << ",";
			ofs << ob->Scale().x << "," << ob->Scale().y << "," << ob->Scale().z << ",";
		}
		// Player
		else if (ob->editObj.name == "Player")
		{
			ofs << "1" << "," << "PLAYER" << ",";
			ofs << ob->Position().x << "," << ob->Position().y << "," << ob->Position().z << ",";
			ofs << ob->pObj.e << "," << ob->pObj.f << "," << ob->pObj.mass << ",";
			ofs << ob->pObj.pNum;
		}
		else if (ob->editObj.name == "Ball")
		{
			ofs << "1" << "," << "BALL" << ",";
			ofs << ob->Position().x << "," << ob->Position().y << "," << ob->Position().z << ",";
			ofs << ob->pObj.e << "," << ob->pObj.f << "," << ob->pObj.mass;
		}
		// ���s
		ofs << endl;
	}
	// �t�@�C�������
	ofs.close();
}

void StageEdit::Load(int n)
{
	list<Object3D*> objs = ObjectManager::FindGameObjects<Object3D>();
	for (Object3D* obj : objs)
	{
		if (obj->editObj.name == "Box" || obj->editObj.name == "MoveBox"|| obj->editObj.name == "Player"|| obj->editObj.name == "Ball")
		{
			obj->DestroyMe();
		}
	}
	char name[64];
	sprintf_s<64>(name, "data/Stage%02d.csv", n);

	CsvReader* csv = new CsvReader(name);

	for (int i = 1; i < csv->GetLines(); i++) { // �P�s���ǂ�
		string str = csv->GetString(i, 0); // �擪�̐��������
		Object3D* obj = nullptr;
		// �擪���u0�v�̏ꍇ�̓X�L�b�v
		if (str == "0")
		{
			continue;
		}
		// �u�P�v�̏ꍇ�����i��
		else if (str == "1")
		{
			str = csv->GetString(i, 1);
			if (str == "PLAYER") {
				float e = csv->GetFloat(i, 5);
				float f = csv->GetFloat(i, 6);
				float mass = csv->GetFloat(i, 7);
				int num = csv->GetFloat(i, 8);
				obj = new Player(num, false);
				obj->pObj.e = e;
				obj->pObj.f = f;
				obj->pObj.mass = mass;
			}
			else if (str == "BOX") {
				VECTOR3 size = VECTOR3(csv->GetFloat(i, 5), csv->GetFloat(i, 6), csv->GetFloat(i, 7));
				VECTOR3 rot = VECTOR3(csv->GetFloat(i, 8), csv->GetFloat(i, 9), csv->GetFloat(i, 10));
				float e = csv->GetFloat(i, 11);
				float f = csv->GetFloat(i, 12);
				obj = new Box(size, rot);	// �����̂̃T�C�Y�Ɖ�]�ʂ�n��
				obj->pObj.e = e;
				obj->pObj.f = f;
			}
			else if (str == "MBox") {
				VECTOR3 size = VECTOR3(csv->GetFloat(i, 5), csv->GetFloat(i, 6), csv->GetFloat(i, 7));
				VECTOR3 rot = VECTOR3(csv->GetFloat(i, 8), csv->GetFloat(i, 9), csv->GetFloat(i, 10));
				VECTOR3 move = VECTOR3(csv->GetFloat(i, 11), csv->GetFloat(i, 12), csv->GetFloat(i, 13));
				VECTOR3 moveSpeed = VECTOR3(csv->GetFloat(i, 14), csv->GetFloat(i, 15), csv->GetFloat(i, 16));
				float e = csv->GetFloat(i, 17);
				float f = csv->GetFloat(i, 18);
				obj = new MoveBox(size, rot, move, moveSpeed);	// �����̂̃T�C�Y�Ɖ�]�ʁA�ړ��ʂ�n��
				obj->pObj.e = e;
				obj->pObj.f = f;
			}
			else if (str == "BALL") {
				float e = csv->GetFloat(i, 5);
				float f = csv->GetFloat(i, 6);
				float mass = csv->GetFloat(i, 7);
				obj = new Ball(false);
				obj->pObj.e = e;
				obj->pObj.f = f;
				obj->pObj.mass = mass;
			}
			else {
				assert(false);
			}
			float x = csv->GetFloat(i, 2);
			float y = csv->GetFloat(i, 3);
			float z = csv->GetFloat(i, 4);
			obj->SetPosition(x, y, z);
		}
	}
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
	return;
}
