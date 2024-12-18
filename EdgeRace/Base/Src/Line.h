#pragma once
#include "BoxCollisionBase.h"

class Line : public BoxCollisionBase
{
public:
	Line(bool playScene = false);
	~Line();

	// Box‚ÌUpdate()‚ÆStart()‚ð‰ñ‚·
	//void Update() override;
	//void Start() override;
private:
	bool isPlayScene;
};
