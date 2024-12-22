#include "ScoreArea.h"

ScoreArea::ScoreArea()
{
}

ScoreArea::~ScoreArea()
{
	SAFE_DELETE(spr);
}

ScoreArea1::ScoreArea1(VECTOR3 size, VECTOR3 rot)
{
	SetTag("SCOREAREA");
	editObj.name = "scoreArea1";
	mesh = new CFbxMesh;
	mesh->Load("Data/Gizmo/Area1.mesh");
	meshCol = new MeshCollider;
	meshCol->MakeFromMesh(mesh);

	vPos = size / 2;
	transform.scale = size;

	// 回転角をラジアンに変換し、回転行列を作成
	transform.rotation.x += rot.x / 180.0f * XM_PI;
	transform.rotation.y += rot.y / 180.0f * XM_PI;
	transform.rotation.z += rot.z / 180.0f * XM_PI;

	rotationMatrix = XMMatrixRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z);

	SetDrawOrder(-1);
	ObjectManager::AddScArea(this);
}

ScoreArea1::~ScoreArea1()
{
	SAFE_DELETE(spr);
	SAFE_DELETE(mesh);
	SAFE_DELETE(meshCol);
	ObjectManager::RemoveArea(this);
}

ScoreArea2::ScoreArea2(VECTOR3 size, VECTOR3 rot)
{
	SetTag("SCOREAREA");
	editObj.name = "scoreArea2";
	mesh = new CFbxMesh;
	mesh->Load("Data/Gizmo/Area2.mesh");
	meshCol = new MeshCollider;
	meshCol->MakeFromMesh(mesh);

	vPos = size / 2;
	transform.scale = size;

	// 回転角をラジアンに変換し、回転行列を作成
	transform.rotation.x += rot.x / 180.0f * XM_PI;
	transform.rotation.y += rot.y / 180.0f * XM_PI;
	transform.rotation.z += rot.z / 180.0f * XM_PI;

	rotationMatrix = XMMatrixRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z);

	SetDrawOrder(-1);
	ObjectManager::AddScArea(this);
}

ScoreArea2::~ScoreArea2()
{
	SAFE_DELETE(spr);
	SAFE_DELETE(mesh);
	SAFE_DELETE(meshCol);
	ObjectManager::RemoveArea(this);
}

ScoreArea3::ScoreArea3(VECTOR3 size, VECTOR3 rot)
{
	SetTag("SCOREAREA");
	editObj.name = "scoreArea3";
	mesh = new CFbxMesh;
	mesh->Load("Data/Gizmo/Area3.mesh");
	meshCol = new MeshCollider;
	meshCol->MakeFromMesh(mesh);

	vPos = size / 2;
	transform.scale = size;

	// 回転角をラジアンに変換し、回転行列を作成
	transform.rotation.x += rot.x / 180.0f * XM_PI;
	transform.rotation.y += rot.y / 180.0f * XM_PI;
	transform.rotation.z += rot.z / 180.0f * XM_PI;

	rotationMatrix = XMMatrixRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z);

	SetDrawOrder(-1);
	ObjectManager::AddScArea(this);
}

ScoreArea3::~ScoreArea3()
{
	SAFE_DELETE(spr);
	SAFE_DELETE(mesh);
	SAFE_DELETE(meshCol);
	ObjectManager::RemoveArea(this);
}

