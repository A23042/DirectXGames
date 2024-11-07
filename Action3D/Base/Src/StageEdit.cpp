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
	gizmoObj->SetScale(VECTOR3(0.15f, 0.15f, 0.15f));
	gizmoObj->SetRotation(VECTOR3(0, 180.0f / 180 * XM_PI, 0));

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
	//CSprite* spr = new CSprite;
	//spr->DrawLine3D(nearWorldPos, farWorldPos, RGB(255, 0, 0), 1);
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

	// Stage読み書き用
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
		// 先に表示中のGizmoだけ衝突判定をとる
		std::list<GizmoXYZ*> gizmos = ObjectManager::FindGameObjectsVisible<GizmoXYZ>();
		if(!isHit)
		{
			for (GizmoXYZ* gizmo : gizmos)
			{
				VECTOR3 hit;
				if (gizmo->HitLineToMesh(nearWorldPos, farWorldPos, &hit))
				{
					float exDistance = (nearWorldPos - getObj->Position()).Length();
					extendedNearWorldPos = nearWorldPos + direction * exDistance;  // exDistanceで延ばす
					prevMousePos = extendedNearWorldPos;
					getGizmo = gizmo;
					isHit = true;
				}
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
	if (getObj->pObj.name != "MoveBox")
	{
		ImGui::SetNextWindowSize(ImVec2(290, 310));
	}
	else
	{
		ImGui::SetNextWindowSize(ImVec2(290, 400));
	}
	ImGui::Begin("OBJINFO");
	// 反発係数・摩擦・質量
	ImGui::SliderFloat("e", &e, 0.0f, 1.0f, "%.1f");
	ImGui::SliderFloat("f", &f, 0.0f, 1.0f, "%.2f");
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
	// MoveBoxの場合移動速と移動量
	if (getObj->pObj.name == "MoveBox")
	{
		ImGui::InputFloat("MoveSpeed", &moveSpeed, 0.5f);
		ImGui::InputFloat("MovementX", &moveVolumu.x, 0.5f);
		ImGui::InputFloat("MovementY", &moveVolumu.y, 0.5f);
		ImGui::InputFloat("MovementZ", &moveVolumu.z, 0.5f);
	}
	ImGui::End();

	// ImGuiで入力された値をオブジェクトに適用
	// 場所はObjectが持っている構造体のCenterに適用させる
	// <-それぞれのオブジェクトのUpdateでCenterをtransform.positinonに適用させてるから
	getObj->pObj.center = objPos;	// 場所
	// 回転角度をラジアンから度数に変換
	VECTOR3 Rot = getObj->Rotation();
	getObj->SetRotation(objRot / 180.0f * XM_PI);	// 回転
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
	getObj->SetScale(objScale);	// スケール

	if (getObj->pObj.name == "MoveBox")
	{

	}

	// 操作変更
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
// Gizmoのサイズが小さすぎてメッシュコライダーがうまく作られていない？
// 
// Position移動Gizmo表示中
void StageEdit::PosGizmoUpdate()
{
	if (getGizmo != nullptr)
	{
		if (getGizmo != posGizmoX)
		{
			ObjectManager::SetVisible(posGizmoX,false);
		}
		// オブジェクトの位置まで伸ばす
		float exDistance = (nearWorldPos - getObj->Position()).Length();	// オブジェクトまでの距離
		extendedNearWorldPos = nearWorldPos + direction * exDistance;  // exDistanceで延ばす
		// ここでGizmoを触ってオブジェクトを動かす
		VECTOR3 diff = extendedNearWorldPos - prevMousePos;
		if (diff.Length() != 0)
		{
			if (pDI->CheckMouse(KD_DAT, 0))
			{
				if (getGizmo->pObj.name == "posGizmoX")
				{
					ObjectManager::SetVisible(posGizmoY, false);
					ObjectManager::SetVisible(posGizmoZ, false);
					objPos.x +=  diff.x;
					getObj->pObj.center.x += diff.x;
				}
				else if (getGizmo->pObj.name == "posGizmoY")
				{
					ObjectManager::SetVisible(posGizmoX, false);
					ObjectManager::SetVisible(posGizmoZ, false);
					objPos.y += diff.y;
					getObj->pObj.center.y += diff.y;
				}
				else if (getGizmo->pObj.name == "posGizmoZ")
				{
					ObjectManager::SetVisible(posGizmoX, false);
					ObjectManager::SetVisible(posGizmoY, false);
					objPos.z += diff.z;
					getObj->pObj.center.z += diff.z;
				}
			}
		}
		prevMousePos = extendedNearWorldPos;
	}
	// getObjのスクリーン座標
	VECTOR3 objScreenPos = XMVector3Project(getObj->Position(), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	// getObjのスクリーン座標をワールド座標に変換
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
	// ここでGizmoを触ってオブジェクトを動かす
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
				if (getGizmo->pObj.name == "rotGizmoX")
				{
					//objRot.x = (objRot.x / 180.0f * XM_PI) + (diff.x * 10 / 180.0f * XM_PI);
					//getObj->SetRotation(VECTOR3((objRot.x / 180.0f * XM_PI) + (diff.x * 10 / 180.0f * XM_PI), getObj->Rotation().y, getObj->Rotation().z));
				}
				else if (getGizmo->pObj.name == "rotGizmoY")
				{
					objRot.y = (objRot.y / 180.0f * XM_PI) + (diff.y * 10 / 180.0f * XM_PI);
					getObj->SetRotation(VECTOR3(objRot.x, (objRot.y / 180.0f * XM_PI) + (diff.y * 10 / 180.0f * XM_PI), objRot.z));
				}
				else if (getGizmo->pObj.name == "rotGizmoZ")
				{
					//objRot.z = diff.z / 180.0f * XM_PI;
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
	if (pDI->CheckMouse(KD_UTRG, 0))
	{
		getGizmo = nullptr;
	}
}

void StageEdit::ScaleGizmoUpdate()
{
	// ここでGizmoを触ってオブジェクトを動かす
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
				if (getGizmo->pObj.name == "scaleGizmoX")
				{
					objScale.x += diff.x * 2;
				}
				else if (getGizmo->pObj.name == "scaleGizmoY")
				{
					objScale.y += diff.y * 2;
				}
				else if (getGizmo->pObj.name == "scaleGizmoZ")
				{
					objScale.z += diff.z * 2;
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
	if (pDI->CheckMouse(KD_UTRG, 0))
	{
		getGizmo = nullptr;
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
	// 選択されてるオブジェクトの保存
	getObj = ob;
	// 選択されてるオブジェクトのGizmo表示
	// 初めてGizmoが出る場合posGizmoを出す
	if (gState == sNoneGizmo)
	{
		gState = sPosGizmo;
	}
	// ステータスによって表示するGizmoを変える
	SetGizmo(gState);

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
	SetGizmo(sNoneGizmo);
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
			ofs << "1" << "," << "BOX" << ",";
			ofs << ob->Position().x << "," << ob->Position().y << "," << ob->Position().z << ",";
			ofs << ob->Scale().x * 2 << "," << ob->Scale().y * 2 << "," << ob->Scale().z * 2 << ",";
			ofs << ob->Rotation().x * 180.0f / XM_PI << "," << ob->Rotation().y * 180.0f / XM_PI << "," << ob->Rotation().z * 180.0f / XM_PI << ",";
			ofs << ob->pObj.e << "," << ob->pObj.f;
			
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

	for (int i = 1; i < csv->GetLines(); i++) { // １行ずつ読む
		std::string str = csv->GetString(i, 0); // 先頭の数字を取る
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
				obj = new Box(size, rot);	// 直方体のサイズと回転量を渡す
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
				obj = new MoveBox(size, rot, move, moveSpeed);	// 直方体のサイズと回転量、移動量を渡す
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
