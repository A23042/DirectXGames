#include "Gizmo3D.h"

Gizmo3D::Gizmo3D()
{
	mesh = new CFbxMesh();
	mesh->Load("Data/Gizmo/gizmo.mesh");
	transform.scale = VECTOR3(0.5f, 0.5f, 0.5f);
	//meshCol = new MeshCollider();
	//meshCol->MakeFromMesh(mesh);
}

Gizmo3D::~Gizmo3D()
{
}

void Gizmo3D::Update()
{
}
