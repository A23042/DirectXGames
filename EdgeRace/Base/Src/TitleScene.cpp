#include "TitleScene.h"
#include "GameMain.h"
#include "CsvReader.h"
#include "MenuPanel.h"
#include "Box.h"
#include "Ball.h"
#include "Player.h"
#include "Camera.h"
#include "FallCheck.h"
#include "CollisionManager.h"

TitleScene::TitleScene()
{
	positionX = 0;
	speedX = 1;
	panel = new MenuPanel();
	stageNum = 1;
}

TitleScene::~TitleScene()
{
}

void TitleScene::Update()
{
	positionX += speedX;
	if (positionX >= 60) {
		speedX *= -1;
	}
	else {
		if (positionX <= 0) {
			speedX *= -1;
		}
	}
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_P)) {
		SceneManager::ChangeScene("PlayScene", stageNum);
	}
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_E)) {
		SceneManager::ChangeScene("MapEditScene");
	}

	if (GameDevice()->m_pDI->CheckJoy(KD_UTRG, DIJ_B) || GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_SPACE))
	{
		switch (panel->GetNum())
		{
		case 0:
			SceneManager::ChangeScene("PlayScene", stageNum);
			break;
		case 1:
			exit(EXIT_SUCCESS);	// ƒAƒvƒŠI—¹
			break;
		case 2:
			SceneManager::ChangeScene("MapEditScene");
			break;
		default:
			break;
		}
	}

	/*ImGui::Begin("STAGENUM");
	ImGui::InputInt("Number", &stageNum);
	ImGui::End();*/
}

void TitleScene::Draw()
{
	GameDevice()->m_pFont->Draw(positionX, 20, "TitleScene", 16, RGB(255, 0, 0));
}