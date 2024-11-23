#include <fstream>
#include <assert.h>
#include "PlayScene.h"
#include "Player.h"
#include "CsvReader.h"
#include "Camera.h"
#include "Score.h"
#include "ScoreDraw.h"
#include "Box.h"
#include "Ball.h"
#include "MoveBox.h"
#include "ScoreArea.h"
#include "FallCheck.h"
#include "Line.h"

PlayScene::PlayScene(int num)
{
	// テキストファイルの読み方
	char name[64];
	sprintf_s<64>(name, "data/Stage%02d.csv", num);
#if 0
	CsvReader* csv = new CsvReader("Data/map00.csv");
#else
	CsvReader* csv = new CsvReader(name);
#endif

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
				float e = csv->GetFloat(i, 5);
				float f = csv->GetFloat(i, 6);
				float mass = csv->GetFloat(i, 7);
				int num = csv->GetFloat(i, 8);
				obj = new Player(num);
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
			else if (str == "MBox") 
			{
				VECTOR3 size = VECTOR3(csv->GetFloat(i, 5), csv->GetFloat(i, 6), csv->GetFloat(i, 7));
				VECTOR3 rot = VECTOR3(csv->GetFloat(i, 8), csv->GetFloat(i, 9), csv->GetFloat(i, 10));
				VECTOR3 move = VECTOR3(csv->GetFloat(i, 11), csv->GetFloat(i, 12), csv->GetFloat(i, 13));
				VECTOR3 moveSpeed = VECTOR3(csv->GetFloat(i, 14), csv->GetFloat(i, 15), csv->GetFloat(i, 16));
				float e = csv->GetFloat(i, 17);
				float f = csv->GetFloat(i, 18);
				obj = new MoveBox(size, rot, move, moveSpeed);	// 直方体のサイズと回転量、移動量を渡す
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
			else if (str == "Area1")
			{
				VECTOR3 size = VECTOR3(csv->GetFloat(i, 5), csv->GetFloat(i, 6), csv->GetFloat(i, 7));
				VECTOR3 rot = VECTOR3(csv->GetFloat(i, 8), csv->GetFloat(i, 9), csv->GetFloat(i, 10));
				obj = new ScoreArea1(size, rot);
			}
			else if (str == "Area2")
			{
				VECTOR3 size = VECTOR3(csv->GetFloat(i, 5), csv->GetFloat(i, 6), csv->GetFloat(i, 7));
				VECTOR3 rot = VECTOR3(csv->GetFloat(i, 8), csv->GetFloat(i, 9), csv->GetFloat(i, 10));
				obj = new ScoreArea2(size, rot);
			}
			else if (str == "Area3")
			{
				VECTOR3 size = VECTOR3(csv->GetFloat(i, 5), csv->GetFloat(i, 6), csv->GetFloat(i, 7));
				VECTOR3 rot = VECTOR3(csv->GetFloat(i, 8), csv->GetFloat(i, 9), csv->GetFloat(i, 10));
				obj = new ScoreArea3(size, rot);
			}
			else if (str == "FallCheck")
			{
				obj = new FallCheck(true);
			}
			else if (str == "Line")
			{
				//float x = csv->GetFloat(i, 2);
				//float y = csv->GetFloat(i, 3);
				//float z = csv->GetFloat(i, 4);
				VECTOR3 size = VECTOR3(csv->GetFloat(i, 5), csv->GetFloat(i, 6), csv->GetFloat(i, 7));
				obj = new Line();
				obj->SetScale(size);
				//obj->pObj.center = VECTOR3(x, y, z);
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
	Score* sc = ObjectManager::FindGameObject<Score>();
	if (sc == nullptr) 
	{
		sc = Instantiate<Score>();
		sc->DontDestroyMe(); // シーンが切り替わっても消えない
	}
	sc->Clear();
	Instantiate<ScoreDraw>();
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_R)) 
	{
		SceneManager::ChangeScene("ResultScene");
	}
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_T)) 
	{
		SceneManager::ChangeScene("TitleScene");
	}
	
}

void PlayScene::Draw()
{
	
}
