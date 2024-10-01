#include "Ball.h"
#include "Box.h"

Ball::Ball()
{
	//SetTag("STAGEOBJ");
	mesh = new CFbxMesh();
	mesh->Load("Data/Object/ball01.mesh");

	
}

Ball::~Ball()
{
	if (mesh != nullptr) {
		delete mesh;
		mesh = nullptr;
	}
}

void Ball::Update()
{
	std::list<Box*> box = ObjectManager::FindGameObjects<Box>();
	for (Box* boxes : box) {
		SphereCollider ballCol = Collider();
		VECTOR3 boxCenter = boxes->Position();
		
	}
}

void Ball::PushVec(VECTOR3 pushVec)
{
	sphere.center += pushVec;
	transform.position = sphere.center;
	return;
}

