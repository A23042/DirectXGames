#include "Ball.h"
#include "Box.h"

namespace { // このcpp以外では使えない
	static const float Gravity = 9.8f * 3; // 重力加速度(正の値)
	// C++の定数定義（型が付く）
	static const float JumpPower = 0.3f;
	static const float RotationSpeed = 3.0f; // 回転速度(度)
	static const float MoveSpeed = 0.8f;
};

Ball::Ball()
{
	//SetTag("STAGEOBJ");
	mesh = new CFbxMesh();
	mesh->Load("Data/Object/ball01.mesh");
	meshCol = new MeshCollider();
	meshCol->MakeFromMesh(mesh);

	sphere.radius = 0.5f;
	sphere.isPlayer = false;
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
	sphere.center = transform.position;
}


void Ball::Update()
{
	//sphere.velocity.y -= Gravity * SceneManager::DeltaTime();

	sphere.center += sphere.velocity * SceneManager::DeltaTime();
	transform.position = sphere.center;

	// Boxとの衝突判定
	std::list<Box*> boxes = ObjectManager::FindGameObjects<Box>();
	for (Box* box : boxes) {
		VECTOR3 refVec = VECTOR3(0, 0, 0);
		VECTOR3 pushVec = VECTOR3(0, 0, 0);
		pushVec = box->HitSphereToCubeplane(this->sphere, refVec);
		PushVec(-pushVec, refVec);
	}
}

void Ball::PushVec(VECTOR3 pushVec, VECTOR3 RefVec)
{
	sphere.center += pushVec;
	transform.position = sphere.center;
	if (RefVec.Length() > 0) {
		sphere.velocity = RefVec;
	}
	return;
}

