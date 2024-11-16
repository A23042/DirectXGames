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

	// Box�ƏՓ˂����Ƃ��ɌĂ΂�锽�ˊ֐�
	void PushVec(VECTOR3 pushVec, VECTOR3 RefVec);

	// Player�ԍ��擾
	int GetPlNum() { return playerNum; }
private:
	enum State {
		sNormal = 0,
		sJump,
	}state;

	void UpdateNormal();
	void UpdateJump();

	// Mapeditor�ŏՓ˔���Ȃǂ��Ȃ����߂�Bool
	bool isPhysic;

	VECTOR3 sumVelocity;

	int playerNum;
};