#include "StageEdit.h"
#include "Box.h"

// マウスのドラッグアンドドロップでステージオブジェクトの配置が理想
// Tankのレティクルの画面座標をワールド座標に変換するのが使えそう？
// オブジェクトを配置、削除したらcsvの行を詰めたい
StageEdit::StageEdit()
{
	obj =new Box(VECTOR3(1,1,1), VECTOR3(0,0,0));

	GameDevice()->m_mView = XMMatrixLookAtLH(
		VECTOR3(0, 20, -10),	// カメラ位置
		VECTOR3(0, 0, 0),	// 注視点
		VECTOR3(0, 1, 1));	// 上ベクトル
}

StageEdit::~StageEdit()
{
}

void StageEdit::Update()
{
	GetWorldPos();
	//obj->SetPosition(VECTOR3(0, 0, 0));
	obj->SetPosition(VECTOR3(worldPos.x,0,worldPos.z));

	ImGui::Begin("WORLDPOS");
	ImGui::InputFloat("X", &worldPos.x);
	ImGui::InputFloat("Y", &worldPos.y);
	ImGui::InputFloat("Z", &worldPos.z);
	ImGui::End();
	ImGui::Begin("MOUSEPOS");
	ImGui::InputFloat("X", &mPos.x);
	ImGui::InputFloat("Y", &mPos.y);
	//ImGui::InputFloat("Z", &worldPos.z);
	ImGui::End();
}

void StageEdit::Draw()
{
}

VECTOR3 StageEdit::GetWorldPos()
{
	MATRIX4X4 mView = GameDevice()->m_mView;
	MATRIX4X4 mInv = XMMatrixInverse(nullptr, mView);
	// ↑Viewの逆行列
	// 
	// マウス座標取得　原点はウィンドウの左上
	POINT mousePos;
	GetCursorPos(&mousePos);

	ScreenToClient(GameDevice()->m_pMain->m_hWnd, &mousePos);
	
	// カメラからマウス座標にRayを飛ばしてLineToMeshで衝突したオブジェクトを Object* obj に入れる
	// マウスを動かしたら追従させる
	
	return worldPos;
}
