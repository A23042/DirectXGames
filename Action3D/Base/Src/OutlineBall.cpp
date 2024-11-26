#include "OutlineBall.h"

OutlineBall::OutlineBall(Object3D* parent)
{
	mesh = new CFbxMesh;
	mesh->Load("Data/Object/blackBall.mesh");
	transform.SetParent(parent);
	transform.scale = VECTOR3(0.97f, 0.97f, 0.97f);
}

OutlineBall::~OutlineBall()
{
	if (mesh != nullptr)
	{
		delete mesh;
		mesh = nullptr;
	}

}
