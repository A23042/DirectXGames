#include "FallCheck.h"
#include "Player.h"
#include "Ball.h"

FallCheck::FallCheck(bool playScene) : isPlayScene(playScene)
{
	SetTag("STAGEOBJ");
	editObj.name = "FallCheck";
	pObj.center = transform.position;

	if (isPlayScene)
	{
		pObj.center = transform.position;
		ObjectManager::SetVisible(this,false);
	}
	else
	{
		mesh = new CFbxMesh();
		mesh->Load("Data/Gizmo/FallCheck.mesh");

		meshCol = new MeshCollider();
		meshCol->MakeFromMesh(mesh);
		ObjectManager::SetDrawOrder(this, 100);
	}
}

FallCheck::~FallCheck()
{
}

void FallCheck::Update()
{
	if (collManager == nullptr)
	{
		collManager = ObjectManager::FindGameObject<CollisonManager>();
	}

	if (isPlayScene)
	{
		// Player�������珉���ʒu�ɖ߂�
		std::list<Player*> players = ObjectManager::FindGameObjects<Player>();
		for (Player* player : players)
		{
			if (player->Position().y < transform.position.y)
			{
				player->SetStartPos(true);
			}
		}

		// Ball����������
		// CollisionManaer�̃��X�g����r��
		for (Ball* ball : ObjectManager::anyObjList<Ball>)
		{
			if (ball->Position().y < transform.position.y)
			{
				//collManager->RemoveBall(ball);
				ball->DestroyMe();
			}
		}
	}
	else
	{
		transform.position = pObj.center;
	}
}
