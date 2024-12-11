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
private: // �N���X�̃����o�[�ϐ���private�ɂ���
	int positionX;
	int speedX;
	int stageNum = 1;

	MenuPanel* panel = nullptr;
	Player* player[2];
};
