#include "TitleScene.h"
#include "GameMain.h"
#include "MenuPanel.h"

TitleScene::TitleScene()
{
	positionX = 0;
	speedX = 1;
	new MenePanel();
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

	//if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_R)) {
	//	SceneManager::ChangeScene("ResultScene");
	//}


	ImGui::Begin("STAGENUM");
	ImGui::InputInt("Number", &stageNum);
	ImGui::End();
}

void TitleScene::Draw()
{
	GameDevice()->m_pFont->Draw(
		positionX, 20, "TitleScene", 16, RGB(255, 0, 0));
}