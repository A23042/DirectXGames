#include "MapEditScene.h"
#include "StageEdit.h"

MapEditScene::MapEditScene()
{
	new StageEdit();
}

MapEditScene::~MapEditScene()
{
}

void MapEditScene::Update()
{
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_T)) {
		SceneManager::ChangeScene("TitleScene");
	}
}

void MapEditScene::Draw()
{
	// 仮グリッド線表示
	CSprite cs;
	for (int i = 0; i < 20; i++)
	{
		cs.DrawLine3D(VECTOR3(-100, 0, 2 * i), VECTOR3(100, 0, 2 * i), RGB(255, 0, 0), 0.8f);
		cs.DrawLine3D(VECTOR3(-100, 0, -2 * i), VECTOR3(100, 0, -2 * i), RGB(255, 0, 0), 0.8f);
		cs.DrawLine3D(VECTOR3(2 * i, 0, 100), VECTOR3(2 * i, 0, -100), RGB(255, 0, 0), 0.8f);
		cs.DrawLine3D(VECTOR3(-2 * i, 0, 100), VECTOR3(-2 * i, 0, -100), RGB(255, 0, 0), 0.8f);
	}
	cs.DrawLine3D(VECTOR3(-100, 0, 0), VECTOR3(100, 0, 0), RGB(255, 0, 0), 0.8f);
	GameDevice()->m_pFont->Draw(400, 15, _T("EDIT SCENE"), 16, RGB(255, 0, 0));
}
