#pragma once
#include "SceneBase.h"

class MapEditScene : public SceneBase
{
public:
	MapEditScene();
	~MapEditScene();
	void Update() override;
	void Draw() override;
};
