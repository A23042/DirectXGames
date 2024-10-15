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
	GameDevice()->m_pFont->Draw(400, 15, _T("EDIT SCENE"), 16, RGB(255, 0, 0));
}
