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

	// Boxと衝突したときに呼ばれる反射関数
	void PushVec(VECTOR3 pushVec, VECTOR3 RefVec);

	// Player番号取得
	int GetPlNum() { return playerNum; }
private:
	enum State {
		sNormal = 0,
		sJump,
	}state;

	void UpdateNormal();
	void UpdateJump();

	// Mapeditorで衝突判定などしないためのBool
	bool isPhysic;

	VECTOR3 sumVelocity;

	int playerNum;
};