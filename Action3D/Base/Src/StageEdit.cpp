#include "StageEdit.h"
#include "Box.h"
#include "Ball.h"
#include "MoveBox.h"
#include "Player.h"
#include "Gizmo3D.h"
#include "GizmoXYZ.h"
#include <fstream>

#define EXTENDED_DISTANCE 50;	// Rayの長さ
#define EXTENDED_OBJ_DISTANCE 20;	// (仮)オブジェクトの移動場所

// マウスのドラッグアンドドロップでステージオブジェクトの配置が理想
// Tankのレティクルの画面座標をワールド座標に変換するのが使えそう？
// オブジェクトを配置、削除したらcsvの行を詰めたい
StageEdit::StageEdit()
{
	gizmoObj = new Gizmo3D();
	gizmoObj->SetScale(VECTOR3(0.0015f, 0.0015f, 0.0015f));
	gizmoObj->SetRotation(VECTOR3(0, 180.0f / 180 * XM_PI, 0));
	//GameDevice()->m_mView = XMMatrixLookAtLH(
	//	VECTOR3(10, 20, -15),	// カメラ位置
	//	VECTOR3(0, 0, 0),	// 注視点
	//	VECTOR3(0, 1, 0));	// 上ベクトル
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

	// 3DGizmo表示位置
	// Windowの左下
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

	// オブジェクト作成ボタンImGui
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
				return;	// 以下コード省略
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
	// 選択解除
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_N))
	{
		getObj = nullptr;
		nState = sNone;
		return;	// 以下コード省略
	}
	// オブジェクト削除
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_DELETE))
	{
		getObj->DestroyMe();
		getObj = nullptr;
		nState = sNone;
		return;
	}
	// マウス左クリック
	if (GameDevice()->m_pDI->CheckMouse(KD_TRG, 0))
	{
		// オブジェクト探索
		std::list<Object3D*> objs = ObjectManager::FindGameObjects<Object3D>();
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
				// 違うオブジェクトがクリックされたら選択オブジェクト変更
				SetObj(ob);
				return;	// 以下コード省略
			}
			else
			{
				//getObj = nullptr;
				//nState = sNone;
				//return;
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
	//objRot = objRot / 180.0f * XM_PI;
	getObj->SetRotation(objRot / 180.0f * XM_PI);	// 回転
	getObj->SetScale(objScale);	// スケール

}

void StageEdit::SetObj(Object3D* ob)
{
	// 選択されてるオブジェクトの保存
	getObj = ob;

	//gizmoXYZ = new GizmoXYZ();

	// それぞれの値をImGui用の変数に保管
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
