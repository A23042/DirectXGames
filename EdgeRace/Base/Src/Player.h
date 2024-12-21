#pragma once
#include "Object3D.h"
#include "Animator.h"
#include "Ball.h"
#include "ScoreArea.h"
#include "Line.h"
//#include "CollisionManager.h"
#include "LoadStage.h"

#include <Xinput.h>

class Player : public Object3D {
public:
	Player(int num = -1, bool isPhysic = true);
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
	/// ��Ԃ����Z�b�g����
	/// �}�b�v�؂�ւ��ȂǂŌĂяo��
	/// </summary>
	void Reset();

	void SetTurn() { isMyTurn = true; }

	/// <summary>
	/// �ړ�����
	/// </summary>
	/// <param name="moveSpeed">�ړ���</param>
	/// <param name="rotSpeed">��]��</param>
	void Move(VECTOR3 moveSpeed = VECTOR3(), VECTOR3 rotSpeed = VECTOR3());

	// Player�ԍ��擾
	int GetPlNum() { return playerNum; }
	// Player�X�e�[�^�X�擾
	int GetState() { return state; }
	// �V���b�g�񐔂����邩
	bool GetRestShot() { return restShot > 0; }

	float GetRate(int i) { return rate[i]; }
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

	//std::list<ScoreArea*> areaes;
	//std::list<Line*> lines;
	Object3D* child = nullptr;
	Ball* myBall = nullptr;
	//CollisonManager* collManager = nullptr;
	Player* otherplayer = nullptr;
	LoadStage* loadStage = nullptr;

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

	float rate[2] = {};
};