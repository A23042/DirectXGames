#include "StageEdit.h"
#include "Box.h"
#include "Ball.h"

#define EXTENDED_DISTANCE 50;	// Rayの長さ
#define EXTENDED_OBJ_DISTANCE 20;	// (仮)オブジェクトの移動場所

// マウスのドラッグアンドドロップでステージオブジェクトの配置が理想
// Tankのレティクルの画面座標をワールド座標に変換するのが使えそう？
// オブジェクトを配置、削除したらcsvの行を詰めたい
StageEdit::StageEdit()
{
	
	GameDevice()->m_mView = XMMatrixLookAtLH(
		VECTOR3(15, 20, -15),	// カメラ位置
		VECTOR3(0, 0, 0),	// 注視点
		VECTOR3(0, 1, 0));	// 上ベクトル
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

	// ImGuiで場所、回転、スケールを変える
	// InputFloatでの数値入力ができないからとりあえずInt型
	// 場所
	ImGui::Begin("OBJINFO");
	ImGui::InputInt("PositionX", &objPosX);
	ImGui::InputInt("PositionY", &objPosY);
	ImGui::InputInt("PositionZ", &objPosZ);
	// 回転
	ImGui::InputInt("RotateX", &objRotX);
	ImGui::InputInt("RotateY", &objRotY);
	ImGui::InputInt("RotateZ", &objRotZ);
	// スケール
	ImGui::InputInt("ScaleX", &objScaleX);
	ImGui::InputInt("scaleY", &objScaleY);
	ImGui::InputInt("ScaleZ", &objScaleZ);
	ImGui::End();

	// ImGuiで入力された値をオブジェクトに適用
	// 場所はObjectが持っている構造体のCenterに適用させる
	// <-それぞれのオブジェクトのUpdateでCenterをtransform.positinonに適用させてるから
	getObj->pObj.center = VECTOR3(objPosX, objPosY, objPosZ);	// 場所

	// 回転角度をラジアンから度数に変換
	VECTOR3 Rot = getObj->Rotation();
	Rot.x = objRotX / 180.0f * XM_PI;
	Rot.y = objRotY / 180.0f * XM_PI;
	Rot.z = objRotZ / 180.0f * XM_PI;
	getObj->SetRotation(Rot);	// 回転
	getObj->SetScale(VECTOR3(objScaleX, objScaleY, objScaleZ));	// スケール

}

// オブジェクトの選択
void StageEdit::SetObj(Object3D* ob)
{
	// 選択されてるオブジェクトの保存
	getObj = ob;

	// それぞれの値をImGuiで入力できるようにInt型に変換
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

// マウスカーソルのワールド座標の取得
VECTOR3 StageEdit::GetWorldPos()
{
	// マウス座標取得
	mousePos = GameDevice()->m_pDI->GetMousePos();

	// 各行列取得
	MATRIX4X4 mView = GameDevice()->m_mView;
	MATRIX4X4 mPrj = GameDevice()->m_mProj;
	MATRIX4X4 identity = XMMatrixIdentity();
	
	// 近視点(0)と遠視点(1)
	nearWorldPos = XMVector3Unproject(VECTOR3(mousePos.x, mousePos.y, 0.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	farWorldPos = XMVector3Unproject(VECTOR3(mousePos.x, mousePos.y, 1.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	
	// 方向ベクトルを正規化して長さを延ばす
	direction = XMVector3Normalize(farWorldPos - nearWorldPos);
	extendedFarWorldPos = nearWorldPos + direction * EXTENDED_DISTANCE;  // EXTENDED_DISTANCEで延ばす

	return farWorldPos;
}
