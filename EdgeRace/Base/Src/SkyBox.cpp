#include "SkyBox.h"

SkyBox::SkyBox()
{
	mesh = new CFbxMesh;
	mesh->Load("Data/Map/sky2.mesh");
	transform.scale = VECTOR3(10, 10, 10);
}

SkyBox::~SkyBox()
{
	SAFE_DELETE(mesh);
}

void SkyBox::Update()
{
}
