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
	
	// テキストファイルの読み方
	char name[64];
	// TestMapを使用する場合は0にする
#if 1
	sprintf_s<64>(name, "data/StageCsv/Stage%02d.csv", -1);
#else
	sprintf_s<64>(name, "data/TestStageCsv/testStage%02d.csv", num);
#endif
	CsvReader* csv = new CsvReader(name);

	assert(csv->GetLines() > 0);
	for (int i = 0; i < csv->GetLines(); i++)
	{	// １行ずつ読む
		std::string str = csv->GetString(i, 0); // 先頭の数字を取る
		Object3D* obj = nullptr;
		// 先頭が「0」の場合はスキップ
		if (str == "0")
		{
			continue;
		}
		// 「１」の場合だけ進む
		else if (str == "1")
		{
			str = csv->GetString(i, 1);
			if (str == "PLAYER")
			{
				float rotY = csv->GetFloat(i, 5);
				float e = csv->GetFloat(i, 6);
				float f = csv->GetFloat(i, 7);
				float mass = csv->GetFloat(i, 8);
				int num = csv->GetFloat(i, 9);
				player[num] = new Player(num,false);
				obj = player[num];
				obj->SetRotation(VECTOR3(0, rotY, 0));
				obj->pObj.e = e;
				obj->pObj.f = f;
				obj->pObj.mass = mass;
			}
			else if (str == "BOX")
			{
				VECTOR3 size = VECTOR3(csv->GetFloat(i, 5), csv->GetFloat(i, 6), csv->GetFloat(i, 7));
				VECTOR3 rot = VECTOR3(csv->GetFloat(i, 8), csv->GetFloat(i, 9), csv->GetFloat(i, 10));
				float e = csv->GetFloat(i, 11);
				float f = csv->GetFloat(i, 12);
				obj = new Box(size, rot);	// 直方体のサイズと回転量を渡す
				obj->pObj.e = e;
				obj->pObj.f = f;
			}
			else if (str == "BALL")
			{
				float e = csv->GetFloat(i, 5);
				float f = csv->GetFloat(i, 6);
				float mass = csv->GetFloat(i, 7);
				obj = new Ball();
				obj->pObj.e = e;
				obj->pObj.f = f;
				obj->pObj.mass = mass;
			}
			else if (str == "FallCheck")
			{
				obj = new FallCheck(true);
			}
			else
			{
				assert(false);
			}
			float x = csv->GetFloat(i, 2);
			float y = csv->GetFloat(i, 3);
			float z = csv->GetFloat(i, 4);
			obj->SetPosition(x, y, z);
		}
	}
	Instantiate<Camera>();
	Instantiate<CollisonManager>();
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

	if (GameDevice()->m_pDI->CheckJoy(KD_UTRG, DIJ_B))
	{
		switch (panel->GetNum())
		{
		case 0:
			SceneManager::ChangeScene("PlayScene", stageNum);
			break;
		case 1:
			exit(EXIT_SUCCESS);
			break;
		case 2:
			SceneManager::ChangeScene("MapEditScene");
			break;
		default:
			break;
		}
	}

	ImGui::Begin("STAGENUM");
	ImGui::InputInt("Number", &stageNum);
	ImGui::End();
}

void TitleScene::Draw()
{
	GameDevice()->m_pFont->Draw(
		positionX, 20, "TitleScene", 16, RGB(255, 0, 0));
}