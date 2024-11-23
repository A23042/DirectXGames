#include "Ball.h"
#include "Box.h"
#include "ScoreArea.h"

namespace { // このcpp以外では使えない
	static const float Gravity = 9.8f * 3; // 重力加速度(正の値)
	// C++の定数定義（型が付く）
	static const float JumpPower = 0.3f;
	static const float RotationSpeed = 3.0f; // 回転速度(度)
	static const float MoveSpeed = 0.8f;
};

Ball::Ball(bool isPhysic,int pNum)
{
	SetTag("PLAYER");
	editObj.name = "Ball";

	pObj.radius = 0.5f;
	pObj.isPlayer = false;
	pObj.pNum = pNum;
	this->isPhysic = isPhysic;

	mesh = new CFbxMesh();
	if (pObj.pNum == 0)
	{
		mesh->Load("Data/Object/blueBall.mesh");
	}
	else if(pObj.pNum == 1)
	{
		mesh->Load("Data/Object/orengeBall.mesh");
	}
	else
	{
		mesh->Load("Data/Object/ball01.mesh");
	}
	meshCol = new MeshCollider();
	meshCol->MakeFromMesh(mesh);

}

Ball::~Ball()
{
	if (mesh != nullptr) 
	{
		delete mesh;
		mesh = nullptr;
	}
	if (meshCol != nullptr)
	{
		delete meshCol;
		meshCol = nullptr;
	}
}

void Ball::Start()
{
	pObj.center = transform.position;
}


void Ball::Update()
{
	if (isPhysic)
	{
		pObj.velocity.y -= Gravity * SceneManager::DeltaTime();
	}
	pObj.center += pObj.velocity * SceneManager::DeltaTime();
	transform.position = pObj.center;

	if (isPhysic)
	{
		//
		// 現在一度BallAからBallBに衝突判定をしても
		//			BallBからBallAに衝突判定を取っているので一度しか衝突判定をしない様に改善する
		// 
#if 0
		// Boxとの衝突判定
		std::list<Box*> boxes = ObjectManager::FindGameObjectsWithTag<Box>("STAGEOBJ");
		for (Box* box : boxes) 
		{
			if (box->CheckSphereAABBCollision(this->pObj))
			{
				VECTOR3 refVec = VECTOR3(0, 0, 0);
				VECTOR3 pushVec = VECTOR3(0, 0, 0);
				pushVec = box->HitSphereToCubeplane(this->pObj, refVec);
				PushVec(-pushVec, refVec);
			}
		}
#endif
#if 0
		// 自分以外のBallと衝突判定
		std::list<Ball*> otherBall = ObjectManager::FindGameObjects<Ball>();
		for (Ball* ball : otherBall)
		{
			if (ball != this)
			{
				if (ball->HitSphereToSphere(this->pObj))
				{
					ball->SetPosition(ball->pObj.center);
					transform.position = pObj.center;
				}
			}
		}
#endif
		// スコアエリアの中にいるか
		std::list<ScoreArea*> areaes = ObjectManager::FindGameObjectsWithTag<ScoreArea>("SCOREAREA");
		for (ScoreArea* area : areaes)
		{
			if (area->CheckSphereAABBCollision(this->pObj))
			{
				area->ScoreCount(this->pObj);
				break;
			}
			else
			{
				pObj.score = 0;
			}
		}
	}
}


