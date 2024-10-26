#pragma once
// Player.h
#include "Object3D.h"
#include "Animator.h"

class Player : public Object3D {
public:
	Player(int num);
	~Player();
	void Start() override;
	void Update() override;
	void Draw() override;

	SphereCollider Collider() override;
	
	void PushVec(VECTOR3 pushVec, VECTOR3 RefVec);

	int playerNum;
private:
	//float speedY; // Yの速度

	Animator* animator; // 部品のインスタンスをポインター型で準備
	enum AnimID {
		aIdle = 0,
		aRun = 1,
		aAttack1,
		aAttack2,
		aAttack3,
	};

	enum State {
		sNormal = 0,
		sJump,
		sAttack,
	};
	State state;
	void UpdateNormal();
	void UpdateJump();
	void UpdateAttack();
	//int frame; // アニメーションのフレームを数える

	VECTOR3 sumVelocity;

};