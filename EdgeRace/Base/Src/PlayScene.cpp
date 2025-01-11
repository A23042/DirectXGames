#include <fstream>
#include <assert.h>
#include "PlayScene.h"
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
#include "SkyBox.h"

PlayScene::PlayScene(int num)
{
	// 光の方向をなくすことで影がなくなる
	//GameDevice()->m_vLightDir = VECTOR3();

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
				float rotY = csv->GetFloat(i, 5);
				float e = csv->GetFloat(i, 6);
				float f = csv->GetFloat(i, 7);
				float mass = csv->GetFloat(i, 8);
				int num = csv->GetFloat(i, 9);
				player[num] = new Player(num);
				obj = player[num];
				obj->SetRotation(VECTOR3(0, rotY, 0));
				obj->pObj.e = e;
				obj->pObj.f = f;
				obj->pObj.mass = mass;
			}
			else if (str == "BOX") 
			{
				VECTOR3 size = VECTOR3(csv->GetVector3(i, 5));
				VECTOR3 rot = VECTOR3(csv->GetVector3(i, 8));
				float e = csv->GetFloat(i, 11);
				float f = csv->GetFloat(i, 12);
				obj = new Box(size, rot);	// 直方体のサイズと回転量を渡す
				obj->pObj.e = e;
				obj->pObj.f = f;
			}
			else if (str == "MBox") 
			{
				VECTOR3 size = VECTOR3(csv->GetVector3(i, 5));
				VECTOR3 rot = VECTOR3(csv->GetVector3(i, 8));
				VECTOR3 move = VECTOR3(csv->GetVector3(i, 11));
				VECTOR3 moveSpeed = VECTOR3(csv->GetVector3(i, 14));
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
				VECTOR3 size = VECTOR3(csv->GetVector3(i, 5));
				VECTOR3 rot = VECTOR3(csv->GetVector3(i, 8));
				obj = new ScoreArea1(size, rot);
			}
			else if (str == "Area2")
			{
				VECTOR3 size = VECTOR3(csv->GetVector3(i, 5));
				VECTOR3 rot = VECTOR3(csv->GetVector3(i, 8));
				obj = new ScoreArea2(size, rot);
			}
			else if (str == "Area3")
			{
				VECTOR3 size = VECTOR3(csv->GetVector3(i, 5));
				VECTOR3 rot = VECTOR3(csv->GetVector3(i, 8));
				obj = new ScoreArea3(size, rot);
			}
			else if (str == "FallCheck")
			{
				obj = new FallCheck(true);
			}
			else if (str == "Line")
			{
				obj = new Line(true);
				obj->SetScale(csv->GetVector3(i, 5));
			}
			else 
			{
				assert(false);
			}
			obj->SetPosition(csv->GetVector3(i, 2));
		}
	}
	Instantiate<Camera>();
	sc = ObjectManager::FindGameObject<Score>();
	if (sc == nullptr) 
	{
		sc = SingleInstantiate<Score>();
		sc->DontDestroyMe(); // シーンが切り替わっても消えない
	}
	sc->Clear();
	Instantiate<ScoreDraw>();
	Instantiate<CollisonManager>();
	//Instantiate<SkyBox>();
	loadStage = Instantiate<LoadStage>();
	resultPanel = SingleInstantiate<SplitScreenLastDraw>();
	resultPanel->DontDestroyMe();
	data = SingleInstantiate<DataHolder>();
	data->DontDestroyMe();
	data->SetPlay(true);

	SAFE_DELETE(csv);
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	// 画面分割の切り替え方 途中でも動的に切り替え可能
#if 1
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_L))
	{
		SplitScreen* ss = ObjectManager::FindGameObject<SplitScreen>();
		if (ss->Multi())
		{
			ss->SetSingleScreen();
		}
		else
		{
			ss->SetMultiScreen();
		}
	}
#endif

	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_T))
	{
		SceneManager::ChangeScene("TitleScene");
	}
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_M))
	{
		int num = Random(1, 7);
		loadStage->Load(num);
	}

	switch (playState)
	{
	case sPlay:
		UpdatePlay();
		break;
	case sResult:
		UpdateResult();
		break;
	default:
		break;
	}
}

void PlayScene::Draw()
{
}

void PlayScene::UpdatePlay()
{
	if(!player[0]->GetRestShot() && !player[1]->GetRestShot())
	{
		data->SetPlay(false);
		playState = sResult;
	}
}

void PlayScene::UpdateResult()
{
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_R))
	{
		int num = Random(1, 7);
		loadStage->Load(num);
		data->SetPlay(true);
		playState = sPlay;
	}
}


