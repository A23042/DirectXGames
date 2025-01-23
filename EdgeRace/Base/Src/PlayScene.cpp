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
	// Œõ‚Ì•ûŒü‚ð‚È‚­‚·‚±‚Æ‚Å‰e‚ª‚È‚­‚È‚é
	//GameDevice()->m_vLightDir = VECTOR3();
	Instantiate<Camera>();
	sc = ObjectManager::FindGameObject<Score>();
	if (sc == nullptr) 
	{
		sc = SingleInstantiate<Score>();
		sc->DontDestroyMe(); // ƒV[ƒ“‚ªØ‚è‘Ö‚í‚Á‚Ä‚àÁ‚¦‚È‚¢
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
	loadStage->Load(2);
	//SAFE_DELETE(csv);
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	// ‰æ–Ê•ªŠ„‚ÌØ‚è‘Ö‚¦•û “r’†‚Å‚à“®“I‚ÉØ‚è‘Ö‚¦‰Â”\
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
			ss->SetMultiSizePlay();
			ss->SetMultiScreen();
		}
	}
#endif

	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_T))
	{
		//ss->SetSingleScreen();
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
	int finishFlag = 0;
	std::list<Player*> pls = ObjectManager::GetObjctList<Player>();
	for (Player* pl : pls)
	{
		if (pl->GetRestShot() <= 0)
		{
			finishFlag++;
		}
	}
	if(finishFlag == 2)
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


