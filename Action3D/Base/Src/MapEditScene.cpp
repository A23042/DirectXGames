#include "MapEditScene.h"
#include "StageEdit.h"
#include "Gizmo3D.h"
#include "EditorCamera.h"

MapEditScene::MapEditScene()
{
	new StageEdit();
	new EditorCamera();
	cs = new CSprite;
}

MapEditScene::~MapEditScene()
{
}

void MapEditScene::Update()
{
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_T)) 
	{
		SceneManager::ChangeScene("TitleScene");
	}
}

void MapEditScene::Draw()
{
	// グリッド線表示
	for (int i = 0; i < 50; i++)
	{
		cs->DrawLine3D(VECTOR3(-100, 0, 2 * i), VECTOR3(100, 0, 2 * i), RGB(255, 255, 255), 0.5f);
		cs->DrawLine3D(VECTOR3(-100, 0, -2 * i), VECTOR3(100, 0, -2 * i), RGB(255, 255, 255), 0.5f);
		cs->DrawLine3D(VECTOR3(2 * i, 0, 100), VECTOR3(2 * i, 0, -100), RGB(255, 255, 255), 0.5f);
		cs->DrawLine3D(VECTOR3(-2 * i, 0, 100), VECTOR3(-2 * i, 0, -100), RGB(255, 255, 255), 0.5f);
	}

	GameDevice()->m_pFont->Draw(400, 15, _T("EDIT SCENE"), 16, RGB(255, 0, 0));
}
