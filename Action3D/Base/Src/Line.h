#pragma once
#include "Box.h"

class Line : public Box
{
public:
	Line(bool playScene = false);
	~Line();

	// Box��Update()��Start()����
	//void Update() override;
	//void Start() override;
private:
	bool isPlayScene;
};
