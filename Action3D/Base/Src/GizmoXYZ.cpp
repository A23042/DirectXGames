#include "GizmoXYZ.h"

GizmoXYZ::GizmoXYZ()
{
	SetTag("Gizmo");
	ObjectManager::SetDrawOrder(this, 100);
	pObj.name = "gizmoCenter";
	mesh = new CFbxMesh;
	mesh->Load("Data/Gizmo/gizmoCenter.mesh");
	transform.scale = VECTOR3(0.2, 0.2, 0.2);
	ObjectManager::SetVisible(this,false);
}

GizmoXYZ::~GizmoXYZ()
{
}

void GizmoXYZ::Update()
{
}

PosGizmoX::PosGizmoX(Object3D* parent)
{
	SetTag("Gizmo");
	ObjectManager::SetDrawOrder(this, 100);
	pObj.name = "posGizmoX";
	transform.SetParent(parent);
	mesh = new CFbxMesh;
	mesh->Load("Data/Gizmo/PosGizmoX.mesh");
	meshCol = new MeshCollider;
	meshCol->MakeFromMesh(mesh);
	transform.rotation.z = (180.0f / 180 * XM_PI);
	transform.scale = VECTOR3(1, 1, 1);
	ObjectManager::SetVisible(this, false);
}

PosGizmoX::~PosGizmoX()
{
}

void PosGizmoX::Update()
{
}

PosGizmoY::PosGizmoY(Object3D* parent)
{
	SetTag("Gizmo");
	pObj.name = "posGizmoY";
	transform.SetParent(parent);
	mesh = new CFbxMesh;
	mesh->Load("Data/Gizmo/PosGizmoY.mesh");
	meshCol = new MeshCollider;
	meshCol->MakeFromMesh(mesh);
	transform.scale = VECTOR3(1, 1, 1);
	ObjectManager::SetVisible(this, false);
}

PosGizmoY::~PosGizmoY()
{
}

void PosGizmoY::Update()
{
}

PosGizmoZ::PosGizmoZ(Object3D* parent)
{
	SetTag("Gizmo");
	pObj.name = "posGizmoZ";
	transform.SetParent(parent);
	mesh = new CFbxMesh;
	mesh->Load("Data/Gizmo/PosGizmoZ.mesh");
	meshCol = new MeshCollider;
	meshCol->MakeFromMesh(mesh);
	transform.rotation.x = (180.0f / 180 * XM_PI);
	transform.scale = VECTOR3(1, 1, 1);
	ObjectManager::SetVisible(this, false);
}

PosGizmoZ::~PosGizmoZ()
{
}

void PosGizmoZ::Update()
{
}

RotGizmoX::RotGizmoX(Object3D* parent)
{
	SetTag("Gizmo");
	pObj.name = "RotGizmoX";
	transform.SetParent(parent);
	mesh = new CFbxMesh;
	mesh->Load("Data/Gizmo/RotGizmoX.mesh");
	meshCol = new MeshCollider;
	meshCol->MakeFromMesh(mesh);
	transform.rotation.x = (180.0f / 180 * XM_PI);
	transform.scale = VECTOR3(1, 1, 1);
	ObjectManager::SetVisible(this, false);
}

RotGizmoX::~RotGizmoX()
{
}

void RotGizmoX::Update()
{
}
RotGizmoY::RotGizmoY(Object3D* parent)
{
	SetTag("Gizmo");
	pObj.name = "RotGizmoY";
	transform.SetParent(parent);
	mesh = new CFbxMesh;
	mesh->Load("Data/Gizmo/RotGizmoY.mesh");
	meshCol = new MeshCollider;
	meshCol->MakeFromMesh(mesh);
	transform.rotation.x = (180.0f / 180 * XM_PI);
	transform.scale = VECTOR3(1, 1, 1);
	ObjectManager::SetVisible(this, false);
}

RotGizmoY::~RotGizmoY()
{
}

void RotGizmoY::Update()
{
}
RotGizmoZ::RotGizmoZ(Object3D* parent)
{
	SetTag("Gizmo");
	pObj.name = "RotGizmoZ";
	transform.SetParent(parent);
	mesh = new CFbxMesh;
	mesh->Load("Data/Gizmo/RotGizmoZ.mesh");
	meshCol = new MeshCollider;
	meshCol->MakeFromMesh(mesh);
	transform.rotation.x = (180.0f / 180 * XM_PI);
	transform.scale = VECTOR3(1, 1, 1);
	ObjectManager::SetVisible(this, false);
}

RotGizmoZ::~RotGizmoZ()
{
}

void RotGizmoZ::Update()
{
}

ScaleGizmoX::ScaleGizmoX(Object3D* parent)
{
	SetTag("Gizmo");
	pObj.name = "ScaleGizmoX";
	transform.SetParent(parent);
	mesh = new CFbxMesh;
	mesh->Load("Data/Gizmo/ScaleGizmoX.mesh");
	meshCol = new MeshCollider;
	meshCol->MakeFromMesh(mesh);
	transform.rotation.z = (180.0f / 180 * XM_PI);
	transform.scale = VECTOR3(1, 1, 1);
	ObjectManager::SetVisible(this, false);
}

ScaleGizmoX::~ScaleGizmoX()
{
}

void ScaleGizmoX::Update()
{
}
ScaleGizmoY::ScaleGizmoY(Object3D* parent)
{
	SetTag("Gizmo");
	pObj.name = "ScaleGizmoY";
	transform.SetParent(parent);
	mesh = new CFbxMesh;
	mesh->Load("Data/Gizmo/ScaleGizmoY.mesh");
	meshCol = new MeshCollider;
	meshCol->MakeFromMesh(mesh);
	//transform.rotation.x = (180.0f / 180 * XM_PI);
	transform.scale = VECTOR3(1, 1, 1);
	ObjectManager::SetVisible(this, false);
}

ScaleGizmoY::~ScaleGizmoY()
{
}

void ScaleGizmoY::Update()
{
}
ScaleGizmoZ::ScaleGizmoZ(Object3D* parent)
{
	SetTag("Gizmo");
	pObj.name = "ScaleGizmoZ";
	transform.SetParent(parent);
	mesh = new CFbxMesh;
	mesh->Load("Data/Gizmo/ScaleGizmoZ.mesh");
	meshCol = new MeshCollider;
	meshCol->MakeFromMesh(mesh);
	transform.rotation.x = (180.0f / 180 * XM_PI);
	transform.scale = VECTOR3(1, 1, 1);
	ObjectManager::SetVisible(this, false);
}

ScaleGizmoZ::~ScaleGizmoZ()
{
}

void ScaleGizmoZ::Update()
{
}