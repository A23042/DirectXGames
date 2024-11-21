#pragma once
#include "GameObject.h"
#include "Player.h"
#include "Ball.h"

class Score : public GameObject {
public:
	Score();
	~Score();
	void Update() override;
	void Draw() override;
	void Clear() {
		p0Score = 0;
		p1Score = 0;
	}

#if 1
	//void AddScore(int val) {
	//	score += val;
	//}
	int GetP0Score() {
		return p0Score;
	}
	int GetP1Score() {
		return p1Score;
	}
#endif
private:
	int p0Score; // P0スコア
	int p1Score; // P1スコア
};