#include "MapEditScene.h"
#include "StageEdit.h"
#include "Gizmo3D.h"
#include "EditorCamera.h"
#include "EditUI.h"
#include "SplitScreen.h"
#include "MainCamera.h"
#include "SubCamera.h"
#include "Camera.h"

MapEditScene::MapEditScene()
{
	new StageEdit();
	new EditUI();
	cs = new CSprite;
	new MainCamera();
	//new EditorCamera();
	new Camera(true);
}

MapEditScene::~MapEditScene()
{
	SAFE_DELETE(cs);
}

void MapEditScene::Update()
{
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_T)) 
	{
		SceneManager::ChangeScene("TitleScene");
	}
	// 画面分割の切り替え方 途中でも動的に切り替え可能
#if 1
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_L))
	{
		ss = ObjectManager::FindGameObject<SplitScreen>();
		if (ss->Multi())
		{
			ss->SetSingleScreen();
		}
		else
		{
			ss->SetMultiSizeEditor();
			ss->SetMultiScreen();
		}
	}
#endif

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
	//GameDevice()->m_pFont->Draw(400, 15, _T("EDIT SCENE"), 16, RGB(255, 0, 0));
}
