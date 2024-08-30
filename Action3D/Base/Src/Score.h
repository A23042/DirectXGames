#pragma once
#include "GameObject.h"

class Score : public GameObject {
public:
	Score();
	~Score();
	void Update() override;
	void Draw() override;
	void Clear() {
		score = 0;
	}
	void AddScore(int val) {
		score += val;
	}
	int GetScore() {
		return score;
	}
private:
	int score; // ÉXÉRÉA
};