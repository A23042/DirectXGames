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

void CollisonManager::Update()
{
	// �Փ˔�������ɍs��Ball����Â�
	// �ЂƂO��checkBall��ۑ����Ă����ƏՓ˔�����s��
	std::list<Ball*> targetBall;	// �Փ˔�������Ball
	for (Ball* checkBall : ObjectManager::anyObjList<Ball>)
	{
#if 1
		// Box�Ƃ̏Փ˔���
		for (Box* box : ObjectManager::anyObjList<Box>)
		{
			if (box->CheckSphereAABBCollision(checkBall->pObj))
			{
				VECTOR3 refVec = VECTOR3(0, 0, 0);
				VECTOR3 pushVec = VECTOR3(0, 0, 0);
				pushVec = box->HitSphereToCubeplane(checkBall->pObj, refVec);
				PushVec(checkBall->pObj, -pushVec, refVec);
			}
		}
#endif
#if 1
		// ������Area�ɐڐG���Ă����狗������ԋ߂����̂̓_�ɂ���
		ScoreArea* tempArea = nullptr;
		float minDistance = 0.0f;
		bool firstFlag = true;
		bool isTouch = false;
		// �X�R�A�G���A�̒��ɂ��邩
		for (ScoreArea* area : ObjectManager::scArea<ScoreArea>)
		{
			if (area == nullptr) continue;
			float distance = 0.0f;
			if (area->CheckSphereAABBCollision(checkBall->pObj, distance))
			{
				isTouch = true;
				if (firstFlag)
				{
					tempArea = area;
					minDistance = distance;
					firstFlag = false;
				}
				else if(distance < minDistance)
				{
					tempArea = area;
					minDistance = distance;
				}
			}
		}
		if (!isTouch)
		{
			pObj.score = 0;
		}
		else
		{
			tempArea->ScoreCount(checkBall->pObj);
			//break;
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
		// checkBall��targetBall�Ɋi�[����
		targetBall.push_back(checkBall);
	}
	// targetBalls���N���A����
	targetBall.clear();

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
#if 0
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
		//ball->DestroyMe();
	}
}
#endif
