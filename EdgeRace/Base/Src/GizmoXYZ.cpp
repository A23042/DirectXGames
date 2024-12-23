#include "GizmoXYZ.h"

GizmoXYZ::GizmoXYZ()
{
	SetTag("Gizmo");
	editObj.name = "gizmoCenter";
	mesh = new CFbxMesh;
	mesh->Load("Data/Gizmo/gizmoCenter.mesh");
	meshCol = new MeshCollider;
	meshCol->MakeFromMesh(mesh);
	transform.scale = VECTOR3(1, 1, 1);
	ObjectManager::SetVisible(this,false);
}

GizmoXYZ::~GizmoXYZ()
{
	SAFE_DELETE(mesh);
	SAFE_DELETE(meshCol);
}

void GizmoXYZ::Update()
{
}

PosGizmoX::PosGizmoX(Object3D* parent)
{
	SetTag("Gizmo");
	editObj.name = "posGizmoX";
	transform.SetParent(parent);
	mesh = new CFbxMesh;
	mesh->Load("Data/Gizmo/PosGizmoX.mesh");
	meshCol = new MeshCollider;
	meshCol->MakeFromMesh(mesh);
	ObjectManager::SetVisible(this, false);
}

PosGizmoX::~PosGizmoX()
{
	SAFE_DELETE(mesh);
	SAFE_DELETE(meshCol);
}

void PosGizmoX::Update()
{
}

PosGizmoY::PosGizmoY(Object3D* parent)
{
	SetTag("Gizmo");
	editObj.name = "posGizmoY";
	transform.SetParent(parent);
	mesh = new CFbxMesh;
	mesh->Load("Data/Gizmo/PosGizmoY.mesh");
	meshCol = new MeshCollider;
	meshCol->MakeFromMesh(mesh);
	ObjectManager::SetVisible(this, false);
}

PosGizmoY::~PosGizmoY()
{
	SAFE_DELETE(mesh);
	SAFE_DELETE(meshCol);
}

void PosGizmoY::Update()
{
}

PosGizmoZ::PosGizmoZ(Object3D* parent)
{
	SetTag("Gizmo");
	editObj.name = "posGizmoZ";
	transform.SetParent(parent);
	mesh = new CFbxMesh;
	mesh->Load("Data/Gizmo/PosGizmoZ.mesh");
	meshCol = new MeshCollider;
	meshCol->MakeFromMesh(mesh);
	ObjectManager::SetVisible(this, false);
}

PosGizmoZ::~PosGizmoZ()
{
	SAFE_DELETE(mesh);
	SAFE_DELETE(meshCol);
}

void PosGizmoZ::Update()
{
}

RotGizmoC::RotGizmoC(Object3D* parent)
{
	SetTag("Gizmo");
	editObj.name = "rotGizmoC";
	transform.SetParent(parent);
	mesh = new CFbxMesh;
	mesh->Load("Data/Gizmo/RotGizmoCenter.mesh");
	meshCol = new MeshCollider;
	meshCol->MakeFromMesh(mesh);
	ObjectManager::SetVisible(this, false);
}

RotGizmoC::~RotGizmoC()
{
	SAFE_DELETE(mesh);
	SAFE_DELETE(meshCol);
}

void RotGizmoC::Update()
{
}
RotGizmoX::RotGizmoX(Object3D* parent)
{
	SetTag("Gizmo");
	editObj.name = "rotGizmoX";
	transform.SetParent(parent);
	mesh = new CFbxMesh;
	mesh->Load("Data/Gizmo/RotGizmoX.mesh");
	meshCol = new MeshCollider;
	meshCol->MakeFromMesh(mesh);
	ObjectManager::SetVisible(this, false);
}

RotGizmoX::~RotGizmoX()
{
	SAFE_DELETE(mesh);
	SAFE_DELETE(meshCol);
}

void RotGizmoX::Update()
{
}
RotGizmoY::RotGizmoY(Object3D* parent)
{
	SetTag("Gizmo");
	editObj.name = "rotGizmoY";
	transform.SetParent(parent);
	mesh = new CFbxMesh;
	mesh->Load("Data/Gizmo/RotGizmoY.mesh");
	meshCol = new MeshCollider;
	meshCol->MakeFromMesh(mesh);
	ObjectManager::SetVisible(this, false);
}

RotGizmoY::~RotGizmoY()
{
	SAFE_DELETE(mesh);
	SAFE_DELETE(meshCol);
}

void RotGizmoY::Update()
{
}
RotGizmoZ::RotGizmoZ(Object3D* parent)
{
	SetTag("Gizmo");
	editObj.name = "rotGizmoZ";
	transform.SetParent(parent);
	mesh = new CFbxMesh;
	mesh->Load("Data/Gizmo/RotGizmoZ.mesh");
	meshCol = new MeshCollider;
	meshCol->MakeFromMesh(mesh);
	ObjectManager::SetVisible(this, false);
}

RotGizmoZ::~RotGizmoZ()
{
	SAFE_DELETE(mesh);
	SAFE_DELETE(meshCol);
}

void RotGizmoZ::Update()
{
}

ScaleGizmoX::ScaleGizmoX(Object3D* parent)
{
	SetTag("Gizmo");
	editObj.name = "scaleGizmoX";
	transform.SetParent(parent);
	mesh = new CFbxMesh;
	mesh->Load("Data/Gizmo/ScaleGizmoX.mesh");
	meshCol = new MeshCollider;
	meshCol->MakeFromMesh(mesh);
	ObjectManager::SetVisible(this, false);
}

ScaleGizmoX::~ScaleGizmoX()
{
	SAFE_DELETE(mesh);
	SAFE_DELETE(meshCol);
}

void ScaleGizmoX::Update()
{
}
ScaleGizmoY::ScaleGizmoY(Object3D* parent)
{
	SetTag("Gizmo");
	editObj.name = "scaleGizmoY";
	transform.SetParent(parent);
	mesh = new CFbxMesh;
	mesh->Load("Data/Gizmo/ScaleGizmoY.mesh");
	meshCol = new MeshCollider;
	meshCol->MakeFromMesh(mesh);
	ObjectManager::SetVisible(this, false);
}

ScaleGizmoY::~ScaleGizmoY()
{
	SAFE_DELETE(mesh);
	SAFE_DELETE(meshCol);
}

void ScaleGizmoY::Update()
{
}
ScaleGizmoZ::ScaleGizmoZ(Object3D* parent)
{
	SetTag("Gizmo");
	editObj.name = "scaleGizmoZ";
	transform.SetParent(parent);
	mesh = new CFbxMesh;
	mesh->Load("Data/Gizmo/ScaleGizmoZ.mesh");
	meshCol = new MeshCollider;
	meshCol->MakeFromMesh(mesh);
	transform.rotation.x = (180.0f / 180 * XM_PI);
	ObjectManager::SetVisible(this, false);
}

ScaleGizmoZ::~ScaleGizmoZ()
{
	SAFE_DELETE(mesh);
	SAFE_DELETE(meshCol);
}

void ScaleGizmoZ::Update()
{
}