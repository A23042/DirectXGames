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

	// Boxと衝突したときに呼ばれる反射関数
	void PushVec(VECTOR3 pushVec, VECTOR3 RefVec);

	/// <summary>
	/// スタートの位置に戻す
	/// </summary>
	void SetStartPos(bool isFall = false);

	/// <summary>
	/// 状態をリセットする
	/// マップ切り替えなどで呼び出す
	/// </summary>
	void Reset();

	void SetTurn() { isMyTurn = true; }

	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="moveSpeed">移動量</param>
	/// <param name="rotSpeed">回転量</param>
	void Move(VECTOR3 moveSpeed = VECTOR3(), VECTOR3 rotSpeed = VECTOR3());

	// Player番号取得
	int GetPlNum() { return playerNum; }
	// Playerステータス取得
	int GetState() { return state; }
	// ショット回数があるか
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


	bool isPhysic;	// Mapeditorで衝突判定などしない
	bool isWait = false;	// 待つ
	bool isMyTurn = false;	// 自分のターンか

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

	int restShot = 0; // 残り回数
	int playerNum;	// プレイヤー番号
	int score = 0;	// スコア
	float pushTime[2] = {};	// 長押し時間
	float myE, myF;	// 反発係数と摩擦を保存しておく

	//XINPUT_STATE m_state;
	//XINPUT_KEYSTROKE m_keystroke;

	float rate[2] = {};
};