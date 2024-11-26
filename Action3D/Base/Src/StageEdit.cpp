// 2024.10.06~ S.Matsunaga	�ҏW��

#include "StageEdit.h"
#include "Box.h"
#include "Ball.h"
#include "MoveBox.h"
#include "Player.h"
#include "Gizmo3D.h"
#include "GizmoXYZ.h"
#include "ScoreArea.h"
#include "FallCheck.h"
#include "Line.h"
#include <fstream>

namespace
{
	// �eImGui�̍��W�ƃT�C�Y�w��
	// ImGui��̃N���b�N�ł̓I�u�W�F�N�g�I�����΂�����
	ImVec2 createObjImPos = ImVec2(30, 30);
	ImVec2 createObjImSize = ImVec2(130, 130);

	ImVec2 stageImPos = ImVec2(170, 30);
	ImVec2 stageImSize = ImVec2(200, 130);
	
	ImVec2 objInfoImPos = ImVec2(30, 170);
	ImVec2 objInfoImSize0 = ImVec2(290, 310);
	ImVec2 objInfoImSize1 = ImVec2(290, 400);

	// ������΂��G���A�w�荶��̍��W�ƉE���̍��W
	VECTOR4 judgeSkipArea0 = VECTOR4(
		createObjImPos.x, createObjImPos.y,
		createObjImSize.x, createObjImSize.y
	);
	VECTOR4 judgeSkipArea1 = VECTOR4(
		stageImPos.x, stageImPos.y,
		stageImSize.x, stageImSize.y
	);
	VECTOR4 judgeSkipArea2 = VECTOR4(
		objInfoImPos.x, objInfoImPos.y,
		objInfoImSize0.x, objInfoImSize0.y
	);
	VECTOR4 judgeSkipArea3 = VECTOR4(
		objInfoImPos.x, objInfoImPos.y,
		objInfoImSize1.x, objInfoImSize1.y
	);

	static const float e = 0.8f;	// �����W��
	static const float f = 0.02f;	// ���C
	static const float mass = 1;	// ����
};

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

	fallCheck = new FallCheck();
	fallCheck->pObj.center = VECTOR3(0, -5, 0);

	line = new Line();

	nState = sNone;
	gState = sNoneGizmo;

	tempE = e;
	tempF = f;
	tempMass = mass;
}

StageEdit::~StageEdit()
{
	// ����[�ł�
	/*SAFE_DELETE(gizmoObj);
	SAFE_DELETE(gizmoC);
	SAFE_DELETE(posGizmoX);
	SAFE_DELETE(posGizmoY);
	SAFE_DELETE(posGizmoZ);
	SAFE_DELETE(rotGizmoX);
	SAFE_DELETE(rotGizmoY);
	SAFE_DELETE(rotGizmoZ);
	SAFE_DELETE(scaleGizmoX);
	SAFE_DELETE(scaleGizmoY);
	SAFE_DELETE(scaleGizmoZ);
	SAFE_DELETE(fallCheck);*/
}

void StageEdit::Update()
{
	// �e�s��ƃJ�[�\���̃��[���h���W�擾
	mView = GameDevice()->m_mView;
	mPrj = GameDevice()->m_mProj;
	identity = XMMatrixIdentity();
	judgeArea = GetWorldPos();

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
	ImGui::SetNextWindowPos(createObjImPos);
	ImGui::SetNextWindowSize(createObjImSize);
	ImGui::Begin("NEWOBJ");
	if (ImGui::Button("Box"))
	{
		isNew = true;
		SelectObj(new Box());
	}
	if (ImGui::Button("MoveBox"))
	{
		isNew = true;
		SelectObj(new MoveBox());
	}
	if (ImGui::Button("Area1"))
	{
		isNew = true;
		SelectObj(new ScoreArea1);
	}
	if (ImGui::Button("Area2"))
	{
		isNew = true;
		SelectObj(new ScoreArea2);
	}
	if (ImGui::Button("Area3"))
	{
		isNew = true;
		SelectObj(new ScoreArea3);
	}
	if (ImGui::Button("Ball"))
	{
		isNew = true;
		SelectObj(new Ball(false));
	}
	// Player�͂Q�܂�
	if(pNum <= 1)
	{
		if (ImGui::Button("Player"))
		{
			isNew = true;
			SelectObj(new Player(pNum,false));
			pNum++;
		}
	}
	ImGui::End();


	// Stage�ǂݏ����p
	ImGui::SetNextWindowPos(stageImPos);
	ImGui::SetNextWindowSize(stageImSize);
	ImGui::Begin("MENU");
	ImGui::InputInt("Stage", &stageNum);
	if (ImGui::Button("SAVE"))
	{
		Save(stageNum);
	}
	if (ImGui::Button("LOAD"))
	{
		Load(stageNum);
	}
	if (ImGui::Button("PLAY"))
	{
		//Save(stageNum);
		SceneManager::ChangeScene("PlayScene",stageNum);
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
	// ImGui�O�Ń}�E�X���N���b�N
	if (judgeArea)
	{
		if (GameDevice()->m_pDI->CheckMouse(KD_TRG, 0))
		{
			Object3D* temp = nullptr;
			// �T�����ꂽ�ŏ��̃I�u�W�F�N�g��
			bool firstFlag = true;
			// naerWorldPos���瓖�������ꏊ�܂ł̋���
			float distance = 0.0f;
			// ���������I�u�W�F�N�g�̂Ȃ��ł̍ŒZ����
			float minDistance = 0.0f;

			// Gizmo�ȊO�̃I�u�W�F�N�g�ƏՓ˔���
			list<Object3D*> objs = FindGameObjectsWithOutTag<Object3D>("Gizmo");
			for (Object3D* obj : objs)
			{
				VECTOR3 hit;
				if (obj->HitLineToMesh(nearWorldPos, farWorldPos, &hit))
				{
					// ���������ꏊ�ւ̋��������߂Ĉ�ԋ߂��I�u�W�F�N�g���i�[����
					distance = (hit - nearWorldPos).Length();
					if (firstFlag)
					{
						minDistance = distance;
						firstFlag = false;
						temp = obj;
					}
					else
					{
						if (minDistance > distance)
						{
							minDistance = distance;
							temp = obj;
						}
					}
				}
			}
			if (temp != nullptr)
			{
				SelectObj(temp);
				return;
			}
		}
	}
	/*if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_1))
	{
		SelectObj(new Box());
	}
	else if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_2))
	{
		SelectObj(new Player(false));
	}*/
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
		// ��������I�u�W�F�N�g�͏����Ȃ�
		if(getObj != fallCheck && getObj != line)
		{
			getObj->DestroyMe();
			DeselectObj();
			return;
		}
	}
	// �}�E�X���N���b�N
	if(judgeArea)
	{
		if (GameDevice()->m_pDI->CheckMouse(KD_TRG, 0))
		{
			bool isHit = false;
			// �T�����ꂽ�ŏ��̃I�u�W�F�N�g��
			bool firstFlag = true;
			// naerWorldPos���瓖�������ꏊ�܂ł̋���
			float distance = 0.0f;
			// ���������I�u�W�F�N�g�̂Ȃ��ł̍ŒZ����
			float minDistance = 0.0f;

			// �I�u�W�F�N�g�T��
			// ��ɕ\������Gizmo�����Փ˔�����Ƃ�
			list<GizmoXYZ*> gizmos = FindGameObjectsVisible<GizmoXYZ>();
			for (GizmoXYZ* gizmo : gizmos)
			{
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
			if (!isHit)
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
				list<Object3D*> objs = FindGameObjectsWithOutTag<Object3D>("Gizmo");
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
						else if (minDistance > distance)
						{
							if (obj != temp)
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
	}

	// Ctrl + D �I�u�W�F�N�g����
	if (pDI->CheckKey(KD_DAT, DIK_LCONTROL) && pDI->CheckKey(KD_TRG, DIK_D))
	{
		DupeObj(getObj);
		return;	// �ȉ��R�[�h�ȗ�
	}

	// ImGui�ŏꏊ�A��]�A�X�P�[����ς���
	ImGui::SetNextWindowPos(objInfoImPos);
	if (getObj->editObj.name != "MoveBox")
	{
		ImGui::SetNextWindowSize(objInfoImSize0);
	}
	else
	{
		ImGui::SetNextWindowSize(objInfoImSize0);
	}
	string name = "OBJINFO:" + getObj->editObj.name;
	if (getObj->editObj.name == "Player")
	{
		name = name + to_string(getObj->pObj.pNum);
	}
	ImGui::Begin(name.c_str());
	// �����W���E���C�E����
	ImGui::SliderFloat("e", &tempE, 0.0f, 1.0f, "%.1f");
	ImGui::SliderFloat("f", &tempF, 0.0f, 0.1f, "%.2f");
	ImGui::InputFloat("mass", &tempMass, 0.5f, 1.0f);
	// �ꏊ
	ImGui::InputFloat("PositionX", &objPos.x, 0.1f, 0.5f, "%.2f");
	ImGui::InputFloat("PositionY", &objPos.y, 0.1f, 0.5f, "%.2f");
	ImGui::InputFloat("PositionZ", &objPos.z, 0.1f, 0.5f, "%.2f");
	// ��]
	ImGui::InputFloat("RotateX", &objRot.x, 0.1f, 0.5f, "%.2f");
	ImGui::InputFloat("RotateY", &objRot.y, 0.1f, 0.5f, "%.2f");
	ImGui::InputFloat("RotateZ", &objRot.z, 0.1f, 0.5f, "%.2f");
	// �X�P�[��
	ImGui::InputFloat("ScaleX", &objScale.x, 0.1f, 0.5f, "%.2f");
	ImGui::InputFloat("scaleY", &objScale.y, 0.1f, 0.5f, "%.2f");
	ImGui::InputFloat("ScaleZ", &objScale.z, 0.1f, 0.5f, "%.2f");
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
	
	getObj->pObj.e = tempE;
	getObj->pObj.f = tempF;
	getObj->pObj.mass = tempMass;
	// �ꏊ��Object�������Ă���\���̂�Center�ɓK�p������
	// ���ꂼ��̃I�u�W�F�N�g��Update��Center��transform.positinon�ɓK�p�����Ă邩��
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
		else if (pDI->CheckKey(KD_TRG, DIK_R))
		{	
			gState = sScaleGizmo;
			SetGizmo(gState);
		}
		else if (pDI->CheckKey(KD_TRG, DIK_E))
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
// Position�ړ�Gizmo�\����
void StageEdit::PosGizmoUpdate()
{
	gizmoC->SetRotation(VECTOR3());
	if (getGizmo != nullptr)
	{
		// �I������Ă���Gizmo�̂ݕ\��
		if (getGizmo != posGizmoX)
		{
			SetVisible(posGizmoX,false);
		}
		if (getGizmo != posGizmoY)
		{
			SetVisible(posGizmoY, false);
		}
		if (getGizmo != posGizmoZ)
		{
			SetVisible(posGizmoZ, false);
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
					objPos.x += diff.x;
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
				else if (getGizmo->editObj.name == "gizmoCenter")
				{
					objPos += diff;
					getObj->pObj.center += diff;
				}
			}
		}
		prevMousePos = extendedNearWorldPos;
	}
	// getObj�̃X�N���[�����W
	VECTOR3 objScreenPos = XMVector3Project(getObj->Position(), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	// getObj�̃X�N���[�����W�����[���h���W�ɕϊ�
	VECTOR3 gizmoWorldPos = XMVector3Unproject(VECTOR3(objScreenPos.x, objScreenPos.y, 0.2f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	gizmoC->SetPosition(gizmoWorldPos);
	if (pDI->CheckMouse(KD_UTRG, 0))
	{
		getGizmo = nullptr;
		SetVisible(posGizmoX, true);
		SetVisible(posGizmoY, true);
		SetVisible(posGizmoZ, true);
	}
}

void StageEdit::RotGizmoUpdate()
{
	gizmoC->SetRotation(getObj->Rotation());
	// ������Gizmo��G���ăI�u�W�F�N�g�𓮂���
	if (getGizmo != nullptr)
	{
		// �I������Ă���Gizmo�̂ݕ\��
		if (getGizmo != rotGizmoX)
		{
			SetVisible(rotGizmoX, false);
		}
		if (getGizmo != rotGizmoY)
		{
			SetVisible(rotGizmoY, false);
		}
		if (getGizmo != rotGizmoZ)
		{
			SetVisible(rotGizmoZ, false);
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
					objRot.x = (objRot.x) + (diff.y * 200 / 180.0f * XM_PI);
				}
				else if (getGizmo->editObj.name == "rotGizmoY")
				{
					objRot.y = (objRot.y) - (diff.x * 100 / 180.0f * XM_PI) + (-diff.z * 100 / 180.0f * XM_PI);
				}
				else if (getGizmo->editObj.name == "rotGizmoZ")
				{
					objRot.z = (objRot.z) + (diff.y * 200 / 180.0f * XM_PI);
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
	VECTOR3 gizmoWorldPos = XMVector3Unproject(VECTOR3(objScreenPos.x, objScreenPos.y, 0.2f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	gizmoC->SetPosition(gizmoWorldPos);
	if (pDI->CheckMouse(KD_UTRG, 0))
	{
		getGizmo = nullptr;
		SetVisible(rotGizmoX, true);
		SetVisible(rotGizmoY, true);
		SetVisible(rotGizmoZ, true);
	}
}

void StageEdit::ScaleGizmoUpdate()
{
	gizmoC->SetRotation(getObj->Rotation());
	// ������Gizmo��G���ăI�u�W�F�N�g�𓮂���
	if (getGizmo != nullptr)
	{
		// �I������Ă���Gizmo�̂ݕ\��
		if (getGizmo != scaleGizmoX)
		{
			SetVisible(scaleGizmoX, false);
		}
		if (getGizmo != scaleGizmoY)
		{
			SetVisible(scaleGizmoY, false);
		}
		if (getGizmo != scaleGizmoZ)
		{
			SetVisible(scaleGizmoZ, false);
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
	VECTOR3 gizmoWorldPos = XMVector3Unproject(VECTOR3(objScreenPos.x, objScreenPos.y, 0.2f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	gizmoC->SetPosition(gizmoWorldPos);
	if (pDI->CheckMouse(KD_UTRG, 0))
	{
		getGizmo = nullptr;
		SetVisible(scaleGizmoX, true);
		SetVisible(scaleGizmoY, true);
		SetVisible(scaleGizmoZ, true);
	}
}

void StageEdit::SetGizmo(int gState)
{
	switch (gState)
	{
	case sPosGizmo:
		SetVisible(gizmoC, true);
		SetVisible(posGizmoX, true);
		SetVisible(posGizmoY, true);
		SetVisible(posGizmoZ, true);
		SetVisible(rotGizmoC, false);
		SetVisible(rotGizmoX, false);
		SetVisible(rotGizmoY, false);
		SetVisible(rotGizmoZ, false);
		SetVisible(scaleGizmoX, false);
		SetVisible(scaleGizmoY, false);
		SetVisible(scaleGizmoZ, false);
		break;
	case sRotGizmo:
		SetVisible(gizmoC, true);
		SetVisible(posGizmoX, false);
		SetVisible(posGizmoY, false);
		SetVisible(posGizmoZ, false);
		SetVisible(rotGizmoC, true);
		SetVisible(rotGizmoX, true);
		SetVisible(rotGizmoY, true);
		SetVisible(rotGizmoZ, true);
		SetVisible(scaleGizmoX, false);
		SetVisible(scaleGizmoY, false);
		SetVisible(scaleGizmoZ, false);
		break;
	case sScaleGizmo:
		SetVisible(gizmoC, true);
		SetVisible(posGizmoX, false);
		SetVisible(posGizmoY, false);
		SetVisible(posGizmoZ, false);
		SetVisible(rotGizmoC, false);
		SetVisible(rotGizmoX, false);
		SetVisible(rotGizmoY, false);
		SetVisible(rotGizmoZ, false);
		SetVisible(scaleGizmoX, true);
		SetVisible(scaleGizmoY, true);
		SetVisible(scaleGizmoZ, true);
		break;
	default:
		SetVisible(gizmoC, false);
		SetVisible(posGizmoX, false);
		SetVisible(posGizmoY, false);
		SetVisible(posGizmoZ, false);
		SetVisible(rotGizmoC, false);
		SetVisible(rotGizmoX, false);
		SetVisible(rotGizmoY, false);
		SetVisible(rotGizmoZ, false);
		SetVisible(scaleGizmoX, false);
		SetVisible(scaleGizmoY, false);
		SetVisible(scaleGizmoZ, false);
		break;
	}
}

void StageEdit::SelectObj(Object3D* ob)
{
	if (isNew)
	{
		if (getObj != nullptr)DeselectObj();
		getObj = ob;
		getObj->pObj.e = e;
		getObj->pObj.f = f;
		getObj->pObj.mass = mass;
		isNew = false;
	}
	else if (getObj == ob)
	{
		return;
	}
	else
	{
		if (getObj != nullptr)DeselectObj();
	}
	getObj = ob;

	// �I������Ă�I�u�W�F�N�g��Gizmo�\��
	// ���߂�Gizmo���o��ꍇposGizmo���o��
	if (gState == sNoneGizmo)
	{
		gState = sPosGizmo;
	}

	// ���ꂼ��̒l��ImGui�p�̕ϐ��ɕۊ�
	objPos = getObj->pObj.center;
	objRot = getObj->Rotation() * 180.0f / XM_PI;
	objScale = getObj->Scale();

	tempE = getObj->pObj.e;
	tempF = getObj->pObj.f;
	tempMass = getObj->pObj.mass;

	// �X�e�[�^�X�ɂ���ĕ\������Gizmo��ς���
	SetGizmo(gState);

	getObj->editObj.isSelect = true;
	nState = sHas;
}

void StageEdit::DeselectObj()
{
	if(getObj != nullptr)
	{
		getObj->editObj.isSelect = false;
		getObj = nullptr;
		nState = sNone;
		SetGizmo(sNoneGizmo);
	}
}

void StageEdit::DupeObj(Object3D* ob)
{
	// �R�s�[���ꂽ�I�u�W�F�N�g��temp�ɕۑ�����
	Object3D* temp = nullptr;
	if (ob->editObj.name == "Box")
	{
		temp = new Box();
	}
	else if (ob->editObj.name == "MoveBax")
	{
		temp = new MoveBox();
	}
	else if (ob->editObj.name == "Ball")
	{
		temp = new Ball(false);
	}
	else if (ob->editObj.name == "Player")
	{
		if (pNum <= 1)
		{
			temp = new Player(pNum, false);
			pNum++;
		}
	}
	else if (ob->editObj.name == "scoreArea1")
	{
		temp = new ScoreArea1();
	}
	else if (ob->editObj.name == "scoreArea2")
	{
		temp = new ScoreArea2();
	}
	else if (ob->editObj.name == "scoreArea3")
	{
		temp = new ScoreArea3();
	}
	if(temp != nullptr)
	{
		temp->pObj.center = ob->pObj.center;
		temp->SetRotation(ob->Rotation());
		temp->SetScale(ob->Scale());

		temp->pObj.e = ob->pObj.e;
		temp->pObj.f = ob->pObj.f;
		temp->pObj.mass = ob->pObj.mass;

		SelectObj(temp);
	}
}

void StageEdit::Save(int n)
{
	char name[64];
	if (!isTestMap)
	{
		sprintf_s<64>(name, "data/StageCsv/Stage%02d.csv", n);
	}
	else
	{
		sprintf_s<64>(name, "data/TestStageCsv/testStage%02d.csv", n);
	}
	// �t�@�C�����J��
	ofstream ofs(name); // �����Ƀt�@�C����
	// �f�[�^������
	// �Z�[�u���邽�߂ɃI�u�W�F�N�g�T��

	list<Object3D*> objs = FindGameObjects<Object3D>();
	for (Object3D* obj : objs)
	{
		if (obj->IsTag("PLAYER"))
		{
			// Player
			if (obj->editObj.name == "Player")
			{
				ofs << "1" << "," << "PLAYER" << ",";
				ofs << obj->Position().x << "," << obj->Position().y << "," << obj->Position().z << ",";
				ofs << obj->Rotation().y << ",";
				ofs << obj->pObj.e << "," << obj->pObj.f << "," << obj->pObj.mass << ",";
				ofs << obj->pObj.pNum;
			}
			else if (obj->editObj.name == "Ball")
			{
				ofs << "1" << "," << "BALL" << ",";
				ofs << obj->Position().x << "," << obj->Position().y << "," << obj->Position().z << ",";
				ofs << obj->pObj.e << "," << obj->pObj.f << "," << obj->pObj.mass;
			}
			// �Ή�������̂��Ȃ���Ζ���
			else
			{
				continue;
			}
			// ���s
			ofs << endl;
		}
		else if (obj->IsTag("STAGEOBJ"))
		{
			// Box
			if (obj->editObj.name == "Box")
			{
				ofs << "1" << "," << "BOX" << ",";
				ofs << obj->Position().x << "," << obj->Position().y << "," << obj->Position().z << ",";
				ofs << obj->Scale().x << "," << obj->Scale().y << "," << obj->Scale().z << ",";
				ofs << obj->Rotation().x * 180.0f / XM_PI << "," << obj->Rotation().y * 180.0f / XM_PI << "," << obj->Rotation().z * 180.0f / XM_PI << ",";
				ofs << obj->pObj.e << "," << obj->pObj.f;
			}
			// MoveBox
			else if (obj->editObj.name == "MoveBox")
			{
				ofs << "1" << "," << "MBox" << ",";
				ofs << obj->Position().x << "," << obj->Position().y << "," << obj->Position().z << ",";
				ofs << obj->Scale().x << "," << obj->Scale().y << "," << obj->Scale().z << ",";
			}
			else if (obj->editObj.name == "FallCheck")
			{
				ofs << "1" << "," << "FallCheck" << ",";
				ofs << obj->Position().x << "," << obj->Position().y << "," << obj->Position().z << ",";
			}
			// ���s
			ofs << endl;
		}
		else if (obj->IsTag("SCOREAREA"))
		{
			if (obj->editObj.name == "scoreArea1")
			{
				ofs << "1" << "," << "Area1" << ",";
				ofs << obj->Position().x << "," << obj->Position().y << "," << obj->Position().z << ",";
				ofs << obj->Scale().x << "," << obj->Scale().y << "," << obj->Scale().z << ",";
				ofs << obj->Rotation().x * 180.0f / XM_PI << "," << obj->Rotation().y * 180.0f / XM_PI << "," << obj->Rotation().z * 180.0f / XM_PI << ",";
			}
			else if (obj->editObj.name == "scoreArea2")
			{
				ofs << "1" << "," << "Area2" << ",";
				ofs << obj->Position().x << "," << obj->Position().y << "," << obj->Position().z << ",";
				ofs << obj->Scale().x << "," << obj->Scale().y << "," << obj->Scale().z << ",";
				ofs << obj->Rotation().x * 180.0f / XM_PI << "," << obj->Rotation().y * 180.0f / XM_PI << "," << obj->Rotation().z * 180.0f / XM_PI << ",";
			}
			else if (obj->editObj.name == "scoreArea3")
			{
				ofs << "1" << "," << "Area3" << ",";
				ofs << obj->Position().x << "," << obj->Position().y << "," << obj->Position().z << ",";
				ofs << obj->Scale().x << "," << obj->Scale().y << "," << obj->Scale().z << ",";
				ofs << obj->Rotation().x * 180.0f / XM_PI << "," << obj->Rotation().y * 180.0f / XM_PI << "," << obj->Rotation().z * 180.0f / XM_PI << ",";
			}
			ofs << endl;
		}
		else if (obj->IsTag("LINE"))
		{
			if (obj->editObj.name == "Line")
			{
				ofs << "1" << "," << "Line" << ",";
				ofs << obj->Position().x << "," << obj->Position().y << "," << obj->Position().z << ",";
				ofs << obj->Scale().x << "," << obj->Scale().y << "," << obj->Scale().z << ",";
			}
		// ���s
		ofs << endl;
		}
	}
	// �t�@�C�������
	ofs.close();
}

void StageEdit::Load(int n)
{
	// �I������Ă�I�u�W�F�N�g������ΑI������
	DeselectObj();
	
	// ���ݔz�u����Ă���I�u�W�F�N�g�����Z�b�g
	list<Object3D*> objs = FindGameObjects<Object3D>();
	for (Object3D* obj : objs)
	{
		if (obj->editObj.name == "Box" ||
			obj->editObj.name == "MoveBox"||
			obj->editObj.name == "Player"||
			obj->editObj.name == "Ball" ||
			obj->editObj.name == "scoreArea1" ||
			obj->editObj.name == "scoreArea2" ||
			obj->editObj.name == "scoreArea3"
			)
		{
			obj->DestroyMe();
		}
	}
	pNum = 0;
	char name[64];
	if (!isTestMap)
	{
		sprintf_s<64>(name, "data/StageCsv/Stage%02d.csv", n);
	}
	else
	{
		sprintf_s<64>(name, "data/TestStageCsv/testStage%02d.csv", n);
	}
	CsvReader* csv = new CsvReader(name);

	for (int i = 0; i < csv->GetLines(); i++) { // �P�s���ǂ�
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
			if (str == "PLAYER") 
			{
				float rotY = csv->GetFloat(i, 5);
				float e = csv->GetFloat(i, 6);
				float f = csv->GetFloat(i, 7);
				float mass = csv->GetFloat(i, 8);
				int num = csv->GetFloat(i, 9);
				obj = new Player(num, false);
				obj->SetRotation(VECTOR3(0, rotY / 180.0f * XM_PI, 0));
				obj->pObj.e = e;
				obj->pObj.f = f;
				obj->pObj.mass = mass;
				pNum++;
			}
			else if (str == "BOX") 
			{
				VECTOR3 size = VECTOR3(csv->GetFloat(i, 5), csv->GetFloat(i, 6), csv->GetFloat(i, 7));
				VECTOR3 rot = VECTOR3(csv->GetFloat(i, 8), csv->GetFloat(i, 9), csv->GetFloat(i, 10));
				float e = csv->GetFloat(i, 11);
				float f = csv->GetFloat(i, 12);
				obj = new Box(size, rot);	// �����̂̃T�C�Y�Ɖ�]�ʂ�n��
				obj->pObj.e = e;
				obj->pObj.f = f;
			}
			else if (str == "MBox") 
			{
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
			else if (str == "BALL")
			{
				float e = csv->GetFloat(i, 5);
				float f = csv->GetFloat(i, 6);
				float mass = csv->GetFloat(i, 7);
				obj = new Ball(false);
				obj->pObj.e = e;
				obj->pObj.f = f;
				obj->pObj.mass = mass;
			}
			else if (str == "Area1")
			{
				VECTOR3 size = VECTOR3(csv->GetFloat(i, 5), csv->GetFloat(i, 6), csv->GetFloat(i, 7));
				VECTOR3 rot = VECTOR3(csv->GetFloat(i, 8), csv->GetFloat(i, 9), csv->GetFloat(i, 10));
				obj = new ScoreArea1(size, rot);
			}
			else if (str == "Area2")
			{
				VECTOR3 size = VECTOR3(csv->GetFloat(i, 5), csv->GetFloat(i, 6), csv->GetFloat(i, 7));
				VECTOR3 rot = VECTOR3(csv->GetFloat(i, 8), csv->GetFloat(i, 9), csv->GetFloat(i, 10));
				obj = new ScoreArea2(size, rot);
			}
			else if (str == "Area3")
			{
				VECTOR3 size = VECTOR3(csv->GetFloat(i, 5), csv->GetFloat(i, 6), csv->GetFloat(i, 7));
				VECTOR3 rot = VECTOR3(csv->GetFloat(i, 8), csv->GetFloat(i, 9), csv->GetFloat(i, 10));
				obj = new ScoreArea3(size, rot);
			}
			else if (str == "FallCheck")
			{
				float x = csv->GetFloat(i, 2);
				float y = csv->GetFloat(i, 3);
				float z = csv->GetFloat(i, 4);
				fallCheck->pObj.center = VECTOR3(x, y, z);
				continue;
			}
			else if (str == "Line")
			{
				float x = csv->GetFloat(i, 2);
				float y = csv->GetFloat(i, 3);
				float z = csv->GetFloat(i, 4);
				VECTOR3 size = VECTOR3(csv->GetFloat(i, 5), csv->GetFloat(i, 6), csv->GetFloat(i, 7));
				line->SetScale(size);
				line->pObj.center = VECTOR3(x, y, z);
				continue;
			}
			else 
			{
				continue;
				//assert(false);
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
bool StageEdit::GetWorldPos()
{
	// �}�E�X���W�擾
	mousePos = GameDevice()->m_pDI->GetMousePos();
/*
	if (pDI->CheckMouse(KD_DAT, 0))
	{
		// �I�u�W�F�N�g�̃X�P�[���Ɖ�]��ς���Ƃ��E�B���h�E�����[�v����悤�ɂ�����

		if (mousePos.x > WINDOW_WIDTH)
		{
			//ClientToScreen(GameDevice()->m_pMain->m_hWnd, &mousePos);
			SetCursorPos(0, mousePos.y);
		}
		else if (mousePos.x < 0)
		{
			SetCursorPos(WINDOW_WIDTH, mousePos.y);
		}
		if (mousePos.y > WINDOW_HEIGHT)
		{
			SetCursorPos(mousePos.x, 0);
		}
		else if (mousePos.y < 0)
		{
			SetCursorPos(mousePos.x, WINDOW_HEIGHT);
		}
	}
*/

	// �ߎ��_(0)�Ɖ����_(1)
	nearWorldPos = XMVector3Unproject(VECTOR3(mousePos.x, mousePos.y, 0.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	farWorldPos = XMVector3Unproject(VECTOR3(mousePos.x, mousePos.y, 1.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);

	// �����x�N�g���𐳋K�����Ē��������΂�
	direction = XMVector3Normalize(farWorldPos - nearWorldPos);

	// ImGui�͈͓��̎���false��Ԃ�
	if (mousePos.x > judgeSkipArea0.x && mousePos.x < judgeSkipArea0.x + judgeSkipArea0.z &&
		mousePos.y > judgeSkipArea0.y && mousePos.y < judgeSkipArea0.y + judgeSkipArea0.w ||
		mousePos.x > judgeSkipArea1.x && mousePos.x < judgeSkipArea1.x + judgeSkipArea1.z &&
		mousePos.y > judgeSkipArea1.y && mousePos.y < judgeSkipArea1.y + judgeSkipArea1.w)
	return false;

if(getObj != nullptr)
{
	if (getObj->editObj.name != "MoveBox")
	{
		if (mousePos.x > judgeSkipArea2.x && mousePos.x < judgeSkipArea2.x + judgeSkipArea2.z &&
			mousePos.y > judgeSkipArea2.y && mousePos.y < judgeSkipArea2.y + judgeSkipArea2.w)
			return false;
	}
	else
	{
		if (mousePos.x > judgeSkipArea3.x && mousePos.x < judgeSkipArea3.x + judgeSkipArea3.z &&
			mousePos.y > judgeSkipArea3.y && mousePos.y < judgeSkipArea3.y + judgeSkipArea3.w)
			return false;
	}
}
	return true;
}
