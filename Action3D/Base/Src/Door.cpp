#include "Door.h"

Door::Door()
{
	SetTag("STAGEOBJ");
	mesh = new CFbxMesh();
	mesh->Load("Data/MapItem/DOOR1.mesh");

	meshCol = new MeshCollider();
	meshCol->MakeFromMesh(mesh);
	transform.position;
}

Door::~Door()
{
	if (mesh != nullptr) {
		delete mesh;
		mesh = nullptr;
	}
}
