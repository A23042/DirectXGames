#include "SkyBox.h"

SkyBox::SkyBox()
{
	mesh = new CFbxMesh;
	mesh->Load("Data/Map/sky2.mesh");
	transform.scale = VECTOR3(5, 5, 5);
	lightDir = GameDevice()->m_vLightDir;
}

SkyBox::~SkyBox()
{
	SAFE_DELETE(mesh);
}

void SkyBox::Update()
{
}

void SkyBox::Draw()
{
	GameDevice()->m_vLightDir = VECTOR3();
	mesh->Render(transform.matrix());
	GameDevice()->m_vLightDir = lightDir;
}
