#include "Line.h"

Line::Line(bool playScene) : isPlayScene(playScene)
{
	SetTag("LINE");
	editObj.name = "Line";

	//ObjectManager::SetVisible(this, false);
	mesh = new CFbxMesh();
	mesh->Load("Data/Gizmo/Line.mesh");
	meshCol = new MeshCollider();
	meshCol->MakeFromMesh(mesh);
	SetDrawOrder(-1);
}

Line::~Line()
{
}
