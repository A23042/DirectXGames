#pragma once
#include "SceneBase.h"
#include "LoadStage.h"
#include "SplitScreenLastDraw.h"
#include "Player.h"

class PlayScene : public SceneBase
{
public:
	PlayScene(int num = 1);
	~PlayScene();
	void Update() override;
	void Draw() override;
	
	void UpdatePlay();
	void UpdateResult();

	void SetPlay(bool isPlay) { this->isPlay = isPlay; }

	bool IsPlay() { return isPlay; }
private:
	enum PlayState
	{
		sPlay,	// ƒvƒŒƒC’†
		sResult,// Œ‹‰Ê•\Ž¦
	}playState;

	LoadStage* loadStage = nullptr;
	SplitScreenLastDraw* resultPanel = nullptr;
	Player* player[2] = {};

	bool isPlay = true;

};
