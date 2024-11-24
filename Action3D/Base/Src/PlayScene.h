#pragma once
#include "SceneBase.h"
#include "CollisionManager.h"

class PlayScene : public SceneBase
{
public:
	PlayScene(int num = 1);
	~PlayScene();
	void Update() override;
	void Draw() override;
	void Load(int num);
private:
	CollisonManager* collManager = nullptr;
};
