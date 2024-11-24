#pragma once
#include "Object3D.h"
#include "Animator.h"
#include "Ball.h"
#include "ScoreArea.h"
#include "Line.h"
#include "CollisionManager.h"

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

	/// <summary>
	/// �ړ�����
	/// </summary>
	/// <param name="moveSpeed">�ړ���</param>
	/// <param name="rotSpeed">��]��</param>
	void Move(VECTOR3 moveSpeed = VECTOR3(), VECTOR3 rotSpeed = VECTOR3());

	// Player�ԍ��擾
	int GetPlNum() { return playerNum; }
	int GetState() { return state; }

	void SetTurn() { isMyTurn = true; }
private:
	enum State {
		sNormal = 0,
		sWait,
		sJump,
	}state;

	void UpdateNormal();
	void UpdateWait();
	void UpdateJump();


	bool isPhysic;	// Mapeditor�ŏՓ˔���Ȃǂ��Ȃ�
	bool isWait = false;	// �҂�
	bool isMyTurn = false;	// �����̃^�[����

	std::list<Object3D*> objes;
	std::list<ScoreArea*> areaes;
	std::list<Line*> lines;
	Ball* myBall = nullptr;
	CollisonManager* collManager = nullptr;
	Player* otherplayer = nullptr;

	VECTOR3 StartPos;
	VECTOR3 tempMoveSpeed = VECTOR3();
	VECTOR3 tempRotSpeed = VECTOR3();

	int restShot = 0; // �c���
	int playerNum;	// �v���C���[�ԍ�
	int score = 0;	// �X�R�A
	float pushTime[2] = {};	// ����������
	float myE, myF;	// �����W���Ɩ��C��ۑ����Ă���

	//XINPUT_STATE m_state;
	//XINPUT_KEYSTROKE m_keystroke;
};