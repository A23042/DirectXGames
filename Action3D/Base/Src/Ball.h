#pragma once
#include "Object3D.h"

class Ball : public Object3D {
public:
	Ball(bool isPhysic = true);
	~Ball();
	void Update() override;
	void Start() override;
	void PushVec(VECTOR3 pushVec, VECTOR3 RefVec);
private:
	bool isPhysic;
};