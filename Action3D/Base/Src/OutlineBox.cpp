#include "OutlineBox.h"

OutlineBox::OutlineBox(Object3D* parent)
{
	mesh = new CFbxMesh;
	mesh->Load("Data/Object/blackBox.mesh");
	transform.SetParent(parent);
	transform.scale = VECTOR3(0.97f, 0.97f, 0.97f);
}

OutlineBox::~OutlineBox()
{
	if (mesh != nullptr)
	{
		delete mesh;
		mesh = nullptr;
	}
}
