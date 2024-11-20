#include "FallCheck.h"
#include "Player.h"
#include "Ball.h"

FallCheck::FallCheck(bool playScene) : isPlayScene(playScene)
{
	SetTag("STAGEOBJ");
	editObj.name = "FallCheck";
	mesh = new CFbxMesh();
	mesh->Load("Data/Gizmo/FallCheck.mesh");

	meshCol = new MeshCollider();
	meshCol->MakeFromMesh(mesh);
	pObj.center = transform.position;

	//transform.scale = VECTOR3(0.5f, 0.5f, 0.5f);

	if (isPlayScene)
	{
		pObj.center = transform.position;
		ObjectManager::SetVisible(this,false);
	}
}

FallCheck::~FallCheck()
{
}

void FallCheck::Update()
{
	if (isPlayScene)
	{
		// Player—‚¿‚½‚ç‰ŠúˆÊ’u‚É–ß‚·
		std::list<Player*> players = ObjectManager::FindGameObjects<Player>();
		for (Player* player : players)
		{
			if (player->Position().y < transform.position.y)
			{
				player->SetStartPos(true);
			}
		}

		// Ball‚ª—‚¿‚½‚çíœ
		std::list<Ball*> balls = ObjectManager::FindGameObjects<Ball>();
		for (Ball* ball : balls)
		{
			if (ball->Position().y < transform.position.y)
			{
				ball->DestroyMe();
			}
		}
	}
	else
	{
		transform.position = pObj.center;
	}
}
