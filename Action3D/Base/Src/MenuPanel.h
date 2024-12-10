#pragma once
// 2024.12.10 S.Matsunaga タイトルシーンのパネル

#include "Object3D.h"

class MenePanel : public Object3D
{
public:
	MenePanel();
	~MenePanel();
	void Update() override;
	void Draw() override;
private:
	CSprite* BackGround = nullptr;
	CSpriteImage* BackGroundImage = nullptr;

	CSprite* Button[5] = {};

	float moveTime = 0.2;
	float time = 0;

	int selectNum = 0;
};