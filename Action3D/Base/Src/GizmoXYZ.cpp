#include "GizmoXYZ.h"

GizmoXYZ::GizmoXYZ()
{
	SetTag("Gizmo");
	ObjectManager::SetDrawOrder(this, 100);
	pObj.name = "gizmoCenter";

	mesh = new CFbxMesh;
	mesh->Load("Data/Object/gizmoCenter.mesh");
	//meshCol = new MeshCollider;
	//meshCol->MakeFromMesh(mesh);
	transform.scale = VECTOR3(0.2, 0.2, 0.2);
	//new GizmoX();
	//new GizmoY();
	//new GizmoZ();
}

GizmoXYZ::~GizmoXYZ()
{
}

void GizmoXYZ::Update()
{
}

GizmoX::GizmoX()
{
	SetTag("Gizmo");
	ObjectManager::SetDrawOrder(this, 100);
	pObj.name = "gizmoX";

	mesh = new CFbxMesh;
	mesh->Load("Data/Object/gizmoX.mesh");
	meshCol = new MeshCollider;
	meshCol->MakeFromMesh(mesh);
	transform.scale = VECTOR3(0.2, 0.2, 0.2);
	transform.rotation.z = (180.0f / 180 * XM_PI);

}

GizmoX::~GizmoX()
{
}

void GizmoX::Update()
{
}

GizmoY::GizmoY()
{
	SetTag("Gizmo");
	pObj.name = "gizmoY";

	mesh = new CFbxMesh;
	mesh->Load("Data/Object/gizmoY.mesh");
	meshCol = new MeshCollider;
	meshCol->MakeFromMesh(mesh);
	transform.scale = VECTOR3(0.2, 0.2, 0.2);
}

GizmoY::~GizmoY()
{
}

void GizmoY::Update()
{
}

GizmoZ::GizmoZ()
{
	SetTag("Gizmo");
	pObj.name = "gizmoZ";

	mesh = new CFbxMesh;
	mesh->Load("Data/Object/gizmoZ.mesh");
	meshCol = new MeshCollider;
	meshCol->MakeFromMesh(mesh);
	transform.scale = VECTOR3(0.2, 0.2, 0.2);
	transform.rotation.x = (180.0f / 180 * XM_PI);
}

GizmoZ::~GizmoZ()
{
}

void GizmoZ::Update()
{
}
