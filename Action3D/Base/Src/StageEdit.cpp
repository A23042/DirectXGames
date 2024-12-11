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

using namespace ObjectManager;

namespace
{
	// �eImGui�̍��W�ƃT�C�Y�w��
	// ImGui��̃N���b�N�ł̓I�u�W�F�N�g�I�����΂�����
	ImVec2 createObjImPos = ImVec2(30, 30);
	ImVec2 createObjImSize = ImVec2(130, 130);

	ImVec2 stageImPos = ImVec2(170, 30);
	ImVec2 stageImSize = ImVec2(200, 130);
	
	// inspector
	ImVec2 objInfoImPos = ImVec2(WINDOW_WIDTH - 320, 170);
	ImVec2 objInfoImSize = ImVec2(290, 310);

	// hierarchy
	ImVec2 objHierarchyImPos = ImVec2(30, 170);
	ImVec2 objHierarchyImSize = ImVec2(250, 350);

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
		objInfoImSize.x, objInfoImSize.y
	);

	VECTOR4 judgeSkipArea3 = VECTOR4(
		objHierarchyImPos.x, objHierarchyImPos.y,
		objHierarchyImSize.x, objHierarchyImSize.y
	);


	static const float defaultE = 0.8f;	// �����W��
	static const float defaultF = 0.02f;	// ���C
	static const float defaultMass = 1;	// ����
};

// �}�E�X�̃h���b�O�A���h�h���b�v�ŃX�e�[�W�I�u�W�F�N�g�̔z�u�����z
// Tank�̃��e�B�N���̉�ʍ��W�����[���h���W�ɕϊ�����̂��g�������H
// �I�u�W�F�N�g��z�u�A�폜������csv�̍s���l�߂���
StageEdit::StageEdit()
{
	new Gizmo3D();

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

	//line = new Line();

	nState = sNone;
	gState = sNoneGizmo;

	tempE = defaultE;
	tempF = defaultF;
	tempMass = defaultMass;
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
	judgeArea = CheckInAreaCursor();
	GetWorldPos();
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
	CreateObjImGui();
	// �q�G�����L�[ImGui
	HierarchyImGui();
	// �X�e�[�W�ǂݏ����pImGui
	StageImGui();
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
	if (CheckInAreaCursor())
	{
		if (GameDevice()->m_pDI->CheckMouse(KD_TRG, 0))
		{
			Object3D* temp = nullptr;
			// Gizmo�ȊO�̃I�u�W�F�N�g�ƏՓ˔���
			list<Object3D*> objs = FindGameObjectsWithOutTag<Object3D>("Gizmo");
			VECTOR3 hit;
			temp = GetClosestHitObject(objs, hit);
			if (temp != nullptr)
			{
				SelectObj(temp);
				// �Փ˂����ꏊ
				GetNearWorldPosEx();
				prevMousePos = nearWorldPosEx;
				return;
			}
		}
	}
}

void StageEdit::HasUpdate()
{
	// �V���[�g�J�b�g�L�[�R�}���h
	Command();

	// �}�E�X���N���b�N
	if(CheckInAreaCursor())
	{
		if (GameDevice()->m_pDI->CheckMouse(KD_TRG, 0))
		{
			bool isHit = false;
			// �I�u�W�F�N�g�T��
			// ��ɕ\������Gizmo�����Փ˔�����Ƃ�
			list<Object3D*> gizmos = FindGameObjectsVisibleWithTag<Object3D>("Gizmo");
			Object3D* temp = nullptr;
			VECTOR3 hit = VECTOR3();
			temp = GetClosestHitObject(gizmos, hit);	// �J�[�\���ɏd�Ȃ��Ă��ԋ߂��I�u�W�F�N�g�����
			if (temp != nullptr)
			{
				// �I�u�W�F�N�g�̒��S�_�����߂�
				GetObjCenter(selectObj);
				// �Փ˂����ꏊ
				GetNearWorldPosEx();
				prevMousePos = nearWorldPosEx;
				getGizmo = temp;
				isHit = true;
			}
			// Gizmo�ɓ������ĂȂ���ΏՓ˔�����Ƃ�
			if (!isHit)
			{
				// Gizmo�ȊO�̃I�u�W�F�N�g�𒲂ׂ�
				list<Object3D*> objs = FindGameObjectsWithOutTag<Object3D>("Gizmo");
				VECTOR3 hit;
				temp = GetClosestHitObject(objs, hit);
				if (temp != nullptr)
				{
					SelectObj(temp);
				}
				else
				{
					DeselectObj();
				}
			}
		}
	}

	// ImGui�ŏꏊ�A��]�A�X�P�[����ς���
	ImGui::SetNextWindowPos(objInfoImPos);
	ImGui::SetNextWindowSize(objInfoImSize);

	Object3D* obj;
	string name;
#if 0
	if (selectObj.size() == 1)
	{
		obj = selectObj.front();
		name = "OBJINFO:" + obj->editObj.name;
		if (obj->editObj.name == "Player")
		{
			name = name + to_string(obj->pObj.pNum);
		}
	}
	else
#endif
	{
		name = "OBJINFO";
	}
#if 0
	if (selectObj.size() < 1)
	{
		ImGui::Begin(name.c_str());
		// �����W���E���C�E����
		ImGui::SliderFloat("e", &tempE, 0.0f, 1.0f, "%.1f");
		ImGui::SliderFloat("f", &tempF, 0.0f, 0.1f, "%.2f");
		ImGui::InputFloat("mass", &tempMass, 0.5f, 1.0f);
		ImGui::SameLine();
		ImGui::Separator();

		// �ꏊ
		ImGui::InputFloat("PositionX", &objPos.x, 0.1f, 0.5f, "%.2f");
		ImGui::InputFloat("PositionY", &objPos.y, 0.1f, 0.5f, "%.2f");
		ImGui::InputFloat("PositionZ", &objPos.z, 0.1f, 0.5f, "%.2f");
		ImGui::SameLine();
		ImGui::Separator();

		// ��]
		ImGui::InputFloat("RotateX", &objRot.x, 0.1f, 0.5f, "%.2f");
		ImGui::InputFloat("RotateY", &objRot.y, 0.1f, 0.5f, "%.2f");
		ImGui::InputFloat("RotateZ", &objRot.z, 0.1f, 0.5f, "%.2f");
		ImGui::SameLine();
		ImGui::Separator();

		// �X�P�[��
		ImGui::InputFloat("ScaleX", &objScale.x, 0.1f, 0.5f, "%.2f");
		ImGui::InputFloat("scaleY", &objScale.y, 0.1f, 0.5f, "%.2f");
		ImGui::InputFloat("ScaleZ", &objScale.z, 0.1f, 0.5f, "%.2f");
		// MoveBox�̏ꍇ�ړ����ƈړ���
		//if (getObj->editObj.name == "MoveBox")
		{
			ImGui::InputFloat("MoveSpeed", &moveSpeed, 0.5f);
			ImGui::InputFloat("MovementX", &moveVolumu.x, 0.5f);
			ImGui::InputFloat("MovementY", &moveVolumu.y, 0.5f);
			ImGui::InputFloat("MovementZ", &moveVolumu.z, 0.5f);
		}
		ImGui::End();
	}
	else
#endif
	{
		ImGui::SetNextWindowPos(objInfoImPos);
		ImGui::SetNextWindowSize(objInfoImSize);
		ImGui::Begin("Inspector");
		ImGuiTabBarFlags tab_bar_flag = ImGuiBackendFlags_None;
		if (ImGui::BeginTabBar("OBJECTINFO", tab_bar_flag))
		{
#if 0
			VECTOR3 tmpPos = {};

			if (ImGui::BeginTabItem("AllObj"))
			{
				ImGui::SliderFloat("e", &tempE, 0.0f, 1.0f, "%.1f");
				ImGui::SliderFloat("f", &tempF, 0.0f, 0.1f, "%.2f");
				ImGui::InputFloat("mass", &tempMass, 0.5f, 1.0f);
				// �ꏊ
				VECTOR3 pos = obj->Position();
				ImGui::InputFloat("PositionX", &tmpPos.x, 0.1f, 0.5f, "%.2f");
				ImGui::InputFloat("PositionY", &tmpPos.y, 0.1f, 0.5f, "%.2f");
				ImGui::InputFloat("PositionZ", &tmpPos.z, 0.1f, 0.5f, "%.2f");

				obj->pObj.center = tmpPos;
				obj->SetPosition(tmpPos);

				// ��]
				VECTOR3 rot = obj->Rotation();
				ImGui::InputFloat("RotateX", &rot.x, 0.1f, 0.5f, "%.2f");
				ImGui::InputFloat("RotateY", &rot.y, 0.1f, 0.5f, "%.2f");
				ImGui::InputFloat("RotateZ", &rot.z, 0.1f, 0.5f, "%.2f");
				// �X�P�[��
				VECTOR3 scale = obj->Scale();
				ImGui::InputFloat("ScaleX", &scale.x, 0.1f, 0.5f, "%.2f");
				ImGui::InputFloat("scaleY", &scale.y, 0.1f, 0.5f, "%.2f");
				ImGui::InputFloat("ScaleZ", &scale.z, 0.1f, 0.5f, "%.2f");
			
				for (Object3D* obj : selectObj)
				{
					obj->pObj.center = tmpPos;
				}

				ImGui::EndTabItem();

			}
#endif
			int i = 1;
			for(Object3D* obj : selectObj)
			{
				for (Object3D* tmp : hierarchyObj)
				{
					if(tmp == obj)
					{
						break;
					}
					i++;
				}

				VECTOR3 tmpPos = obj->Position();
				VECTOR3 tmpRot = obj->Rotation() * 180 / XM_PI;
				VECTOR3 tmpScale = obj->Scale();

				name = "[" + to_string(i) + "] :" + obj->editObj.name;
				if (ImGui::BeginTabItem(name.c_str()))
				{
					ImGui::SliderFloat("e", &obj->pObj.e, 0.0f, 1.0f, "%.1f");
					ImGui::SliderFloat("f", &obj->pObj.f, 0.0f, 0.1f, "%.2f");
					ImGui::InputFloat("mass", &obj->pObj.mass, 0.5f, 1.0f);
					// �ꏊ
					ImGui::InputFloat("PositionX", &tmpPos.x, 0.1f, 0.5f, "%.2f");
					ImGui::InputFloat("PositionY", &tmpPos.y, 0.1f, 0.5f, "%.2f");
					ImGui::InputFloat("PositionZ", &tmpPos.z, 0.1f, 0.5f, "%.2f");

					obj->pObj.center = tmpPos;
					obj->SetPosition(tmpPos);

					// ��]
					VECTOR3 rot = obj->Rotation();
					ImGui::InputFloat("RotateX", &tmpRot.x, 5.0f, 0.5f, "%.2f");
					ImGui::InputFloat("RotateY", &tmpRot.y, 5.0f, 0.5f, "%.2f");
					ImGui::InputFloat("RotateZ", &tmpRot.z, 5.0f, 0.5f, "%.2f");
					
					obj->SetRotation(tmpRot / 180 * XM_PI);

					// �X�P�[��
					VECTOR3 scale = obj->Scale();
					ImGui::InputFloat("ScaleX", &tmpScale.x, 0.1f, 0.5f, "%.2f");
					ImGui::InputFloat("scaleY", &tmpScale.y, 0.1f, 0.5f, "%.2f");
					ImGui::InputFloat("ScaleZ", &tmpScale.z, 0.1f, 0.5f, "%.2f");

					obj->SetScale(tmpScale);

					ImGui::EndTabItem();
				}
				
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
	

	// ImGui�œ��͂��ꂽ�l���I�u�W�F�N�g�ɓK�p
	
	// �ꏊ��Object�������Ă���\���̂�Center�ɓK�p������
	// ���ꂼ��̃I�u�W�F�N�g��Update��Center��transform.positinon�ɓK�p�����Ă邩��

	// ��]�p�x�����W�A������x���ɕϊ�

	//getObj->SetRotation(objRot / 180.0f * XM_PI);	// ��]
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
	//getObj->SetScale(objScale);	// �X�P�[��

	//if (getObj->editObj.name == "MoveBox")
	{
		// ������MoveBox�ňړ����x�A�ړ��ʂ�n��
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
}
// 
// Position�ړ�Gizmo�\����
void StageEdit::PosGizmoUpdate()
{
	gizmoC->SetRotation(VECTOR3());
	GetObjCenter(selectObj);

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
		GetNearWorldPosEx();
		// ������Gizmo��G���ăI�u�W�F�N�g�𓮂���
		VECTOR3 diff = nearWorldPosEx - prevMousePos;
		if (diff.Length() != 0)
		{
			if (pDI->CheckMouse(KD_DAT, 0))
			{
				if (getGizmo->editObj.name == "posGizmoX")
				{
					objPos.x += diff.x;
					for(Object3D* obj : selectObj)
					{
						obj->pObj.center.x += diff.x;
					}
				}
				else if (getGizmo->editObj.name == "posGizmoY")
				{
					objPos.y += diff.y;
					for (Object3D* obj : selectObj)
					{
						obj->pObj.center.y += diff.y;
					}
				}
				else if (getGizmo->editObj.name == "posGizmoZ")
				{
					objPos.z += diff.z;
					for (Object3D* obj : selectObj)
					{
						obj->pObj.center.z += diff.z;
					}
				}
				else if (getGizmo->editObj.name == "gizmoCenter")
				{
					objPos += diff;
					for (Object3D* obj : selectObj)
					{
						obj->pObj.center += diff;
					}
				}
			}
		}
		prevMousePos = nearWorldPosEx;
	}

	// getObj�̃X�N���[�����W
	VECTOR3 objScreenPos = XMVector3Project(objCenter, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
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
	//gizmoC->SetRotation(getObj->Rotation());
	GetObjCenter(selectObj);


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
		GetNearWorldPosEx();
		// ������Gizmo��G���ăI�u�W�F�N�g�𓮂���
		VECTOR3 diff = nearWorldPosEx - prevMousePos;
		if (diff.Length() != 0)
		{
			if (pDI->CheckMouse(KD_DAT, 0))
			{
				if (CursorLoop())
				{
					GetWorldPos();
					prevMousePos = nearWorldPosEx;
					return;
				}

				if (getGizmo->editObj.name == "rotGizmoX")
				{
					objRot.x = (objRot.x) + (diff.y * 200 / 180.0f * XM_PI);
					for (Object3D* obj : selectObj)
					{
						obj->SetRotation(obj->Rotation().x + (diff.y / 180.0f * XM_PI), obj->Rotation().y, obj->Rotation().z);
					}
				}
				else if (getGizmo->editObj.name == "rotGizmoY")
				{
					objRot.y = (objRot.y) - (diff.x * 100 / 180.0f * XM_PI) + (-diff.z * 100 / 180.0f * XM_PI);
					for (Object3D* obj : selectObj)
					{
						obj->SetRotation(obj->Rotation().x, obj->Rotation().y - (diff.x / 180.0f * XM_PI) + (-diff.z / 180.0f * XM_PI), obj->Rotation().z);
					}
				}
				else if (getGizmo->editObj.name == "rotGizmoZ")
				{
					objRot.z = (objRot.z) + (diff.y * 200 / 180.0f * XM_PI);
					for (Object3D* obj : selectObj)
					{
						obj->SetRotation(obj->Rotation().x, obj->Rotation().y, obj->Rotation().z + (diff.y / 180.0f * XM_PI));
					}
				}
			}
		}
		GetNearWorldPosEx();
		prevMousePos = nearWorldPosEx;
	}
	// getObj�̃X�N���[�����W
	VECTOR3 objScreenPos = XMVector3Project(objCenter, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
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
	//gizmoC->SetRotation(getObj->Rotation());
	GetObjCenter(selectObj);


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
		GetNearWorldPosEx();
		// ������Gizmo��G���ăI�u�W�F�N�g�𓮂���
		VECTOR3 diff = nearWorldPosEx - prevMousePos;
		if (diff.Length() != 0)
		{
			if (pDI->CheckMouse(KD_DAT, 0))
			{
				if (CursorLoop())
				{
					GetWorldPos();
					GetNearWorldPosEx();
					prevMousePos = nearWorldPosEx;
					return;
				}
				if (getGizmo->editObj.name == "scaleGizmoX")
				{
					objScale.x += diff.x * 2;
					for (Object3D* obj : selectObj)
					{
						obj->SetScale(VECTOR3(obj->Scale().x + diff.x * 2, obj->Scale().y, obj->Scale().z));
					}
				}
				else if (getGizmo->editObj.name == "scaleGizmoY")
				{
					objScale.y += diff.y * 2;
					for (Object3D* obj : selectObj)
					{
						obj->SetScale(VECTOR3(obj->Scale().x, obj->Scale().y + diff.y * 2, obj->Scale().z));
					}
				}
				else if (getGizmo->editObj.name == "scaleGizmoZ")
				{
					objScale.z += diff.z * 2;
					for (Object3D* obj : selectObj)
					{
						obj->SetScale(VECTOR3(obj->Scale().x, obj->Scale().y, obj->Scale().z + diff.z * 2));
					}
				}
				else if (getGizmo->editObj.name == "gizmoCenter")
				{
					objScale += diff * 2;
					for (Object3D* obj : selectObj)
					{
						obj->SetScale(obj->Scale() + diff * 2);
					}
				}

			}
		}
		GetNearWorldPosEx();
		prevMousePos = nearWorldPosEx;
	}
	// getObj�̃X�N���[�����W
	VECTOR3 objScreenPos = XMVector3Project(objCenter, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
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

void StageEdit::SelectObj(Object3D* obj)
{
	//LCtrl������Ă���
	if (pDI->CheckKey(KD_DAT, DIK_LCONTROL))
	{
		// list�̒��ɂ��łɂ���ΑI������
		if (find(selectObj.begin(), selectObj.end(), obj) != selectObj.end())
		{
			DeselectObj(obj);
			selectObj.remove(obj);
		}
		// �Ȃ���Βǉ�
		else
		{
			selectObj.push_back(obj);
		}
	}
	else
	{
		DeselectObj();
		selectObj.push_back(obj);
	}
	if (isNew)
	{
		obj->pObj.e = defaultE;
		obj->pObj.f = defaultF;
		obj->pObj.mass = defaultMass;
		//selectObj.push_back(obj);
		hierarchyObj.push_back(obj);
		isNew = false;
	}
	// �I�����ꂽ�I�u�W�F�N�g�̐F��ς���
	//getObj->GetMesh()->m_vDiffuse = VECTOR4(1.0f, 0.2f, 1.0f, 1.0f);
	for (Object3D* obj : selectObj)
	{
		obj->GetMesh()->m_vDiffuse = VECTOR4(1.0f, 0.2f, 1.0f, 1.0f);
	}

	// �I������Ă�I�u�W�F�N�g��Gizmo�\��
	// ���߂�Gizmo���o��ꍇposGizmo���o��
	if (gState == sNoneGizmo)
	{
		gState = sPosGizmo;
	}

	objPos = obj->pObj.center;
	objRot = obj->Rotation() * 180.0f / XM_PI;
	objScale = obj->Scale();

	tempE = obj->pObj.e;
	tempF = obj->pObj.f;
	tempMass = obj->pObj.mass;

	// �X�e�[�^�X�ɂ���ĕ\������Gizmo��ς���
	SetGizmo(gState);

	//getObj->editObj.isSelect = true;
	obj->editObj.isSelect = true;
	nState = sHas;
}

void StageEdit::DeselectObj(Object3D* obj)
{
	//if(getObj != nullptr)
	if(obj == nullptr)
	{
		if (selectObj.size() != 0)
		{
			// �F�����Ƃɖ߂�
			for (Object3D* obj : selectObj)
			{
				obj->GetMesh()->m_vDiffuse = VECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			}
			selectObj.clear();
		}
	}
	else
	{
		if (find(selectObj.begin(), selectObj.end(), obj) != selectObj.end())
		{
			obj->GetMesh()->m_vDiffuse = VECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			selectObj.remove(obj);
		}
	}

	nState = sNone;
	SetGizmo(sNoneGizmo);
}

void StageEdit::DeleteObj()
{
	for (Object3D* obj : selectObj)
	{
		if(obj->editObj.name != "FallCheck")
		{
			hierarchyObj.remove(obj);
			obj->DestroyMe();
		}
	}
	selectObj.clear();
}

void StageEdit::DupeObj()
{
	// �R�s�[���ꂽ�I�u�W�F�N�g��temp�ɕۑ�����
	std::list<Object3D*> tempList = {};
	Object3D* tempObj = nullptr;
	for (Object3D* obj : selectObj)
	{
		if (obj->editObj.name == "Box")
		{
			tempObj = new Box();
		}
		else if (obj->editObj.name == "MoveBax")
		{
			tempObj = new MoveBox();
		}
		else if (obj->editObj.name == "Ball")
		{
			tempObj = new Ball(false);
		}
		else if (obj->editObj.name == "Player")
		{
			if (pNum <= 1)
			{
				tempObj = new Player(pNum, false);
				pNum++;
			}
		}
		else if (obj->editObj.name == "scoreArea1")
		{
			tempObj = new ScoreArea1();
		}
		else if (obj->editObj.name == "scoreArea2")
		{
			tempObj = new ScoreArea2();
		}
		else if (obj->editObj.name == "scoreArea3")
		{
			tempObj = new ScoreArea3();
		}
		else if (obj->editObj.name == "Line")
		{
			tempObj = new Line();
		}
		if(tempObj != nullptr)
		{
			tempObj->pObj.center = obj->pObj.center;
			tempObj->SetPosition(tempObj->pObj.center);
			tempObj->SetRotation(obj->Rotation());
			tempObj->SetScale(obj->Scale());

			tempObj->pObj.e = obj->pObj.e;
			tempObj->pObj.f = obj->pObj.f;
			tempObj->pObj.mass = obj->pObj.mass;
			tempList.push_back(tempObj);
			hierarchyObj.push_back(tempObj);
		}
	}
	DeselectObj();
	selectObj = tempList;
	for (Object3D* obj : selectObj)
	{
		obj->editObj.isSelect = true;
		obj->GetMesh()->m_vDiffuse = VECTOR4(1.0f, 0.2f, 1.0f, 1.0f);

	}
	// �X�e�[�^�X�ɂ���ĕ\������Gizmo��ς���
	SetGizmo(gState);
	nState = sHas;

#if 0
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
#endif
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
	CheckResetObj(objs);

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
				obj->SetRotation(VECTOR3(0, rotY, 0));
				obj->pObj.e = e;
				obj->pObj.f = f;
				obj->pObj.mass = mass;
				pNum++;
			}
			else if (str == "BOX") 
			{
				VECTOR3 size = csv->GetVector3(i, 5);
				VECTOR3 rot = csv->GetVector3(i, 8);
				float e = csv->GetFloat(i, 11);
				float f = csv->GetFloat(i, 12);
				obj = new Box(size, rot);	// �����̂̃T�C�Y�Ɖ�]�ʂ�n��
				obj->pObj.e = e;
				obj->pObj.f = f;
			}
			else if (str == "MBox") 
			{
				VECTOR3 size = csv->GetVector3(i, 5);
				VECTOR3 rot = csv->GetVector3(i, 8);
				VECTOR3 move = csv->GetVector3(i, 11);
				VECTOR3 moveSpeed = csv->GetVector3(i, 14);
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
				VECTOR3 size = csv->GetVector3(i, 5);
				VECTOR3 rot = csv->GetVector3(i, 8);
				obj = new ScoreArea1(size, rot);
			}
			else if (str == "Area2")
			{
				VECTOR3 size = csv->GetVector3(i, 5);
				VECTOR3 rot = csv->GetVector3(i, 8);
				obj = new ScoreArea2(size, rot);
			}
			else if (str == "Area3")
			{
				obj = new ScoreArea3(csv->GetVector3(i, 5), csv->GetVector3(i, 8));
			}
			else if (str == "FallCheck")
			{
				VECTOR3 pos = csv->GetVector3(i, 2);
				fallCheck->pObj.center = pos;
				continue;
			}
			else if (str == "Line")
			{
				VECTOR3 size = csv->GetVector3(i, 5);
				obj = new Line();
				obj->SetScale(size);
			}
			else 
			{
				continue;
				//assert(false);
			}
			VECTOR3 pos = csv->GetVector3(i, 2);
			obj->SetPosition(pos);
			hierarchyObj.push_back(obj);
		}
	}
	delete csv;
}

bool StageEdit::CursorLoop()
{
	POINT clientPos = { 0,0 };
	ClientToScreen(GameDevice()->m_pMain->m_hWnd, &clientPos);

	if (clientPos.x + mousePos.x > clientPos.x + WINDOW_WIDTH)
	{
		SetCursorPos(clientPos.x, clientPos.y + mousePos.y);
		return true;
	}
	else if (clientPos.x + mousePos.x < clientPos.x)
	{
		SetCursorPos(clientPos.x + WINDOW_WIDTH, mousePos.y);
		return true;
	}
	if (clientPos.y + mousePos.y > clientPos.y + WINDOW_HEIGHT)
	{
		SetCursorPos(clientPos.x + mousePos.x, clientPos.y);
		return true;
	}
	else if (clientPos.y + mousePos.y < clientPos.y)
	{
		SetCursorPos(clientPos.x + mousePos.x, clientPos.y + WINDOW_HEIGHT);
		return true;
	}
	return false;
}

// �}�E�X�J�[�\���̃��[���h���W�̎擾
bool StageEdit::GetWorldPos()
{
	// �}�E�X���W�擾
	mousePos = GameDevice()->m_pDI->GetMousePos();

	// �ߎ��_(0)�Ɖ����_(1)
	nearWorldPos = XMVector3Unproject(VECTOR3(mousePos.x, mousePos.y, 0.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	farWorldPos = XMVector3Unproject(VECTOR3(mousePos.x, mousePos.y, 1.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);

	// �����x�N�g���𐳋K�����Ē��������΂�
	direction = XMVector3Normalize(farWorldPos - nearWorldPos);

	return CheckInAreaCursor();
}

void StageEdit::CheckResetObj(list<Object3D*> objs)
{
	for (Object3D* obj : objs)
	{
		// ��������I�u�W�F�N�g
		if (obj->editObj.name == "Box" ||
			obj->editObj.name == "MoveBox" ||
			obj->editObj.name == "Player" ||
			obj->editObj.name == "Ball" ||
			obj->editObj.name == "scoreArea1" ||
			obj->editObj.name == "scoreArea2" ||
			obj->editObj.name == "scoreArea3" ||
			obj->editObj.name == "Line"
			)
		{
			// �q�G�����L�[����폜
			hierarchyObj.remove(obj);
			obj->DestroyMe();
		}
	}
}

void StageEdit::CreateObjImGui()
{
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
	if (ImGui::Button("Line"))
	{
		isNew = true;
		SelectObj(new Line());
	}
	if (ImGui::Button("Ball"))
	{
		isNew = true;
		SelectObj(new Ball(false));
	}
	// Player�͂Q�܂�
	if (pNum <= 1)
	{
		if (ImGui::Button("Player"))
		{
			isNew = true;
			SelectObj(new Player(pNum, false));
			pNum++;
		}
	}
	ImGui::End();
}

void StageEdit::HierarchyImGui()
{
	int i = 0;	// �I�u�W�F�N�g�ԍ�
	ImGui::SetNextWindowPos(objHierarchyImPos);
	ImGui::SetNextWindowSize(objHierarchyImSize);

	ImGui::Begin("HIERARCHY");
	for (Object3D* obj : hierarchyObj)
	{
		i++;
		// �{�^���ɃI�u�W�F�N�g�̔ԍ� + ���O������
		string name = "[" + to_string(i) + "] : " + obj->editObj.name;
		if (ImGui::Button(name.c_str()))
		{
			SelectObj(obj);
		}
	}
	ImGui::End();
}

void StageEdit::StageImGui()
{
	// Stage�ǂݏ����p
	ImGui::SetNextWindowPos(stageImPos);
	ImGui::SetNextWindowSize(stageImSize);
	ImGui::Begin("MENU");
	ImGui::InputInt("Stage", &stageNum);
	if (ImGui::Button("SAVE"))
	{
		if (MessageBox(GameDevice()->m_pMain->m_hWnd, "�㏑���Z�[�u���܂���", "�Z�[�u", MB_OKCANCEL) == IDOK)
		{
			Save(stageNum);
		}
	}
	if (ImGui::Button("LOAD"))
	{
		if (MessageBox(GameDevice()->m_pMain->m_hWnd, "���݂̃}�b�v���㏑�����[�h���܂���", "���[�h", MB_OKCANCEL) == IDOK)
		{
			Load(stageNum);
		}
	}
	if (ImGui::Button("PLAY"))
	{
		SceneManager::ChangeScene("PlayScene", stageNum);
	}
	ImGui::End();
}

void StageEdit::Command()
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
		DeleteObj();
		return;
	}
	// Ctrl + D �I�u�W�F�N�g����
	if (pDI->CheckKey(KD_DAT, DIK_LCONTROL) && pDI->CheckKey(KD_TRG, DIK_D))
	{
		DupeObj();
		return;	// �ȉ��R�[�h�ȗ�
	}
}

Object3D* StageEdit::GetClosestHitObject(list<Object3D*>objs, VECTOR3 &closestHit)
{
	Object3D* temp = nullptr;
	// �T�����ꂽ�ŏ��̃I�u�W�F�N�g��
	bool firstFlag = true;
	// naerWorldPos���瓖�������ꏊ�܂ł̋���
	float distance = 0.0f;
	// ���������I�u�W�F�N�g�̂Ȃ��ł̍ŒZ����
	float minDistance = 0.0f;

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
					closestHit = hit;
					temp = obj;
				}
			}
		}
	}
	if (temp != nullptr)
	{
		return temp;
	}
	return nullptr;
}

void StageEdit::GetObjCenter(list<Object3D*> objs)
{
	int i = 0;
	objCenter = VECTOR3();
	for (Object3D* obj : selectObj)
	{
		i++;
		objCenter += obj->Position();
	}
	objCenter /= i;
	return;
}

void StageEdit::GetNearWorldPosEx()
{
	float exDistance = (nearWorldPos - objCenter).Length();
	nearWorldPosEx = nearWorldPos + direction * exDistance;  // exDistance�ŉ��΂�
	return ;
}

bool StageEdit::CheckInAreaCursor()
{
	if (mousePos.x > judgeSkipArea0.x && mousePos.x < judgeSkipArea0.x + judgeSkipArea0.z &&
		mousePos.y > judgeSkipArea0.y && mousePos.y < judgeSkipArea0.y + judgeSkipArea0.w ||
		mousePos.x > judgeSkipArea1.x && mousePos.x < judgeSkipArea1.x + judgeSkipArea1.z &&
		mousePos.y > judgeSkipArea1.y && mousePos.y < judgeSkipArea1.y + judgeSkipArea1.w ||
		mousePos.x > judgeSkipArea2.x && mousePos.x < judgeSkipArea2.x + judgeSkipArea2.z &&
		mousePos.y > judgeSkipArea2.y && mousePos.y < judgeSkipArea2.y + judgeSkipArea2.w||
		mousePos.x > judgeSkipArea3.x && mousePos.x < judgeSkipArea3.x + judgeSkipArea3.z &&
		mousePos.y > judgeSkipArea3.y && mousePos.y < judgeSkipArea3.y + judgeSkipArea3.w)
	return false;
}
