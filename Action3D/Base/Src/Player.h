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

	// Box‚ÆÕ“Ë‚µ‚½‚Æ‚«‚ÉŒÄ‚Î‚ê‚é”½ËŠÖ”
	void PushVec(VECTOR3 pushVec, VECTOR3 RefVec);

	// Player”Ô†æ“¾
	int GetPlNum() { return playerNum; }
private:
	enum State {
		sNormal = 0,
		sJump,
	}state;

	void UpdateNormal();
	void UpdateJump();

	// Mapeditor‚ÅÕ“Ë”»’è‚È‚Ç‚µ‚È‚¢‚½‚ß‚ÌBool
	bool isPhysic;

	VECTOR3 sumVelocity;

	int playerNum;
	int score = 0;
	float pushTime[2] = {};

	XINPUT_STATE m_state;
	XINPUT_KEYSTROKE m_keystroke;

};