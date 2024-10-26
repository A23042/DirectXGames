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
	//float speedY; // Y�̑��x

	Animator* animator; // ���i�̃C���X�^���X���|�C���^�[�^�ŏ���
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
	//int frame; // �A�j���[�V�����̃t���[���𐔂���

	VECTOR3 sumVelocity;

};