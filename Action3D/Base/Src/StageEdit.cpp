#include "StageEdit.h"
#include "Box.h"
#include "Ball.h"

#define EXTENDED_DISTANCE 50;

// マウスのドラッグアンドドロップでステージオブジェクトの配置が理想
// Tankのレティクルの画面座標をワールド座標に変換するのが使えそう？
// オブジェクトを配置、削除したらcsvの行を詰めたい
StageEdit::StageEdit()
{
	//obj =new Box(VECTOR3(1,1,1), VECTOR3(0,0,0));
	new Ball;
	obj = new Ball();
	GameDevice()->m_mView = XMMatrixLookAtLH(
		VECTOR3(0, 20, -10),	// カメラ位置
		VECTOR3(0, 0, 0),	// 注視点
		VECTOR3(0, 1, 1));	// 上ベクトル
	GameDevice()->m_vEyePt = VECTOR3(0, 20, -10);
}

StageEdit::~StageEdit()
{
}

void StageEdit::Update()
{
	GetWorldPos();
	int x = (int)mousePos.x;
	int y = (int)mousePos.y;
	ImGui::Begin("MOUSEPOS");
	ImGui::InputInt("X", &x);
	ImGui::InputInt("Y", &y);
	//ImGui::InputFloat("Z", &worldPos.z);
	ImGui::End();
}

void StageEdit::Draw()
{
	CSprite cs;
	cs.DrawLine3D(nearWorldPos, extendedFarWorldPos, RGB(0, 255, 0), 1.0f);
	//cs.DrawLine3D(GameDevice()->m_vEyePt, farWorldPos, RGB(0, 255, 0), 1.0f);
	obj->SetPosition(extendedFarWorldPos);
}

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
	VECTOR3 direction = XMVector3Normalize(farWorldPos - nearWorldPos);
	extendedFarWorldPos = nearWorldPos + direction * EXTENDED_DISTANCE;  // EXTENDED_DISTANCEで延ばす


	// カメラからマウス座標にRayを飛ばしてLineToMeshで衝突したオブジェクトを Object* obj に入れる
	// マウスを動かしたら追従させる
	std::list<Object3D*> objs = ObjectManager::FindGameObjects<Object3D>();
	for (Object3D* ob : objs)
	{
		VECTOR3 hit;
		if (GameDevice()->m_pDI->CheckMouse(KD_TRG, 0))
		{
			if (ob->HitLineToMesh(nearWorldPos, extendedFarWorldPos, &hit))
			{
				ob->DestroyMe();
			}
		}
	}
	

	return farWorldPos;
}
