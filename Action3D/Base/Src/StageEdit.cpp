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
	static const int EXTENDED_DISTANCE = 50;	// Rayの長さ
	static const int EXTENDED_OBJ_DISTANCE = 20;	// (仮)オブジェクトの移動場所
}

// マウスのドラッグアンドドロップでステージオブジェクトの配置が理想
// Tankのレティクルの画面座標をワールド座標に変換するのが使えそう？
// オブジェクトを配置、削除したらcsvの行を詰めたい
StageEdit::StageEdit()
{
	// 左下Gizmo初期化
	gizmoObj = new Gizmo3D();
	gizmoObj->SetScale(VECTOR3(0.0015f, 0.0015f, 0.0015f));
	gizmoObj->SetRotation(VECTOR3(0, 180.0f / 180 * XM_PI, 0));

	// 親Gizmoと移動、回転サイズGizmoの初期化
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

	// 3DGizmo表示位置
	// Windowの左下
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

	// オブジェクト作成ボタンImGui
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
				return;	// 以下コード省略
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
	// 選択解除
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_N))
	{
		DeselectObj();
		return;	// 以下コード省略
	}
	// オブジェクト削除
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_DELETE))
	{
		getObj->DestroyMe();
		DeselectObj();
		return;
	}
	// マウス左クリック
	if (GameDevice()->m_pDI->CheckMouse(KD_TRG, 0))
	{
		bool isHit = false;
		// オブジェクト探索
		// 先にGizmoだけ衝突判定をとる
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
		// Gizmoに当たってなければ衝突判定をとる
		if(!isHit)
		{
			// Gizmo以外のオブジェクトを調べる
			std::list<Object3D*> objs = ObjectManager::FindGameObjectsWithOutTag<Object3D>("Gizmo");
			for (Object3D* ob : objs)
			{
				VECTOR3 hit;
				// カーソルのワールド座標の近視点から遠視点の距離を伸ばした点までのRayを飛ばしす
				if (ob->HitLineToMesh(nearWorldPos, extendedFarWorldPos, &hit))
				{
					// 現在選択状態のオブジェクトの場合何もしない
					if (ob == getObj)
					{
						break;
					}
					else
					{
						// 違うオブジェクトがクリックされたら選択オブジェクト変更
						SelectObj(ob);
						return;	// 以下コード省略
					}
				}
			}
		}
	}

	// Ctrl + D オブジェクト複製
	if (pDI->CheckKey(KD_DAT, DIK_LCONTROL) && pDI->CheckKey(KD_TRG, DIK_D))
	{
		DupeObj(getObj);
		return;	// 以下コード省略
	}

	// ImGuiで場所、回転、スケールを変える
	ImGui::SetNextWindowPos(ImVec2(30, 170));
	ImGui::SetNextWindowSize(ImVec2(290, 310));
	ImGui::Begin("OBJINFO");
	// 反発係数・摩擦・質量
	ImGui::SliderFloat("e", &e, 0.0f, 1.0f);
	ImGui::SliderFloat("f", &f, 0.0f, 1.0f);
	ImGui::InputFloat("mass", &mass, 0.5f, 1.0f);
	// 場所
	ImGui::InputFloat("PositionX", &objPos.x, 0.5f);
	ImGui::InputFloat("PositionY", &objPos.y, 0.5f);
	ImGui::InputFloat("PositionZ", &objPos.z, 0.5f);
	// 回転
	ImGui::InputFloat("RotateX", &objRot.x, 0.5f);
	ImGui::InputFloat("RotateY", &objRot.y, 0.5f);
	ImGui::InputFloat("RotateZ", &objRot.z, 0.5f);
	// スケール
	ImGui::InputFloat("ScaleX", &objScale.x, 0.5f);
	ImGui::InputFloat("scaleY", &objScale.y, 0.5f);
	ImGui::InputFloat("ScaleZ", &objScale.z, 0.5f);
	ImGui::End();

	// ImGuiで入力された値をオブジェクトに適用
	// 場所はObjectが持っている構造体のCenterに適用させる
	// <-それぞれのオブジェクトのUpdateでCenterをtransform.positinonに適用させてるから
	getObj->pObj.center = objPos;	// 場所

	// 回転角度をラジアンから度数に変換
	VECTOR3 Rot = getObj->Rotation();
	getObj->SetRotation(objRot / 180.0f * XM_PI);	// 回転
	getObj->SetScale(objScale);	// スケール

	// 操作変更
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

	// ギズモの表示状態でステータス分け
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
// Gizmoをクリック出来ない問題発生中
// 原因はおそらくGizmoの位置とRayの発射位置が両方nearWorldPosになっているから？
// 
// 
// Position移動Gizmo表示中
void StageEdit::PosGizmoUpdate()
{
	if (getGizmo != nullptr)
	{
		// オブジェクトの位置まで伸ばす
		float exDistance = (nearWorldPos - getObj->Position()).Length();
		VECTOR3 nearWorldPosEx = nearWorldPos + direction * exDistance;  // exDistanceで延ばす
		// ここでGizmoを触ってオブジェクトを動かす
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
		nearWorldPosEx = nearWorldPos + direction * exDistance;  // exDistanceで延ばす
		prevMousePos = nearWorldPosEx;
	}
	// getObjのスクリーン座標
	VECTOR3 objScreenPos = XMVector3Project(getObj->Position(), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	// getObjのスクリーン座標をワールド座標に変換
	VECTOR3 gizmoWorldPos = XMVector3Unproject(VECTOR3(objScreenPos.x, objScreenPos.y, 0.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	gizmoC->SetPosition(gizmoWorldPos);
}

void StageEdit::RotGizmoUpdate()
{
	// ここでGizmoを触ってオブジェクトを動かす

	// getObjのスクリーン座標
	VECTOR3 objScreenPos = XMVector3Project(getObj->Position(), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	// getObjのスクリーン座標をワールド座標に変換
	VECTOR3 gizmoWorldPos = XMVector3Unproject(VECTOR3(objScreenPos.x, objScreenPos.y, 0.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	gizmoC->SetPosition(gizmoWorldPos);
}

void StageEdit::ScaleGizmoUpdate()
{
	// ここでGizmoを触ってオブジェクトを動かす

	// getObjのスクリーン座標
	VECTOR3 objScreenPos = XMVector3Project(getObj->Position(), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	// getObjのスクリーン座標をワールド座標に変換
	VECTOR3 gizmoWorldPos = XMVector3Unproject(VECTOR3(objScreenPos.x, objScreenPos.y, 0.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	gizmoC->SetPosition(gizmoWorldPos);
}

void StageEdit::SelectObj(Object3D* ob)
{
	// 選択されてるオブジェクトの保存
	getObj = ob;

	// 選択されてるオブジェクトのGizmo表示
	// 初めてGizmoが出る場合posGizmoを出す
	if (gState == sNoneGizmo)
	{
		gState = sPosGizmo;
	}
	// ステータスによって表示するGizmoを変える
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

	// それぞれの値をImGui用の変数に保管
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
	// ファイルを開く
	std::ofstream ofs(name); // 引数にファイル名
	// データを書く
	// セーブするためにオブジェクト探索
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
		// 改行
		ofs << std::endl;
	}
	// ファイルを閉じる
	ofs.close();
}

void StageEdit::Load(int n)
{
	char name[64];
	sprintf_s<64>(name, "data/Stage%02d.csv", n);

	CsvReader* csv = new CsvReader(name);
	
	delete csv;
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
	extendedFarWorldPos = nearWorldPos + direction * EXTENDED_DISTANCE;  // EXTENDED_DISTANCEで延ばす
	return;
}
