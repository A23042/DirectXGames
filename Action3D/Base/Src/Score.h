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

#if 0
	void AddScore(int val) {
		score += val;
	}
	int GetScore() {
		return score;
	}
#endif
private:
	int p0Score; // スコア
	int p1Score; // スコア
};