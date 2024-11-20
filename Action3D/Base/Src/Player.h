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

	// Box�ƏՓ˂����Ƃ��ɌĂ΂�锽�ˊ֐�
	void PushVec(VECTOR3 pushVec, VECTOR3 RefVec);

	/// <summary>
	/// �X�^�[�g�̈ʒu�ɖ߂�
	/// </summary>
	void SetStartPos(bool isFall = false);

	// Player�ԍ��擾
	int GetPlNum() { return playerNum; }
	int GetState() { return state; }
private:
	enum State {
		sNormal = 0,
		sWait,
		sJump,
	}state;

	void UpdateNormal();
	void UpdateWait();
	void UpdateJump();

	// Mapeditor�ŏՓ˔���Ȃǂ��Ȃ����߂�Bool
	bool isPhysic;
	bool isWait = false;

	Object3D* myBall = nullptr;
	Player* otherplayer = nullptr;

	VECTOR3 sumVelocity;
	VECTOR3 StartPos;

	int restShot = 0; // �c���
	int playerNum;	// �v���C���[�ԍ�
	int score = 0;	// �X�R�A
	float pushTime[2] = {};	// 

	//XINPUT_STATE m_state;
	//XINPUT_KEYSTROKE m_keystroke;

};