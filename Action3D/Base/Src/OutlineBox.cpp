#include "OutlineBox.h"

OutlineBox::OutlineBox(Object3D* parent)
{
	mesh = new CFbxMesh;
	mesh->Load("Data/Object/blackBox.mesh");
	transform.SetParent(parent);
}

OutlineBox::~OutlineBox()
{
}
