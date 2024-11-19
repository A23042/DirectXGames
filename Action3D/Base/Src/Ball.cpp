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

Ball::Ball(bool isPhysic)
{
	SetTag("BALL");
	editObj.name = "Ball";
	mesh = new CFbxMesh();
	mesh->Load("Data/Object/ball01.mesh");
	meshCol = new MeshCollider();
	meshCol->MakeFromMesh(mesh);

	pObj.radius = 0.5f;
	pObj.isPlayer = false;
	this->isPhysic = isPhysic;
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
		// Box�Ƃ̏Փ˔���
		std::list<Box*> boxes = ObjectManager::FindGameObjects<Box>();
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

		// �X�R�A�G���A�̒��ɂ��邩
		std::list<ScoreArea*> areaes = ObjectManager::FindGameObjectsWithTag<ScoreArea>("SCOREAREA");
		for (ScoreArea* area : areaes)
		{
			if (area->CheckSphereAABBCollision(this->pObj))
			{
				area->ScoreCount(this->pObj);
			}
		}


	}
}

void Ball::PushVec(VECTOR3 pushVec, VECTOR3 RefVec)
{
	pObj.center += pushVec;
	transform.position = pObj.center;
	if (RefVec.Length() > 0) 
	{
		pObj.velocity = RefVec;
	}
	return;
}

