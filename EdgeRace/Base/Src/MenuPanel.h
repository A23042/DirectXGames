#pragma once
// 2024.12.10 S.Matsunaga タイトルシーンのパネル

#include "Object3D.h"

class MenuPanel : public Object3D
{
public:
	MenuPanel();
	~MenuPanel();
	void Update() override;
	void Draw() override;

	int GetNum() { return selectNum; }
private:
	CSprite* spr = nullptr;
	CSpriteImage* BackGroundImage = nullptr;
	CSpriteImage* TextImage[3] = {};

	CSprite* Button[3] = {};

	float moveTime = 0.2;
	float time = 0;

	int selectNum = 0;
};