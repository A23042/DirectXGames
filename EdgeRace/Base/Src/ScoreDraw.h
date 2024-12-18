#pragma once
#include "GameObject.h"

class ScoreDraw : public GameObject {
public:
	ScoreDraw();
	~ScoreDraw();
	void Update() override;
	void Draw() override;
};