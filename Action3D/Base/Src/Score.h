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
		p0ScoreTemp = 0;
		p1ScoreTemp = 0;
	}

	void CountScore();

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
	int p0ScoreTemp; // P0�X�R�A
	int p1ScoreTemp; // P1�X�R�A

	int p0Score; // P0�X�R�A
	int p1Score; // P1�X�R�A
};