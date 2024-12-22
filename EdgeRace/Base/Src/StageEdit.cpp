// 2024.10.06~ S.Matsunaga	編集中

#include <fstream>

#include "StageEdit.h"
#include "HierarchyManager.h"
#include "Box.h"
#include "Ball.h"
#include "MoveBox.h"
#include "Player.h"
#include "Gizmo3D.h"
#include "GizmoXYZ.h"
#include "ScoreArea.h"
#include "FallCheck.h"
#include "Line.h"
#include "CommandManager.h"
#include "MoveCommand.h"
#include "RotationCommand.h"
#include "ScaleCommand.h"
#include "CreateCommand.h"
#include "DeleteCommand.h"

namespace
{
	// 各ImGuiの座標とサイズ指定
	// ImGui上のクリックではオブジェクト選択を飛ばすため
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

	// 判定を飛ばすエリア指定左上の座標と右下の座標
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


	static const float defaultE = 0.8f;	// 反発係数
	static const float defaultF = 0.02f;	// 摩擦
	static const float defaultMass = 1;	// 質量

	CommandManager commandManager;
};

// マウスのドラッグアンドドロップでステージオブジェクトの配置が理想
// Tankのレティクルの画面座標をワールド座標に変換するのが使えそう？
// オブジェクトを配置、削除したらcsvの行を詰めたい
StageEdit::StageEdit()
{
	new Gizmo3D();

	// 親Gizmoと移動、回転サイズGizmoの初期化
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

	nState = sNone;
	gState = sNoneGizmo;

	tempE = defaultE;
	tempF = defaultF;
	tempMass = defaultMass;

	commandManager.SetUp();
}

StageEdit::~StageEdit()
{
	HierarchyManager::ClearHierarchy();
}

void StageEdit::Update()
{
	// 各行列とカーソルのワールド座標取得
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

	// オブジェクト作成ボタンImGui
	CreateObjImGui();
	// ヒエラルキーImGui
	HierarchyImGui();
	// ステージ読み書き用ImGui
	StageImGui();

	// Undo、Redoコマンド
	if (pDI->CheckKey(KD_DAT, DIK_LCONTROL) && pDI->CheckKey(KD_TRG, DIK_Z))
	{
		commandManager.Undo();
		DeselectObj();
	}
	if (pDI->CheckKey(KD_DAT, DIK_LCONTROL) && pDI->CheckKey(KD_TRG, DIK_Y))
	{
		commandManager.Redo();
		DeselectObj();
	}

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
	// ImGui外でマウス左クリック
	if (CheckInAreaCursor())
	{
		if (GameDevice()->m_pDI->CheckMouse(KD_TRG, 0))
		{
			Object3D* temp = nullptr;
			// Gizmo以外のオブジェクトと衝突判定
			list<Object3D*> objs = FindGameObjectsWithOutTag<Object3D>("Gizmo");
			VECTOR3 hit;
			temp = GetClosestHitObject(objs, hit);
			if (temp != nullptr)
			{
				SelectObj(temp);
				// 衝突した場所
				GetNearWorldPosEx();
				prevMousePos = nearWorldPosEx;
				return;
			}
		}
	}
}

void StageEdit::HasUpdate()
{
	// ショートカットキーコマンド
	Command();
	GizmoUpdate();
	// マウス左クリック
	if(CheckInAreaCursor())
	{
		if (GameDevice()->m_pDI->CheckMouse(KD_TRG, 0))
		{
			bool isHit = false;
			// オブジェクト探索
			// 先に表示中のGizmoだけ衝突判定をとる
			list<Object3D*> gizmos = FindGameObjectsVisibleWithTag<Object3D>("Gizmo");
			Object3D* temp = nullptr;	// 衝突したオブジェクトの一時格納
			VECTOR3 hit = VECTOR3();
			temp = GetClosestHitObject(gizmos, hit);	// カーソルに重なってる一番近いオブジェクトを取る
			if (temp != nullptr)
			{
				// オブジェクトの中心点を求める
				GetObjCenter(selectObj);
				// 衝突した場所
				GetNearWorldPosEx();
				prevMousePos = nearWorldPosEx;
				getGizmo = temp;
				
				// 操作前情報を登録しておく
				// クリックしたギズモによって各GizumoのUpdateを回す
				if (getGizmo == posGizmoX || getGizmo == posGizmoY || getGizmo == posGizmoZ)
				{
					oldPos.clear();
					for(Object3D* obj : selectObj)
					{
						oldPos.push_back(obj->Position());
					}
					gState = sPosGizmo;
				}
				else if (getGizmo == rotGizmoX || getGizmo == rotGizmoY || getGizmo == rotGizmoZ)
				{
					oldRot.clear();
					for(Object3D* obj : selectObj)
					{
						oldRot.push_back(obj->Rotation());
					}
					gState = sRotGizmo;
				}
				else if (getGizmo == scaleGizmoX || getGizmo == scaleGizmoY || getGizmo == scaleGizmoZ)
				{
					oldScale.clear();
					for(Object3D* obj : selectObj)
					{
						oldScale.push_back(obj->Scale());
					}
					gState = sScaleGizmo;
				}
				isHit = true;
			}
			// Gizmoに当たってなければ衝突判定をとる
			if (!isHit)
			{
				// Gizmo以外のオブジェクトを調べる
				list<Object3D*> objs = FindGameObjectsVisibleWithOutTag<Object3D>("Gizmo");
				VECTOR3 hit;
				temp = GetClosestHitObject(objs, hit);
				if (temp != nullptr)
				{
					SelectObj(temp);
				}
				else
				{
					// 何も触ってなければ選択解除
					DeselectObj();
				}
			}
		}
	}

	// ImGuiで場所、回転、スケールを変える
	ImGui::SetNextWindowPos(objInfoImPos);
	ImGui::SetNextWindowSize(objInfoImSize);

	//Object3D* obj;
	string name = "OBJINFO";
	{
		ImGui::SetNextWindowPos(objInfoImPos);
		ImGui::SetNextWindowSize(objInfoImSize);
		ImGui::Begin("Inspector");
		ImGuiTabBarFlags tab_bar_flag = ImGuiBackendFlags_None;
		if (ImGui::BeginTabBar("OBJECTINFO", tab_bar_flag))
		{
			int i = 1;
			for(Object3D* obj : selectObj)
			{
				for (Object3D* tmp : HierarchyManager::GetHierarchyList())
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
					// 場所
					ImGui::InputFloat("PositionX", &tmpPos.x, 0.1f, 0.5f, "%.2f");
					ImGui::InputFloat("PositionY", &tmpPos.y, 0.1f, 0.5f, "%.2f");
					ImGui::InputFloat("PositionZ", &tmpPos.z, 0.1f, 0.5f, "%.2f");

					obj->pObj.center = tmpPos;
					obj->SetPosition(tmpPos);

					// 回転
					VECTOR3 rot = obj->Rotation();
					ImGui::InputFloat("RotateX", &tmpRot.x, 5.0f, 0.5f, "%.2f");
					ImGui::InputFloat("RotateY", &tmpRot.y, 5.0f, 0.5f, "%.2f");
					ImGui::InputFloat("RotateZ", &tmpRot.z, 5.0f, 0.5f, "%.2f");
					
					obj->SetRotation(tmpRot / 180 * XM_PI);

					// スケール
					VECTOR3 scale = obj->Scale();
					ImGui::InputFloat("ScaleX", &tmpScale.x, 0.1f, 0.5f, "%.2f");
					ImGui::InputFloat("scaleY", &tmpScale.y, 0.1f, 0.5f, "%.2f");
					ImGui::InputFloat("ScaleZ", &tmpScale.z, 0.1f, 0.5f, "%.2f");
					if (tmpScale.x < 0.1f)
					{
						tmpScale.x = 0.1f;
					}
					else if (tmpScale.y < 0.1f)
					{
						tmpScale.y = 0.1f;
					}
					else if (tmpScale.z < 0.1f)
					{
						tmpScale.z = 0.1f;
					}
					obj->SetScale(tmpScale);

					ImGui::EndTabItem();
				}
				
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
	

	// ImGuiで入力された値をオブジェクトに適用
	
	// 場所はObjectが持っている構造体のCenterに適用させる
	// それぞれのオブジェクトのUpdateでCenterをtransform.positinonに適用させてるから

	// 回転角度をラジアンから度数に変換

	//getObj->SetRotation(objRot / 180.0f * XM_PI);	// 回転
	// スケールの最小値は0.1
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
	//getObj->SetScale(objScale);	// スケール

	//if (getObj->editObj.name == "MoveBox")
	{
		// ここにMoveBoxで移動速度、移動量を渡す
	}

	// 操作変更
	if (!pDI->CheckMouse(KD_DAT, 1))
	{
		if (pDI->CheckKey(KD_TRG, DIK_W))
		{
			vGizmo = vPos;
			SetGizmo();
		}
		else if (pDI->CheckKey(KD_TRG, DIK_R))
		{
			vGizmo = vRot;
			SetGizmo();
		}
		else if (pDI->CheckKey(KD_TRG, DIK_E))
		{
			vGizmo = vScale;
			SetGizmo();
		}
	}

	// ギズモの表示状態でステータス分け
	switch (gState)
	{
	case sNoneGizmo:
		break;
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
void StageEdit::GizmoUpdate()
{
	GetObjCenter(selectObj);
	// getObjのスクリーン座標
	VECTOR3 objScreenPos = XMVector3Project(objCenter, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	// getObjのスクリーン座標をワールド座標に変換
	VECTOR3 gizmoWorldPos = XMVector3Unproject(VECTOR3(objScreenPos.x, objScreenPos.y, 0.2f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	gizmoC->SetPosition(gizmoWorldPos);

}

// Position移動Gizmo表示中
void StageEdit::PosGizmoUpdate()
{
	if (getGizmo != nullptr)
	{
		// 選択されているGizmoのみ表示
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
		// オブジェクトの位置まで伸ばす
		GetNearWorldPosEx();
		// ここでGizmoを触ってオブジェクトを動かす
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
	if (pDI->CheckMouse(KD_UTRG, 0))
	{
		commandManager.Do(make_shared<MoveCommand>(selectObj, oldPos));
		
		getGizmo = nullptr;
		SetVisible(posGizmoX, true);
		SetVisible(posGizmoY, true);
		SetVisible(posGizmoZ, true);
		gState = sNoneGizmo;
	}
}

void StageEdit::RotGizmoUpdate()
{
	// ここでGizmoを触ってオブジェクトを動かす
	if (getGizmo != nullptr)
	{
		// 選択されているGizmoのみ表示
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
		// ここでGizmoを触ってオブジェクトを動かす
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
	if (pDI->CheckMouse(KD_UTRG, 0))
	{
		commandManager.Do(make_shared<RotationCommand>(selectObj, oldRot));

			getGizmo = nullptr;
		SetVisible(rotGizmoX, true);
		SetVisible(rotGizmoY, true);
		SetVisible(rotGizmoZ, true);
		gState = sNoneGizmo;
	}
}

void StageEdit::ScaleGizmoUpdate()
{
	// ここでGizmoを触ってオブジェクトを動かす
	if (getGizmo != nullptr)
	{
		// 選択されているGizmoのみ表示
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
		// オブジェクトの位置まで伸ばす
		GetNearWorldPosEx();
		// ここでGizmoを触ってオブジェクトを動かす
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
	if (pDI->CheckMouse(KD_UTRG, 0))
	{
		commandManager.Do(make_shared<ScaleCommand>(selectObj, oldScale));

		getGizmo = nullptr;
		SetVisible(scaleGizmoX, true);
		SetVisible(scaleGizmoY, true);
		SetVisible(scaleGizmoZ, true);
		gState = sNoneGizmo;
	}
}

void StageEdit::SetGizmo()
{
	switch (vGizmo)
	{
	case vPos:
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
	case vRot:
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
	case vScale:
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
	//LCtrl押されてたら
	if (pDI->CheckKey(KD_DAT, DIK_LCONTROL))
	{
		// listの中にすでにあれば選択解除
		if (find(selectObj.begin(), selectObj.end(), obj) != selectObj.end())
		{
			DeselectObj(obj);
			selectObj.remove(obj);
		}
		// なければ追加
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
		HierarchyManager::AddHierarchy(obj);
		isNew = false;
	}
	// 選択されたオブジェクトの色を変える
	//getObj->GetMesh()->m_vDiffuse = VECTOR4(1.0f, 0.2f, 1.0f, 1.0f);
	for (Object3D* obj : selectObj)
	{
		obj->GetMesh()->m_vDiffuse = VECTOR4(1.0f, 0.2f, 1.0f, 1.0f);
	}

	// 選択されてるオブジェクトのGizmo表示
	// 初めてGizmoが出る場合posGizmoを出す
	if (vGizmo == vNone)
	{
		vGizmo = vPos;
	}

	objPos = obj->pObj.center;
	objRot = obj->Rotation() * 180.0f / XM_PI;
	objScale = obj->Scale();

	tempE = obj->pObj.e;
	tempF = obj->pObj.f;
	tempMass = obj->pObj.mass;

	// ステータスによって表示するGizmoを変える
	SetGizmo();

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
			// 色をもとに戻す
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
	SetGizmo();
}

void StageEdit::DeleteObj()
{
	for (Object3D* obj : selectObj)
	{
		if(obj->editObj.name != "FallCheck")
		{
			HierarchyManager::RemoveHierarchy(obj);
		}
	}
	commandManager.Do(make_shared<DeleteCommand<Object3D>>(selectObj));
	selectObj.clear();
}

void StageEdit::CloneObj()
{
	// コピーされたオブジェクトをtempに保存する
	list<Object3D*> tempList = {};
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
		// 複製されたオブジェクトを一つづつ配列に格納する
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
			HierarchyManager::AddHierarchy(tempObj);
		}
	}
	DeselectObj();
	selectObj = tempList;
	commandManager.Do(make_shared<CreateCommand<Object3D>>(tempList));	// Object3Dではなくそれぞれの型にしなければならない
	for (Object3D* obj : selectObj)
	{
		obj->editObj.isSelect = true;
		obj->GetMesh()->m_vDiffuse = VECTOR4(1.0f, 0.2f, 1.0f, 1.0f);
	}
	// ステータスによって表示するGizmoを変える
	SetGizmo();
	nState = sHas;

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
	// ファイルを開く
	ofstream ofs(name); // 引数にファイル名
	// データを書く
	// セーブするためにオブジェクト探索

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
			// 対応するものがなければ無視
			else
			{
				continue;
			}
			// 改行
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
			// 改行
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
		// 改行
		ofs << endl;
		}
	}
	// ファイルを閉じる
	ofs.close();
}

void StageEdit::Load(int n)
{
	// 選択されてるオブジェクトがあれば選択解除
	DeselectObj();
	
	// 現在配置されているオブジェクトをリセット
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

	for (int i = 0; i < csv->GetLines(); i++) { // １行ずつ読む
		string str = csv->GetString(i, 0); // 先頭の数字を取る
		Object3D* obj = nullptr;
		// 先頭が「0」の場合はスキップ
		if (str == "0")
		{
			continue;
		}
		// 「１」の場合だけ進む
		else if (str == "1")
		{
			str = csv->GetString(i, 1);
			if (str == "PLAYER") 
			{
				obj = new Player(csv->GetFloat(i, 9), false);
				obj->SetRotation(VECTOR3(0, csv->GetFloat(i, 5), 0));
				obj->pObj.e = csv->GetFloat(i, 6);
				obj->pObj.f = csv->GetFloat(i, 7);
				obj->pObj.mass = csv->GetFloat(i, 8);
				pNum++;
			}
			else if (str == "BOX") 
			{
				obj = new Box(csv->GetVector3(i, 5), csv->GetVector3(i, 8));	// 直方体のサイズと回転量を渡す
				obj->pObj.e = csv->GetFloat(i, 11);
				obj->pObj.f = csv->GetFloat(i, 12);
			}
			else if (str == "MBox") 
			{
				obj = new MoveBox(csv->GetVector3(i, 5), csv->GetVector3(i, 8), csv->GetVector3(i, 11), csv->GetVector3(i, 14));	// 直方体のサイズと回転量、移動量を渡す
				obj->pObj.e = csv->GetFloat(i, 17);
				obj->pObj.f = csv->GetFloat(i, 18);
			}
			else if (str == "BALL")
			{
				obj = new Ball(false);
				obj->pObj.e = csv->GetFloat(i, 5);
				obj->pObj.f = csv->GetFloat(i, 6);
				obj->pObj.mass = csv->GetFloat(i, 7);
			}
			else if (str == "Area1")
			{
				obj = new ScoreArea1(csv->GetVector3(i, 5), csv->GetVector3(i, 8));
			}
			else if (str == "Area2")
			{
				obj = new ScoreArea2(csv->GetVector3(i, 5), csv->GetVector3(i, 8));
			}
			else if (str == "Area3")
			{
				obj = new ScoreArea3(csv->GetVector3(i, 5), csv->GetVector3(i, 8));
			}
			else if (str == "FallCheck")
			{
				fallCheck->pObj.center = csv->GetVector3(i, 2);
				continue;
			}
			else if (str == "Line")
			{
				obj = new Line();
				obj->SetScale(csv->GetVector3(i, 5));
			}
			else 
			{
				continue;
				//assert(false);
			}
			obj->SetPosition(csv->GetVector3(i, 2));
			HierarchyManager::AddHierarchy(obj);
		}
	}
	delete csv;
}

bool StageEdit::CursorLoop()
{
	POINT clientPosLT = { 0,0 };	// ウィンドウの左上座標
	ClientToScreen(GameDevice()->m_pMain->m_hWnd, &clientPosLT);

	POINT clientPosRU = { 0,0 };	// ウィンドウの右下座標
	clientPosRU.x = clientPosLT.x + WINDOW_WIDTH;
	clientPosRU.y = clientPosLT.y + WINDOW_HEIGHT;

	POINT currsorClientPos = { 0,0 };	// カーソルのウィンドウ上の座標
	currsorClientPos.x = clientPosLT.x + mousePos.x;
	currsorClientPos.y = clientPosLT.y + mousePos.y;

	// カーソルがウィンドウの外にでたら
	if (currsorClientPos.x > clientPosRU.x)
	{
		SetCursorPos(clientPosLT.x, currsorClientPos.y);
		return true;
	}
	else if (currsorClientPos.x < clientPosLT.x)
	{
		SetCursorPos(clientPosRU.x, mousePos.y);
		return true;
	}
	if (currsorClientPos.y > clientPosRU.y)
	{
		SetCursorPos(currsorClientPos.x, clientPosLT.y);
		return true;
	}
	else if (currsorClientPos.y < clientPosLT.y)
	{
		SetCursorPos(currsorClientPos.x, clientPosRU.y);
		return true;
	}
	return false;
}

// マウスカーソルのワールド座標の取得
void StageEdit::GetWorldPos()
{
	// マウス座標取得
	mousePos = GameDevice()->m_pDI->GetMousePos();

	// 近視点(0)と遠視点(1)
	nearWorldPos = XMVector3Unproject(VECTOR3(mousePos.x, mousePos.y, 0.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	farWorldPos = XMVector3Unproject(VECTOR3(mousePos.x, mousePos.y, 1.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);

	// 方向ベクトルを正規化して長さを延ばす
	direction = XMVector3Normalize(farWorldPos - nearWorldPos);

	return;
}

void StageEdit::CheckResetObj(list<Object3D*> objs)
{
	for (Object3D* obj : objs)
	{
		// 判定を取るオブジェクト
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
			// ヒエラルキーから削除
			HierarchyManager::RemoveHierarchy(obj);
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
		Box* temp = new Box();
		SelectObj(temp);
		commandManager.Do(make_shared<CreateCommand<Box>>(temp));
	}
	if (ImGui::Button("MoveBox"))
	{
		isNew = true;
		MoveBox* temp = new MoveBox();
		SelectObj(temp);
		commandManager.Do(make_shared<CreateCommand<MoveBox>>(temp));
	}
	if (ImGui::Button("Area1"))
	{
		isNew = true;
		ScoreArea1* temp = new ScoreArea1();
		SelectObj(temp);
		commandManager.Do(make_shared<CreateCommand<ScoreArea1>>(temp));
	}
	if (ImGui::Button("Area2"))
	{
		isNew = true;
		ScoreArea2* temp = new ScoreArea2();
		SelectObj(temp);
		commandManager.Do(make_shared<CreateCommand<ScoreArea2>>(temp));
	}
	if (ImGui::Button("Area3"))
	{
		isNew = true;
		ScoreArea3* temp = new ScoreArea3();
		SelectObj(temp);
		commandManager.Do(make_shared<CreateCommand<ScoreArea3>>(temp));
	}
	if (ImGui::Button("Line"))
	{
		isNew = true;
		Line* temp = new Line();
		SelectObj(temp);
		commandManager.Do(make_shared<CreateCommand<Line>>(temp));
	}
	if (ImGui::Button("Ball"))
	{
		isNew = true;
		Ball* temp = new Ball(false);
		SelectObj(temp);
		commandManager.Do(make_shared<CreateCommand<Ball>>(temp));
	}
	// Playerは２つまで
	if (pNum <= 1)
	{
		if (ImGui::Button("Player"))
		{
			isNew = true;
			Player* temp = new Player(pNum, false);
			SelectObj(temp);
			commandManager.Do(make_shared<CreateCommand<Player>>(temp));
			pNum++;
		}
	}
	ImGui::End();
}

void StageEdit::HierarchyImGui()
{
	int i = 0;	// オブジェクト番号
	ImGui::SetNextWindowPos(objHierarchyImPos);
	ImGui::SetNextWindowSize(objHierarchyImSize);

	ImGui::Begin("HIERARCHY");
	for (Object3D* obj : HierarchyManager::GetHierarchyList())
	{
		i++;
		// ボタンにオブジェクトの番号 + 名前を入れる
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
	// Stage読み書き用
	ImGui::SetNextWindowPos(stageImPos);
	ImGui::SetNextWindowSize(stageImSize);
	ImGui::Begin("MENU");
	ImGui::InputInt("Stage", &stageNum);
	if (ImGui::Button("SAVE"))
	{
		if (MessageBox(GameDevice()->m_pMain->m_hWnd, "上書きセーブしますか", "セーブ", MB_OKCANCEL) == IDOK)
		{
			Save(stageNum);
		}
	}
	if (ImGui::Button("LOAD"))
	{
		if (MessageBox(GameDevice()->m_pMain->m_hWnd, "現在のマップを上書きロードしますか", "ロード", MB_OKCANCEL) == IDOK)
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
	// 選択解除
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_N))
	{
		DeselectObj();
		return;	// 以下コード省略
	}
	// オブジェクト削除
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_DELETE))
	{
		DeleteObj();
		return;
	}
	// Ctrl + D オブジェクト複製
	if (pDI->CheckKey(KD_DAT, DIK_LCONTROL) && pDI->CheckKey(KD_TRG, DIK_D))
	{
		CloneObj();
		return;	// 以下コード省略
	}
}

Object3D* StageEdit::GetClosestHitObject(list<Object3D*>objs, VECTOR3 &closestHit)
{
	Object3D* temp = nullptr;
	// 探索された最初のオブジェクトか
	bool firstFlag = true;
	// naerWorldPosから当たった場所までの距離
	float distance = 0.0f;
	// 当たったオブジェクトのなかでの最短距離
	float minDistance = 0.0f;

	for (Object3D* obj : objs)
	{
		VECTOR3 hit;
		if (obj->HitLineToMesh(nearWorldPos, farWorldPos, &hit))
		{
			// 当たった場所への距離を求めて一番近いオブジェクトを格納する
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
	nearWorldPosEx = nearWorldPos + direction * exDistance;  // exDistanceで延ばす
	return ;
}

bool StageEdit::CheckInAreaCursor()
{
	POINT clientPosLT = { 0,0 };	// ウィンドウの左上座標
	ClientToScreen(GameDevice()->m_pMain->m_hWnd, &clientPosLT);

	POINT clientPosRU = { 0,0 };	// ウィンドウの右下座標
	clientPosRU.x = clientPosLT.x + WINDOW_WIDTH;
	clientPosRU.y = clientPosLT.y + WINDOW_HEIGHT;

	POINT currsorClientPos = { 0,0 };	// カーソルのウィンドウ上の座標
	currsorClientPos.x = clientPosLT.x + mousePos.x;
	currsorClientPos.y = clientPosLT.y + mousePos.y;

	// カーソルがウィンドウの外にでたら
	if (currsorClientPos.x > clientPosRU.x)
	{
		return false;
	}
	else if (currsorClientPos.x < clientPosLT.x)
	{
		return false;
	}
	if (currsorClientPos.y > clientPosRU.y)
	{
		return false;
	}
	else if (currsorClientPos.y < clientPosLT.y)
	{
		return false;
	}
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
