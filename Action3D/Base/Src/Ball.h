#pragma once
#include "Object3D.h"

class Ball : public Object3D {
public:
	Ball();
	~Ball();
	void Update() override;
	void PushVec(VECTOR3 pushVec);
};