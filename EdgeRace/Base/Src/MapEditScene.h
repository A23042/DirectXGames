#pragma once
#include "SceneBase.h"

class SplitScreen;
class MapEditScene : public SceneBase
{
public:
	MapEditScene();
	~MapEditScene();
	void Update() override;
	void Draw() override;
private:
	CSprite* cs;
	SplitScreen* ss;
};
