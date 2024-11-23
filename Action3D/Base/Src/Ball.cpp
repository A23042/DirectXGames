#include "Ball.h"
#include "Box.h"
#include "ScoreArea.h"

namespace { // ����cpp�ȊO�ł͎g���Ȃ�
	static const float Gravity = 9.8f * 3; // �d�͉����x(���̒l)
	// C++�̒萔��`�i�^���t���j
	static const float JumpPower = 0.3f;
	static const float RotationSpeed = 3.0f; // ��]���x(�x)
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
		// ���݈�xBallA����BallB�ɏՓ˔�������Ă�
		//			BallB����BallA�ɏՓ˔��������Ă���̂ň�x�����Փ˔�������Ȃ��l�ɉ��P����
		// 
#if 0
		// Box�Ƃ̏Փ˔���
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
		// �����ȊO��Ball�ƏՓ˔���
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
		// �X�R�A�G���A�̒��ɂ��邩
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


