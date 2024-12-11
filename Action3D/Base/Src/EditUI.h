#pragma once

#include "Object3D.h"

class EditUI : public Object3D
{
public:
	EditUI();
	~EditUI();
	void Draw() override;
private:
	CSprite* spr = nullptr;
	std::list<CSpriteImage*> sprImages;
};