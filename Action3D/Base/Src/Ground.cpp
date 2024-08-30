#include "Ground.h"

Ground::Ground()
{
	SetTag("STAGEOBJ");

	mesh = new CFbxMesh();
	mesh->Load("Data/Map/map50Field1.mesh");
	//mesh->Load("Data/Map/dodai.mesh");

	meshCol = new MeshCollider();
	meshCol->MakeFromMesh(mesh);
}

Ground::~Ground()
{
	if (mesh != nullptr) {
		delete mesh;
		mesh = nullptr;
	}
}
