#include "Ball.h"
#include "Box.h"

namespace { // ����cpp�ȊO�ł͎g���Ȃ�
	static const float Gravity = 9.8f * 3; // �d�͉����x(���̒l)
	// C++�̒萔��`�i�^���t���j
	static const float JumpPower = 0.3f;
	static const float RotationSpeed = 3.0f; // ��]���x(�x)
	static const float MoveSpeed = 0.8f;
};

Ball::Ball(bool isPhysic) : isPhysic(isPhysic)
{
	//SetTag("STAGEOBJ");
	pObj.name = "Ball";
	mesh = new CFbxMesh();
	mesh->Load("Data/Object/ball01.mesh");
	meshCol = new MeshCollider();
	meshCol->MakeFromMesh(mesh);

	pObj.radius = 0.5f;
	pObj.isPlayer = false;
}

Ball::~Ball()
{
	if (mesh != nullptr) {
		delete mesh;
		mesh = nullptr;
	}
}

void Ball::Start()
{
	pObj.center = transform.position;
}


void Ball::Update()
{
	pObj.center += pObj.velocity * SceneManager::DeltaTime();
	transform.position = pObj.center;
	if (isPhysic)
	{
		pObj.velocity.y -= Gravity * SceneManager::DeltaTime();
		// Box�Ƃ̏Փ˔���
		std::list<Box*> boxes = ObjectManager::FindGameObjects<Box>();
		for (Box* box : boxes) {
			VECTOR3 refVec = VECTOR3(0, 0, 0);
			VECTOR3 pushVec = VECTOR3(0, 0, 0);
			pushVec = box->HitSphereToCubeplane(this->pObj, refVec);
			PushVec(-pushVec, refVec);
		}
	}
}

void Ball::PushVec(VECTOR3 pushVec, VECTOR3 RefVec)
{
	pObj.center += pushVec;
	transform.position = pObj.center;
	if (RefVec.Length() > 0) {
		pObj.velocity = RefVec;
	}
	return;
}

