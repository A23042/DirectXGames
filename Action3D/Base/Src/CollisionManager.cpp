#include "CollisionManager.h"

namespace { // このcpp以外では使えない
	static const float Gravity = 9.8f * 4; // 重力加速度(正の値)
	// C++の定数定義（型が付く）
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
	areaes = ObjectManager::FindGameObjectsWithTag<ScoreArea>("SCOREAREA");
}

void CollisonManager::Update()
{
#if 1
	/*
	// BallをFindGameObjectsして
	// Aを保存、AからB〜Nまで衝突判定を実行
	// Aは衝突判定されたからBからC〜Nまで衝突判定
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
		if (targetBall.size() > 0)
		{
			// 衝突判定を取られるBallを格納する
			for (Ball* ball : targetBall)
			{
				HitSphereToSphere(checkBall->pObj, ball->pObj);
				// checkBallからtargetBallに対して衝突判定
				//if (HitSphereToSphere(checkBall->pObj, ball->pObj))
				{
					//checkBall->SetPosition(checkBall->pObj.center);
					//ball->SetPosition(ball->pObj.center);
				}
			}
		}
		// checkBallからresoleveBall以外すべてのballと衝突判定を取り終えたら
		// checkBallをresolveBallに格納する
		resolvedBalls.push_back(checkBall);
		targetBall.push_back(checkBall);
	}
	// resolveBallsをクリアする
	resolvedBalls.clear();
	targetBall.clear();
#endif

	// 改善前
#if 0

	/*
	// BallをFindGameObjectsして
	// Aを保存、AからB〜Nまで衝突判定を実行
	// Aは衝突判定されたからBからC〜Nまで衝突判定
	// を繰り返す？
	*/


	// 衝突判定を取りに行くBallを一つづつ回す
	//std::list<Ball*> balls = ObjectManager::FindGameObjects<Ball>();
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
		if (std::find(resolvedBalls.begin(), resolvedBalls.end(), checkBall) == resolvedBalls.end())
		{
			// 衝突判定を取られるBallを格納する
			for (Ball* targetBall : balls)
			{
				// 衝突判定を取りに行くBallと取られるBallが同じでなければ進
				if (targetBall != checkBall)
				{
					// checkBallからresolveBall以外すべてのtargetBallに衝突判定を取る
					// std::find(探索する先頭、末尾、探すもの)あったら？
					if (std::find(resolvedBalls.begin(), resolvedBalls.end(), targetBall) == resolvedBalls.end())
					{
						// checkBallからtargetBallに対して衝突判定
						if (HitSphereToSphere(checkBall->pObj, targetBall->pObj))
						{
							checkBall->SetPosition(checkBall->pObj.center);
							targetBall->SetPosition(targetBall->pObj.center);
						}
					}
				}
			}
		}
		// checkBallからresoleveBall以外すべてのballと衝突判定を取り終えたら
		// checkBallをresolveBallに格納する
		resolvedBalls.push_back(checkBall);
	}
	// resolveBallsをクリアする
	resolvedBalls.clear();
#endif

}

void CollisonManager::PushVec(PhysicsObject& sphere, VECTOR3 pushVec, VECTOR3 RefVec)
{
	sphere.center += pushVec;
	if (RefVec.Length() > 0)
	{
		sphere.velocity = RefVec;
	}
	return;
}

void CollisonManager::AddBall(Ball* ball)
{
	// リストに入っていなければ追加
	if (std::find(balls.begin(), balls.end(), ball) == balls.end())
	{
		balls.push_back(ball);
	}
}

void CollisonManager::AddBox(Box* box)
{
	// リストに入っていなければ追加
	if (std::find(boxes.begin(), boxes.end(), box) == boxes.end())
	{
		boxes.push_back(box);
	}
}

void CollisonManager::AddArea(ScoreArea* area)
{
	// リストに入っていなければ追加
	if (std::find(areaes.begin(), areaes.end(), area) == areaes.end())
	{
		areaes.push_back(area);
	}
}

void CollisonManager::RemoveBall(Ball* ball)
{
	if (std::find(balls.begin(), balls.end(), ball) != balls.end())
	{
		balls.remove(ball);
		ball->DestroyMe();
	}
}
