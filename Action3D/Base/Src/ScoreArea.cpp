#include "ScoreArea.h"

ScoreArea::ScoreArea()
{
}

ScoreArea::~ScoreArea()
{
}

void ScoreArea::Update()
{
	transform.position = pObj.center;
	vPos = transform.scale / 2;

	// 立方体の各頂点座標
	vertex[0] = VECTOR3(vPos.x, vPos.y, -vPos.z);
	vertex[1] = VECTOR3(vPos.x, -vPos.y, -vPos.z);
	vertex[2] = VECTOR3(-vPos.x, -vPos.y, -vPos.z);
	vertex[3] = VECTOR3(-vPos.x, vPos.y, -vPos.z);
	vertex[4] = VECTOR3(vPos.x, vPos.y, vPos.z);
	vertex[5] = VECTOR3(vPos.x, -vPos.y, vPos.z);
	vertex[6] = VECTOR3(-vPos.x, -vPos.y, vPos.z);
	vertex[7] = VECTOR3(-vPos.x, vPos.y, vPos.z);


	// 回転を顧慮する
	rotationMatrix = XMMatrixRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z);

	// 各頂点に回転行列を掛ける
	for (int i = 0; i < 8; i++)
	{
		vertex[i] *= rotationMatrix;
	}

	// transform.positionを各頂点に加算
	for (int i = 0; i < 8; i++)
	{
		vertex[i] += transform.position;
	}

	min = vertex[0];
	max = vertex[0];

	// AABBの最小点、最大点を求める
	//for (int i = 0; i < sizeof(vertex); i++)
	for (int i = 0; i < 8; i++)
	{
		max.x = fmax(max.x, vertex[i].x);
		max.y = fmax(max.y, vertex[i].y);
		max.z = fmax(max.z, vertex[i].z);

		min.x = fmin(min.x, vertex[i].x);
		min.y = fmin(min.y, vertex[i].y);
		min.z = fmin(min.z, vertex[i].z);
	}

}

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

		for (int i = 0; i < 12; i++)
		{
			spr->DrawLine3D(vertex[edgePoint[i][1]], vertex[edgePoint[i][0]], RGB(0, 255, 50), 1.0f);
		}
	}
}

bool ScoreArea::CheckSphereAABBCollision(PhysicsObject& tObj)
{
	if (!isStart)
	{
		Start();
	}

	// 球体の中心から最も近いAABBの頂点を取得する
	float x = std::fmax(min.x, std::fmin(tObj.center.x, max.x));
	float y = std::fmax(min.y, std::fmin(tObj.center.y, max.y));
	float z = std::fmax(min.z, std::fmin(tObj.center.z, max.z));

	// AABBと球体のもっとも近い点との距離計算
	VECTOR3 closest = VECTOR3(x, y, z);
	float distance = sqrt(
		(closest.x - tObj.center.x) * (closest.x - tObj.center.x) +
		(closest.y - tObj.center.y) * (closest.y - tObj.center.y) +
		(closest.z - tObj.center.z) * (closest.z - tObj.center.z));

	// 範囲内であればtrue
	return distance <= tObj.radius;
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
}

ScoreArea1::~ScoreArea1()
{
}

void ScoreArea1::Update()
{
	transform.position = pObj.center;
	vPos = transform.scale / 2;
	CubeSize(vPos.x, vPos.y, vPos.z);
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
}

ScoreArea2::~ScoreArea2()
{
}
void ScoreArea2::Update()
{
	transform.position = pObj.center;
	vPos = transform.scale / 2;
	CubeSize(vPos.x, vPos.y, vPos.z);

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
}

ScoreArea3::~ScoreArea3()
{
}

void ScoreArea3::Update()
{
	transform.position = pObj.center;
	vPos = transform.scale / 2;
	CubeSize(vPos.x, vPos.y, vPos.z);

}

