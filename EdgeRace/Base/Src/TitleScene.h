#pragma once
#include "SceneBase.h"

class Player;
class MenuPanel;

class TitleScene : public SceneBase
{
public:
	TitleScene();
	~TitleScene();
	void Update() override;
	void Draw() override;
private: // クラスのメンバー変数はprivateにする
	int positionX;
	int speedX;
	int stageNum = 1;

	MenuPanel* panel = nullptr;
	Player* player[2];
};
