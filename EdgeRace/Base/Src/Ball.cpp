#include "Ball.h"
#include "Box.h"
#include "OutlineBall.h"

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
	child = new OutlineBall(this);
	meshCol = new MeshCollider();
	meshCol->MakeFromMesh(mesh);
	ObjectManager::AddObj(this);
}

Ball::~Ball()
{
	child->DestroyMe();
	SAFE_DELETE(mesh);
	SAFE_DELETE(meshCol);
	ObjectManager::RemoveObj(this);
}

void Ball::Start()
{
	pObj.center = transform.position;
	//areaes = ObjectManager::FindGameObjectsWithTag<ScoreArea>("SCOREAREA");
}


void Ball::Update()
{
	if (isPhysic)
	{
		pObj.velocity.y -= Gravity * SceneManager::DeltaTime();
	}
	pObj.center += pObj.velocity * SceneManager::DeltaTime();
	transform.position = pObj.center;

}


