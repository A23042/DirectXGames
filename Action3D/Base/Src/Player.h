#pragma once
#include "Object3D.h"
#include "Animator.h"

class Player : public Object3D {
public:
	Player(int num, bool isPhysic = true);
	~Player();
	void Start() override;
	void Update() override;
	void Draw() override;

	SphereCollider Collider() override;

	void PushVec(VECTOR3 pushVec, VECTOR3 RefVec);

	int GetPlNum() { return playerNum; }
private:
	enum State {
		sNormal = 0,
		sJump,
	}state;

	void UpdateNormal();
	void UpdateJump();

	bool isPhysic;

	VECTOR3 sumVelocity;

	int playerNum;
};