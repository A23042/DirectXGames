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
		//ObjectManager::SetDrawOrder(this, 100);
		//mesh->m_vDiffuse = VECTOR4(1.0f, 1.0f, 1.0f, 0.6f);
	}
}

FallCheck::~FallCheck()
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

void FallCheck::Update()
{
	if (collManager == nullptr)
	{
		collManager = ObjectManager::FindGameObject<CollisonManager>();
	}

	if (isPlayScene)
	{
		// Player落ちたら初期位置に戻す
		std::list<Player*> players = ObjectManager::FindGameObjects<Player>();
		for (Player* player : players)
		{
			if (player->Position().y < transform.position.y)
			{
				player->SetStartPos(true);
			}
		}

		// Ballが落ちたら
		// CollisionManaerのリストから排除
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
