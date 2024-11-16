#pragma once
#include "SceneBase.h"

class PlayScene : public SceneBase
{
public:
	PlayScene(int num = 1);
	~PlayScene();
	void Update() override;
	void Draw() override;
};
