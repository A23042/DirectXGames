#pragma once
#include "Box.h"

class Line : public Box
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
