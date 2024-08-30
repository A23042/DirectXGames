#pragma once
#include "Object3D.h"
#include "Animator.h"

class Dancer : public Object3D {
public:
	Dancer();
	~Dancer();
	void Start() override; // 最初のUpdateを呼ぶ前に1回呼ばれる
	void Update() override;
	SphereCollider Collider() override;
	void AddDamage(float damage, VECTOR3 pPos);
private:
	Animator* animator;
	float hitPoint;

	enum State {
		sNormal = 0, // 通常状態(巡回)
		sReach,      // 追いかける
		sBack,       // 帰る
		sDead, // 死亡
	};
	State state;
	void updateNormal();
	void updateReach();
	void updateBack();
	void updateDead();
	float deadTimer; // 死亡時間

	VECTOR3 areaCenter; // エリアの中心座標

	void moveToTarget(VECTOR3 target);

	int aroundID;
};