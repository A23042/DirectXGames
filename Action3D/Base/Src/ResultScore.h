#pragma once
#include "GameObject.h"
#include "Score.h"

class ResultScore : public GameObject {
public:
	ResultScore();
	~ResultScore();
	void Update() override;
	void Draw() override;
private:
	Score* sc = nullptr;
	// �\������X�R�A
	int viewP0Score; 
	int viewP1Score;
	float timer;
};