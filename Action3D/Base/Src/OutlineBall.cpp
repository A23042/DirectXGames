#include "OutlineBall.h"

OutlineBall::OutlineBall(Object3D* parent)
{
	mesh = new CFbxMesh;
	mesh->Load("Data/Object/blackBall.mesh");
	transform.SetParent(parent);
}

OutlineBall::~OutlineBall()
{
}
