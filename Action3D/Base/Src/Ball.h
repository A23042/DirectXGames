#pragma once
// S.Matsunaga

#include "Object3D.h"
//#include "ScoreArea.h"

class Ball : public Object3D {
public:
	Ball(bool isPhysic = true, int pNum = -1);
	~Ball();
	void Update() override;
	void Start() override;
private:
	Object3D* child = nullptr;
	bool isPhysic = true;
};