#include "ScoreArea.h"

ScoreArea::ScoreArea()
{
}

ScoreArea::~ScoreArea()
{
}

#if 0
void ScoreArea::Draw()
{
	mesh->Render(transform.matrix());

	// 各辺の頂点パーツ
	int edgePoint[12][2] = {
		{0, 1}, {1, 2}, {2, 3}, {3, 0},//正面：右、　下、　左、　下
		{0, 4}, {1, 5}, {2, 6}, {3, 7},//側面：右上、右下、左下、左上
		{4, 5}, {5, 6}, {6, 7}, {7, 4} //背面：右、　下、　左、　下、
	};

	// 選択されている場合自身のアウトライを表示させる
	if (editObj.isSelect)
	{
		vPos = transform.scale / 2;
		CubeSize(vPos.x, vPos.y, vPos.z);
		if (spr == nullptr) spr = new CSprite;
		for (int i = 0; i < 12; i++)
		{
			spr->DrawLine3D(vertex[edgePoint[i][1]], vertex[edgePoint[i][0]], RGB(0, 255, 50), 1.0f);
		}
	}
}
#endif

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
	ObjectManager::RemoveArea(this);
}

