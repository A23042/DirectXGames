#include <fstream>
#include <assert.h>

#include "LoadStage.h"
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

LoadStage::LoadStage()
{
	ObjectManager::SetVisible(this, false);
}

LoadStage::~LoadStage()
{
}

void LoadStage::Start()
{
}

void LoadStage::Update()
{
}

void LoadStage::Load(int num)
{
	// 現在配置されているオブジェクトをリセット
	std::list<Object3D*> objs = ObjectManager::FindGameObjects<Object3D>();
	for (Object3D* obj : objs)
	{
		if (obj->editObj.name == "Box"
			|| obj->editObj.name == "MoveBox"
			|| obj->editObj.name == "Ball"
			|| obj->editObj.name == "scoreArea1"
			|| obj->editObj.name == "scoreArea2"
			|| obj->editObj.name == "scoreArea3"
			)
		{
			obj->DestroyMe();
		}
	}

	// テキストファイルの読み方
	char name[64];
	// TestMapを使用する場合は0にする
#if 1
	sprintf_s<64>(name, "data/StageCsv/Stage%02d.csv", num);
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
				float x = csv->GetFloat(i, 2);
				float y = csv->GetFloat(i, 3);
				float z = csv->GetFloat(i, 4);
				float rotY = csv->GetFloat(i, 5);
				float e = csv->GetFloat(i, 6);
				float f = csv->GetFloat(i, 7);
				float mass = csv->GetFloat(i, 8);
				int num = csv->GetFloat(i, 9);
				// Playerは再度生成せずもともとあるPlayerを使う
				std::list<Player*> pls = ObjectManager::FindGameObjects<Player>();
				for (Player* pl : pls)
				{
					if (pl->GetPlNum() == num)
					{
						obj = pl;
						obj->SetPosition(x, y, z);
						obj->pObj.center = VECTOR3(x, y, z);
						obj->SetRotation(VECTOR3(0, rotY, 0));
						obj->pObj.e = e;
						obj->pObj.f = f;
						obj->pObj.mass = mass;
						pl->Reset();
						continue;
					}
				}
				//obj = new Player(num);
			}
			else if (str == "BOX")
			{
				Box* box = nullptr;
				VECTOR3 size = VECTOR3(csv->GetFloat(i, 5), csv->GetFloat(i, 6), csv->GetFloat(i, 7));
				VECTOR3 rot = VECTOR3(csv->GetFloat(i, 8), csv->GetFloat(i, 9), csv->GetFloat(i, 10));
				float e = csv->GetFloat(i, 11);
				float f = csv->GetFloat(i, 12);
				box = new Box(size, rot);	// 直方体のサイズと回転量を渡す
				//collManager->AddBox(box);
				box->pObj.e = e;
				box->pObj.f = f;
				obj = box;
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
				Ball* ball = nullptr;
				float e = csv->GetFloat(i, 5);
				float f = csv->GetFloat(i, 6);
				float mass = csv->GetFloat(i, 7);
				ball = new Ball();
				//collManager->AddBall(ball);
				ball->pObj.e = e;
				ball->pObj.f = f;
				ball->pObj.mass = mass;
				obj = ball;
			}
			else if (str == "Area1")
			{
				VECTOR3 size = VECTOR3(csv->GetFloat(i, 5), csv->GetFloat(i, 6), csv->GetFloat(i, 7));
				VECTOR3 rot = VECTOR3(csv->GetFloat(i, 8), csv->GetFloat(i, 9), csv->GetFloat(i, 10));
				ScoreArea* area = new ScoreArea1(size, rot);
				//collManager->AddArea(area);
				obj = area;
			}
			else if (str == "Area2")
			{
				VECTOR3 size = VECTOR3(csv->GetFloat(i, 5), csv->GetFloat(i, 6), csv->GetFloat(i, 7));
				VECTOR3 rot = VECTOR3(csv->GetFloat(i, 8), csv->GetFloat(i, 9), csv->GetFloat(i, 10));
				ScoreArea* area = new ScoreArea2(size, rot);
				//collManager->AddArea(area);
				obj = area;
			}
			else if (str == "Area3")
			{
				VECTOR3 size = VECTOR3(csv->GetFloat(i, 5), csv->GetFloat(i, 6), csv->GetFloat(i, 7));
				VECTOR3 rot = VECTOR3(csv->GetFloat(i, 8), csv->GetFloat(i, 9), csv->GetFloat(i, 10));
				ScoreArea* area = new ScoreArea3(size, rot);
				//collManager->AddArea(area);
				obj = area;
			}
			else if (str == "FallCheck")
			{
				//obj = new FallCheck(true);
				FallCheck* fallCheck = ObjectManager::FindGameObject<FallCheck>();
				if (fallCheck == nullptr)
				{
					fallCheck = new FallCheck(true);
				}
				obj = fallCheck;
			}
			else if (str == "Line")
			{
				Line* line = ObjectManager::FindGameObject<Line>();
				if (line == nullptr)
				{
					line = new Line(true);
				}
				obj = line;
				VECTOR3 size = VECTOR3(csv->GetFloat(i, 5), csv->GetFloat(i, 6), csv->GetFloat(i, 7));
				obj->SetScale(size);
			}
			else
			{
				assert(false);
			}
			float x = csv->GetFloat(i, 2);
			float y = csv->GetFloat(i, 3);
			float z = csv->GetFloat(i, 4);
			if(obj != nullptr)
			{
				obj->SetPosition(x, y, z);
				obj->pObj.center = VECTOR3(x, y, z);
			}
		}
	}
	SAFE_DELETE(csv);
}