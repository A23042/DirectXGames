#include "CollisionManager.h"

namespace { // このcpp以外では使えない
	static const float Gravity = 9.8f * 3; // 重力加速度(正の値)
	// C++の定数定義（型が付く）
	static const float JumpPower = 0.3f;
	static const float RotationSpeed = 3.0f; // 回転速度(度)
	static const float MoveSpeed = 0.8f;
};

CollisonManager::CollisonManager()
{
	ObjectManager::SetVisible(this,false);
}

CollisonManager::~CollisonManager()
{
}

void CollisonManager::Start()
{
	balls = ObjectManager::FindGameObjects<Ball>();
	boxes = ObjectManager::FindGameObjectsWithTag<Box>("STAGEOBJ");
}

void CollisonManager::Update()
{
#if 1
	/*
	// BallをFindGameObjectsして
	// Aを保存、AからB～Nまで衝突判定を実行
	// Aは衝突判定されたからBからC～Nまで衝突判定
	// を繰り返す？
	*/

	std::list<Ball*> targetBall;
	// 衝突判定を取りに行くBallを一つづつ回す
	// ひとつ前のcheckBallを保存してそれらと衝突判定を行う
	for (Ball* checkBall : balls)
	{
		// Boxとの衝突判定
		for (Box* box : boxes)
		{
			if (box->CheckSphereAABBCollision(checkBall->pObj))
			{
				VECTOR3 refVec = VECTOR3(0, 0, 0);
				VECTOR3 pushVec = VECTOR3(0, 0, 0);
				pushVec = box->HitSphereToCubeplane(checkBall->pObj, refVec);
				PushVec(checkBall->pObj, -pushVec, refVec);
			}
		}

		// 衝突判定が行われたBallでなければ
		//if (std::find(resolvedBalls.begin(), resolvedBalls.end(), checkBall) == resolvedBalls.end())
		//{
		if (targetBall.size() > 0)
		{
			// 衝突判定を取られるBallを格納する
			for (Ball* ball : targetBall)
			{
				// 衝突判定を取りに行くBallと取られるBallが同じでなければ進
				//if (targetBall != checkBall)
				//{
					// checkBallからresolveBall以外すべてのtargetBallに衝突判定を取る
					// std::find(探索する先頭、末尾、探すもの)あったら？
					//if (std::find(resolvedBalls.begin(), resolvedBalls.end(), targetBall) == resolvedBalls.end())
					//{
						// checkBallからtargetBallに対して衝突判定
						if (HitSphereToSphere(checkBall->pObj, ball->pObj))
						{
							checkBall->SetPosition(checkBall->pObj.center);
							ball->SetPosition(ball->pObj.center);
						}
					//}
				//}
			}
		}
		//}
		// checkBallからresoleveBall以外すべてのballと衝突判定を取り終えたら
		// checkBallをresolveBallに格納する
		resolvedBalls.push_back(checkBall);

		targetBall.push_back(checkBall);
	}
	// resolveBallsをクリアする
	resolvedBalls.clear();
#endif
}

void CollisonManager::PushVec(PhysicsObject& sphere, VECTOR3 pushVec, VECTOR3 RefVec)
{
	sphere.center += pushVec;
	transform.position = sphere.center;
	if (RefVec.Length() > 0)
	{
		sphere.velocity = RefVec;
	}
	return;
}