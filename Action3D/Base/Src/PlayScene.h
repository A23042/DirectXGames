#pragma once
#include "SceneBase.h"
#include "LoadStage.h"
#include "SplitScreenLastDraw.h"
#include "Player.h"
#include "DataHolder.h"

class PlayScene : public SceneBase
{
public:
	PlayScene(int num = 1);
	~PlayScene();
	void Update() override;
	void Draw() override;
	
	void UpdatePlay();
	void UpdateResult();
private:
	enum PlayState
	{
		sPlay,	// ƒvƒŒƒC’†
		sResult,// Œ‹‰Ê•\Ž¦
	}playState;

	LoadStage* loadStage = nullptr;
	SplitScreenLastDraw* resultPanel = nullptr;
	Player* player[2] = {};
	Score* sc = nullptr;
	DataHolder* data = nullptr;
};
