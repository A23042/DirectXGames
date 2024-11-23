#include "CollisionManager.h"

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
	transform.position = sphere.center;
	if (RefVec.Length() > 0)
	{
		sphere.velocity = RefVec;
	}
	return;
}