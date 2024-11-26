#pragma once
#include "BoxCollisionBase.h"

class Line : public BoxCollisionBase
{
public:
	Line(bool playScene = false);
	~Line();

	// BoxのUpdate()とStart()を回す
	//void Update() override;
	//void Start() override;
private:
	bool isPlayScene;
};
