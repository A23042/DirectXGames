#include "CollisionManager.h"

namespace { // ����cpp�ȊO�ł͎g���Ȃ�
	static const float Gravity = 9.8f * 4; // �d�͉����x(���̒l)
	// C++�̒萔��`�i�^���t���j
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
	// Ball��FindGameObjects����
	// A��ۑ��AA����B�`N�܂ŏՓ˔�������s
	// A�͏Փ˔��肳�ꂽ����B����C�`N�܂ŏՓ˔���
	// ���J��Ԃ��H
	*/
	std::list<Ball*> targetBall;
	// �Փ˔�������ɍs��Ball����Â�
	// �ЂƂO��checkBall��ۑ����Ă����ƏՓ˔�����s��
	for (Ball* checkBall : balls)
	{
		// Box�Ƃ̏Փ˔���
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

#if 1
		// �X�R�A�G���A�̒��ɂ��邩
		for (ScoreArea* area : areaes)
		{
			if (area->CheckSphereAABBCollision(checkBall->pObj))
			{
				area->ScoreCount(checkBall->pObj);
				break;
			}
			else
			{
				pObj.score = 0;
			}
		}
#endif


		// �Փ˔��肪�s��ꂽBall�łȂ����
		if (targetBall.size() > 0)
		{
			// �Փ˔���������Ball���i�[����
			for (Ball* ball : targetBall)
			{
				HitSphereToSphere(checkBall->pObj, ball->pObj);
				// checkBall����targetBall�ɑ΂��ďՓ˔���
				//if (HitSphereToSphere(checkBall->pObj, ball->pObj))
				{
					//checkBall->SetPosition(checkBall->pObj.center);
					//ball->SetPosition(ball->pObj.center);
				}
			}
		}
		// checkBall����resoleveBall�ȊO���ׂĂ�ball�ƏՓ˔�������I������
		// checkBall��resolveBall�Ɋi�[����
		resolvedBalls.push_back(checkBall);
		targetBall.push_back(checkBall);
	}
	// resolveBalls���N���A����
	resolvedBalls.clear();
	targetBall.clear();
#endif

	// ���P�O
#if 0

	/*
	// Ball��FindGameObjects����
	// A��ۑ��AA����B�`N�܂ŏՓ˔�������s
	// A�͏Փ˔��肳�ꂽ����B����C�`N�܂ŏՓ˔���
	// ���J��Ԃ��H
	*/


	// �Փ˔�������ɍs��Ball����Â�
	//std::list<Ball*> balls = ObjectManager::FindGameObjects<Ball>();
	for (Ball* checkBall : balls)
	{
		// Box�Ƃ̏Փ˔���
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


		// �Փ˔��肪�s��ꂽBall�łȂ����
		if (std::find(resolvedBalls.begin(), resolvedBalls.end(), checkBall) == resolvedBalls.end())
		{
			// �Փ˔���������Ball���i�[����
			for (Ball* targetBall : balls)
			{
				// �Փ˔�������ɍs��Ball�Ǝ����Ball�������łȂ���ΐi
				if (targetBall != checkBall)
				{
					// checkBall����resolveBall�ȊO���ׂĂ�targetBall�ɏՓ˔�������
					// std::find(�T������擪�A�����A�T������)��������H
					if (std::find(resolvedBalls.begin(), resolvedBalls.end(), targetBall) == resolvedBalls.end())
					{
						// checkBall����targetBall�ɑ΂��ďՓ˔���
						if (HitSphereToSphere(checkBall->pObj, targetBall->pObj))
						{
							checkBall->SetPosition(checkBall->pObj.center);
							targetBall->SetPosition(targetBall->pObj.center);
						}
					}
				}
			}
		}
		// checkBall����resoleveBall�ȊO���ׂĂ�ball�ƏՓ˔�������I������
		// checkBall��resolveBall�Ɋi�[����
		resolvedBalls.push_back(checkBall);
	}
	// resolveBalls���N���A����
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
	// ���X�g�ɓ����Ă��Ȃ���Βǉ�
	if (std::find(balls.begin(), balls.end(), ball) == balls.end())
	{
		balls.push_back(ball);
	}
}

void CollisonManager::AddBox(Box* box)
{
	// ���X�g�ɓ����Ă��Ȃ���Βǉ�
	if (std::find(boxes.begin(), boxes.end(), box) == boxes.end())
	{
		boxes.push_back(box);
	}
}

void CollisonManager::AddArea(ScoreArea* area)
{
	// ���X�g�ɓ����Ă��Ȃ���Βǉ�
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
