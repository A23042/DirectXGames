#include "Box.h"
#include "Player.h"
#include "Ball.h"
#include "OutlineBox.h"
// 動かないBoxオブジェクト

Box::Box(VECTOR3 size, VECTOR3 rot)
{
	SetTag("STAGEOBJ");
	editObj.name = "Box";
	mesh = new CFbxMesh();
	mesh->Load("Data/Object/box00.mesh");
	//mesh->m_vDiffuse.w = 0.5f;

	meshCol = new MeshCollider();
	meshCol->MakeFromMesh(mesh);
	
	vPos = VECTOR3(size.x / 2, size.y / 2, size.z / 2);
	transform.scale = size;

	// 回転角をラジアンに変換し、回転行列を作成
	transform.rotation.x += rot.x / 180.0f * XM_PI;
	transform.rotation.y += rot.y / 180.0f * XM_PI;
	transform.rotation.z += rot.z / 180.0f * XM_PI;

	rotationMatrix = XMMatrixRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z);

	pushVec = VECTOR3(0, 0, 0);
	HitPoint = VECTOR3(0, 0, 0);
	refVec = VECTOR3(0, 0, 0);

	pObj.center = transform.position;
	
	spr = new CSprite;
	child = new OutlineBox(this);
	ObjectManager::AddObj(this);
}

void Box::Start()
{
	pObj.center = transform.position;
	vPos = transform.scale / 2;
	CubeSize(vPos.x, vPos.y, vPos.z);
	isStart = true;
}

void Box::Update()
{
	transform.position = pObj.center;
	
	// 衝突判定の関数呼び出しはそれぞれのクラスで行う
}

void Box::Draw()
{
	mesh->Render(transform.matrix());
	if (meshCol != nullptr)
	{
		//meshCol->DrawPolygons(transform.matrix());
	}
#if 1
	// 各辺の頂点パーツ
	int edgePoint[12][2] = {
		{0, 1}, {1, 2}, {2, 3}, {3, 0},//正面：右、　下、　左、　下
		{0, 4}, {1, 5}, {2, 6}, {3, 7},//側面：右上、右下、左下、左上
		{4, 5}, {5, 6}, {6, 7}, {7, 4} //背面：右、　下、　左、　下、
	};

	vPos = transform.scale / 2;
	CubeSize(vPos.x, vPos.y, vPos.z);
	for (int i = 0; i < 12; i++)
	{
		spr->DrawLine3D(vertex[edgePoint[i][1]], vertex[edgePoint[i][0]], RGB(0, 0, 0), 1.0f);
	}

#endif
}

Box::~Box()
{
	SAFE_DELETE(spr);
	SAFE_DELETE(mesh);
	SAFE_DELETE(meshCol);

	if(child != nullptr)
	{
		child->DestroyMe();
	}
	ObjectManager::RemoveObj(this);
}


