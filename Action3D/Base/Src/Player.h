#pragma once
#include "Object3D.h"
#include "Animator.h"
#include <Xinput.h>

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
		sWait,
		sJump,
	}state;

	void UpdateNormal();
	void UpdateWait();
	void UpdateJump();

	// Mapeditorで衝突判定などしないためのBool
	bool isPhysic;

	Object3D* myBall = nullptr;

	VECTOR3 sumVelocity;
	VECTOR3 StartPos;

	int playerNum;
	int score = 0;
	float pushTime[2] = {};

	XINPUT_STATE m_state;
	XINPUT_KEYSTROKE m_keystroke;

};