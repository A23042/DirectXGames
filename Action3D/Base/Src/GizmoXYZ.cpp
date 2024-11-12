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
	if (mesh != nullptr)
	{
		delete mesh;
		mesh = nullptr;
	}
	if (meshCol != nullptr)
	{
		delete meshCol;
		meshCol = nullptr;
	}
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
	transform.rotation.z = (180.0f / 180 * XM_PI);
	ObjectManager::SetVisible(this, false);
}

PosGizmoX::~PosGizmoX()
{
	if (mesh != nullptr)
	{
		delete mesh;
		mesh = nullptr;
	}
	if (meshCol != nullptr)
	{
		delete meshCol;
		meshCol = nullptr;
	}
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
	if (mesh != nullptr)
	{
		delete mesh;
		mesh = nullptr;
	}
	if (meshCol != nullptr)
	{
		delete meshCol;
		meshCol = nullptr;
	}
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
	transform.rotation.x = (180.0f / 180 * XM_PI);
	ObjectManager::SetVisible(this, false);
}

PosGizmoZ::~PosGizmoZ()
{
	if (mesh != nullptr)
	{
		delete mesh;
		mesh = nullptr;
	}
	if (meshCol != nullptr)
	{
		delete meshCol;
		meshCol = nullptr;
	}
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
	if (mesh != nullptr)
	{
		delete mesh;
		mesh = nullptr;
	}
	if (meshCol != nullptr)
	{
		delete meshCol;
		meshCol = nullptr;
	}
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
	transform.rotation.x = (180.0f / 180 * XM_PI);
	ObjectManager::SetVisible(this, false);
}

RotGizmoX::~RotGizmoX()
{
	if (mesh != nullptr)
	{
		delete mesh;
		mesh = nullptr;
	}
	if (meshCol != nullptr)
	{
		delete meshCol;
		meshCol = nullptr;
	}
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
	transform.rotation.x = (180.0f / 180 * XM_PI);
	ObjectManager::SetVisible(this, false);
}

RotGizmoY::~RotGizmoY()
{
	if (mesh != nullptr)
	{
		delete mesh;
		mesh = nullptr;
	}
	if (meshCol != nullptr)
	{
		delete meshCol;
		meshCol = nullptr;
	}
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
	transform.rotation.x = (180.0f / 180 * XM_PI);
	ObjectManager::SetVisible(this, false);
}

RotGizmoZ::~RotGizmoZ()
{
	if (mesh != nullptr)
	{
		delete mesh;
		mesh = nullptr;
	}
	if (meshCol != nullptr)
	{
		delete meshCol;
		meshCol = nullptr;
	}
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
	transform.rotation.z = (180.0f / 180 * XM_PI);
	ObjectManager::SetVisible(this, false);
}

ScaleGizmoX::~ScaleGizmoX()
{
	if (mesh != nullptr)
	{
		delete mesh;
		mesh = nullptr;
	}
	if (meshCol != nullptr)
	{
		delete meshCol;
		meshCol = nullptr;
	}
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
	if (mesh != nullptr)
	{
		delete mesh;
		mesh = nullptr;
	}
	if (meshCol != nullptr)
	{
		delete meshCol;
		meshCol = nullptr;
	}
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
	if (mesh != nullptr)
	{
		delete mesh;
		mesh = nullptr;
	}
	if (meshCol != nullptr)
	{
		delete meshCol;
		meshCol = nullptr;
	}
}

void ScaleGizmoZ::Update()
{
}