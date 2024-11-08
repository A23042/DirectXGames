#include "Camera.h"
#include "Player.h"
#include "Ball.h"
//                                      ������_�@�@�@�@�@�@�^�㎋�_
static const VECTOR3 CameraPos[] = { VECTOR3(0, 4, -10), VECTOR3(0, 10, -0.5) };
static const VECTOR3 LookPos[] =   { VECTOR3(0, 0,  5), VECTOR3(0,  1,  1  ) };
static const float CHANGE_TIME_LIMIT = 0.5f; // �b

Camera::Camera()
{
	ObjectManager::SetVisible(this, false);	// ���̔�\��
	SetDrawOrder(-1000);	// �Ō�ɏ�������
	viewType = 0;
	changeTime = CHANGE_TIME_LIMIT; // �؂�ւ��Ȃ�

	ssObj = ObjectManager::FindGameObject<SplitScreen>();
	while (view.size() < ssObj->MultiSize())
	{
		MATRIX4X4 m = XMMatrixIdentity();
		view.emplace_back(m);
		VECTOR3 v = VECTOR3(0, 0, 0);
		eyePt.emplace_back(v);
		lookatPt.emplace_back(v);
	}
}

Camera::~Camera()
{
}

void Camera::Update()
{
	if (ssObj->Multi())
	{
		std::list<Player*> pcs = ObjectManager::FindGameObjects<Player>();
		for(Player* pc : pcs)
		// ����ʂ̂Ƃ�
		for (int i = 0; i < ssObj->MultiSize(); i++)
		{

			// �Q��ʂ̂Ƃ�
			switch (i)
			{
			case 0:
			{
				// ���0��Player1���_
				//Player* pc = ObjectManager::FindGameObject<Player>();
					if (pc->GetPlNum() == 0)
					{
						if (pc != nullptr)
						{
							updateCamera(i, pc->Position(), pc->Rotation());
						}
						else {
							updateCamera(i, VECTOR3(0, 0, 0), VECTOR3(0, 0, 0));
						}
					}
			}
			break;

			case 1:
			{
				// ���1��Player2���_
				
				if (pc->GetPlNum() == 1)
				{
#if 1
					if (pc != nullptr)
					{
						updateCamera(i, pc->Position(), pc->Rotation());
					}
					else {
						updateCamera(i, VECTOR3(0, 0, 0), VECTOR3(0, 0, 0));
					}
#else
					updateCamera(i, VECTOR3(0, 2, -15), VECTOR3(0, 0, 0));
#endif
				}
			}
			break;
			}

			/*
			// �S��ʂ̂Ƃ�
			switch (i)
			{
			case 0:
				{
					// ���0��Player���_
					Player* pc = ObjectManager::FindGameObject<Player>();
					if (pc != nullptr)
					{
						updateCamera(i, pc->Position(), pc->Rotation());
					}
					else {
						updateCamera(i, VECTOR3(0,0,0), VECTOR3(0,0,0));
					}
				}
				break;

			case 1:
				{
					// ���1��EnemyRS���_
					EnemyRS* enm = ObjectManager::FindGameObject<EnemyRS>();
					if (enm != nullptr && enm->Mesh() != nullptr)
					{
						updateCamera(i, enm->Position(), enm->Rotation());
					}
					else {
						updateCamera(i, VECTOR3(0, 0, 0), VECTOR3(0, 0, 0));
					}
				}
				break;

			case 2:
				{
					// ���2��EnemyGolem���_
					EnemyGolem* enm = ObjectManager::FindGameObject<EnemyGolem>();
					if (enm != nullptr && enm->Mesh() != nullptr)
					{
						updateCamera(i, enm->Position(), enm->Rotation());
					}
					else {
						updateCamera(i, VECTOR3(0, 0, 0), VECTOR3(0, 0, 0));
					}
				}
				break;

			case 3:
				{
					// ���3�͌��_���_
					updateCamera(i, VECTOR3(0, 0, 0), VECTOR3(0, 0, 0));
				}
				break;
			}
			*/
		}
	}
	else {
		// �P��ʂ̂Ƃ�Player���_
		Player* pc = ObjectManager::FindGameObject<Player>();
		if (pc != nullptr)
		{
			updateCamera(0, pc->Position(), pc->Rotation());
		}
		else {
			updateCamera(0, VECTOR3(0, 0, 0), VECTOR3(0, 0, 0));
		}
	}
}

void Camera::Draw()
{
	GameDevice()->m_mView = XMMatrixLookAtLH(
		transform.position, // �J�������W
		lookPosition, // �����_
		VECTOR3(0, 1, 0));
}

void Camera::updateCamera(int counter, VECTOR3 pos, VECTOR3 rot)
{
			// �v���C���[�̍s������߂�
			MATRIX4X4 rotY = XMMatrixRotationY(rot.y);
			MATRIX4X4 trans = XMMatrixTranslation(pos.x, pos.y / 2.0f + 0.0f, pos.z);
			MATRIX4X4 m = rotY * trans;
			// �v���C���[����]�E�ړ����ĂȂ����̃J�����ʒu��
			// �v���C���[�̉�]�E�ړ��s����|����ƁA
			if (changeTime >= CHANGE_TIME_LIMIT) {
				transform.position = CameraPos[viewType] * m;
				lookPosition = LookPos[viewType] * m;
			}
			else { // ���_�؂�ւ���
				changeTime += 1.0f / 60.0f;
				float timeRate = changeTime / CHANGE_TIME_LIMIT; // 0.0�`1.0
				float rate = timeRate;
				VECTOR3 position = (changePosGoal - changePosStart) * rate + changePosStart;
				VECTOR3 look = (changeLookGoal - changeLookStart) * rate + changeLookStart;
				transform.position = position * m;
				lookPosition = look * m;
			}
			// �J�������ǂɂ߂荞�܂Ȃ��悤�ɂ���
			VECTOR3 start = pos;
			VECTOR3 end = transform.position;
			// start����end�Ɍ������x�N�g�������A������0.2��������
			VECTOR3 camVec = end - start;
			camVec = XMVector3Normalize(camVec) * (camVec.Length() + 0.2f);
			end = start + camVec;
			{

				std::list<Object3D*> objList = ObjectManager::FindGameObjectsWithTag<Object3D>("STAGEOBJ");
				for (Object3D* g : objList)
				{
					VECTOR3 hit;
					if (g->HitLineToMesh(start, end, &hit)) {
						end = hit;
					}
				}
			}
			//end����0.2��O�ɒu��;
			transform.position = XMVector3Normalize(camVec) * ((end - start).Length() - 0.2f) + start;


			// ------------------------------------------------------------------
			// �J�������W��z��ɐݒ肷��
			eyePt[counter] = transform.position;   // �J�������W
			lookatPt[counter] = lookPosition;      // �����_
			view[counter] = XMMatrixLookAtLH(	    // �r���[�}�g���b�N�X
				transform.position,
				lookPosition,
				VECTOR3(0, 1, 0));

			// ------------------------------------------------------------------
			// ���_����̋����̂Q���DrawObject�ɐݒ肷��
			// ����́A���_����̋����̍~���ɕ`�悵��������
			{
				std::list<Object3D*> objList = ObjectManager::FindGameObjects<Object3D>();
				for (Object3D*& obj : objList)
				{
					if (obj != this)
					{
						float distQ = magnitudeSQ(obj->Position() - transform.position);
						ObjectManager::SetEyeDist(obj, distQ, counter);  // ���_����̋����̂Q���DrawObject�ɐݒ�
					}
				}
			}
}
