#pragma once
// Player.h
#include "Object3D.h"
#include "Animator.h"

class Player : public Object3D {
public:
	Player();
	~Player();
	void Update() override;
	void Draw() override;

	SphereCollider Collider() override;

	struct Sphere
	{
		VECTOR3 velocity = VECTOR3(0, 0, 0);
		VECTOR3 center = VECTOR3(0, 0, 0);
		float radius = 0.0f;
	};
	Sphere sphere;

private:
	float speedY; // Y�̑��x

	Animator* animator; // ���i�̃C���X�^���X���|�C���^�[�^�ŏ���
	enum AnimID {
		aIdle = 0,
		aRun = 1,
		aAttack1,
		aAttack2,
		aAttack3,
	};

	enum State {
		sOnGround = 0,
		sJump,
		sAttack,
	};
	State state;
	void UpdateOnGround();
	void UpdateJump();
	void UpdateAttack();
	VECTOR3 move;
	int frame; // �A�j���[�V�����̃t���[���𐔂���

	
};