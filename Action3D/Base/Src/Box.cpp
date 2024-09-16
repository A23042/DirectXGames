#include "Box.h"
#include "Player.h"

Box::Box(float x, float y, float z)
{
	SetTag("STAGEOBJ");
	mesh = new CFbxMesh();
	mesh->Load("Data/Object/box00.mesh");

	meshCol = new MeshCollider();
	meshCol->MakeFromMesh(mesh);

	vPos = VECTOR3(x / 2, y / 2, z / 2);

	ten[0] = VECTOR3(transform.position.x + vPos.x, transform.position.y + vPos.y, transform.position.z - vPos.z);
	ten[1] = VECTOR3(transform.position.x + vPos.x, transform.position.y - vPos.y, transform.position.z - vPos.z);
	ten[2] = VECTOR3(transform.position.x - vPos.x, transform.position.y - vPos.y, transform.position.z - vPos.z);
	ten[3] = VECTOR3(transform.position.x - vPos.x, transform.position.y + vPos.y, transform.position.z - vPos.z);
	ten[4] = VECTOR3(transform.position.x + vPos.x, transform.position.y + vPos.y, transform.position.z + vPos.z);
	ten[5] = VECTOR3(transform.position.x + vPos.x, transform.position.y - vPos.y, transform.position.z + vPos.z);
	ten[6] = VECTOR3(transform.position.x - vPos.x, transform.position.y - vPos.y, transform.position.z + vPos.z);
	ten[7] = VECTOR3(transform.position.x - vPos.x, transform.position.y + vPos.y, transform.position.z + vPos.z);

}

void Box::Update()
{

	std::list<Player*> playeres =
		ObjectManager::FindGameObjects<Player>();
	for (Player* player : playeres) {
		CubeSize(vPos.x, vPos.y, vPos.z);
		HitSphereToCubeplane(player);
		transform.position += pushVec;
	}
}

void Box::CubeSize(float x, float y, float z)
{
	// 立方体の各頂点座標
	ten[0] = VECTOR3(transform.position.x + vPos.x, transform.position.y + vPos.y, transform.position.z - vPos.z);
	ten[1] = VECTOR3(transform.position.x + vPos.x, transform.position.y - vPos.y, transform.position.z - vPos.z);
	ten[2] = VECTOR3(transform.position.x - vPos.x, transform.position.y - vPos.y, transform.position.z - vPos.z);
	ten[3] = VECTOR3(transform.position.x - vPos.x, transform.position.y + vPos.y, transform.position.z - vPos.z);
	ten[4] = VECTOR3(transform.position.x + vPos.x, transform.position.y + vPos.y, transform.position.z + vPos.z);
	ten[5] = VECTOR3(transform.position.x + vPos.x, transform.position.y - vPos.y, transform.position.z + vPos.z);
	ten[6] = VECTOR3(transform.position.x - vPos.x, transform.position.y - vPos.y, transform.position.z + vPos.z);
	ten[7] = VECTOR3(transform.position.x - vPos.x, transform.position.y + vPos.y, transform.position.z + vPos.z);
	
	// 各辺の頂点パーツ
	int edgePoint[12][2] = {
		{0, 1}, {1, 2}, {2, 3}, {3, 0},//正面：右、　下、　左、　下
		{0, 4}, {1, 5}, {2, 6}, {3, 7},//側面：右上、右下、左下、左上
		{4, 5}, {5, 6}, {6, 7}, {7, 4} //背面：右、　下、　左、　下、
	};

	// 辺の長さを１で作成
	for (int i = 0; i < 12; i++) {
		//	gpt				点　頂点パーツの左				頂点パーツの右
		edge[i] = normalize(ten[edgePoint[i][1]] - ten[edgePoint[i][0]]);
	}

	int planePoit[6][3] = {
		// 法線ベクトルを求めるための辺ベクトル2つと共通の頂点 
		//　　正面　　　　右　　　　　　左　　　　　後ろ
			{1, 0, 2}, {5, 4, 1}, {2, 3, 6}, {5, 4, 6},
		//　　上　　　　　下
			{3, 0, 7}, {2, 1, 5}
	};

	for (int i = 0; i < 6; i++) {
		VECTOR3 v1 = ten[planePoit[i][0]] - ten[planePoit[i][1]];
		VECTOR3 v2 = ten[planePoit[i][2]] - ten[planePoit[i][1]];
		plane[i] = normalize(cross(v1, v2));

		d[i] = dot(-plane[i], ten[planePoit[i][1]]);
	}
	return ;
}

VECTOR3 Box::HitSphereToCubeplane(Object3D* player)
{
	pushVec = VECTOR3(0, 0, 0);

	// 球の中心点から各頂点へのベクトル
	for (int i = 0; i < 8; i++) {
		pt[i] = player->Position() - ten[i];
	}

	// 球の中心点から辺に垂線を下ろしたときに辺の範囲内にあるかどうか
	// 1~0が範囲内

	int TptPoint[12] = {
		{0}, {1}, {2}, {3},
		{0}, {1}, {2}, {3},
		{4}, {5}, {6}, {7}
	};

	for (int i = 0; i < 12; i++) {
		Tpt[i] = dot(edge[i], pt[TptPoint[i]]) / edge[i].Length();
	}

	// 平面との距離計算

	for (int i = 0; i < 6; i++) {
		distance[i] = abs(dot(plane[i], player->Position()) + d[i]) / plane[i].Length();
	}

	// 面1
	if (distance[0] <= 0.5f) {	// 無限平面に接している
		if (0 <= Tpt[0] && Tpt[0] <= 1 && 0 <= Tpt[1] && Tpt[1] <= 1) {
			//transform.position.z += 0.02f;
			pushVec.z = 0.02;
			return pushVec;
		}
	}
	// 面2
	if (distance[1] <= 0.5f) {
		if (0 <= Tpt[0] && Tpt[0]<= 1 && 0 <= Tpt[4] && Tpt[4] <= 1) {
			//transform.position.x -= 0.02;
			pushVec.x = -0.02f;
			return pushVec;
		}
	}
	// 面3
	if (distance[2] <= 0.5f) {
		if (0 <= Tpt[2] && Tpt[2] <= 1 && 0 <= Tpt[6] && Tpt[6] <= 1) {
			//transform.position.x += 0.02;
			pushVec.x = 0.02f;
			return pushVec;
		}
	}
	// 面4
	if (distance[3] <= 0.5f) {
		if (0 <= Tpt[8] && Tpt[8] <= 1 && 0 <= Tpt[9] && Tpt[9] <= 1) {
			//transform.position.z -= 0.02;
			pushVec.z = -0.02f;
			return pushVec;
		}
	}
	// 面5
	if (distance[4] <= 0.5f) {
		if (0 <= Tpt[3] && Tpt[3] <= 1 && 0 <= Tpt[4] && Tpt[4] <= 1) {
			//transform.position.y -= 0.02;
			pushVec.y = -0.02f;
			return pushVec;
		}
	}
	// 面6
	if (distance[5] <= 0.5f) {
		if (0 <= Tpt[1] && Tpt[1] <= 1 && 0 <= Tpt[5] && Tpt[5] <= 1) {
			//transform.position.y += 0.02;
			pushVec.y = 0.02f;
			return pushVec;
		}
	}
	
	HitSphereToCubeEdge(player);

	return VECTOR3();
}

VECTOR3 Box::HitSphereToCubeEdge(Object3D* player)
{
	int TptPoint[12] = {
		{0}, {1}, {2}, {3},
		{0}, {1}, {2}, {3},
		{4}, {5}, {6}, {7}
	};

	for (int i = 0; i < 12; i++) {
		distanceV[i] = edge[i] * dot(edge[i], pt[TptPoint[i]]) / edge[i].Length() - pt[TptPoint[i]];
	}

	// 辺0
	if (0 <= Tpt[0] && Tpt[0] <= 1) {
		if (distanceV[0].Length() <= 0.5f) {
			//transform.position.z += 0.02f;
			//transform.position.x -= 0.02f;

			pushVec = VECTOR3(-0.02, 0, 0.02);
			return pushVec;
		}
	}
	// 辺1
	if (0 <= Tpt[1] && Tpt[1] <= 1) {
		if (distanceV[1].Length() <= 0.5f) {
			//transform.position.y += 0.02f;
			//transform.position.z += 0.02f;

			pushVec = VECTOR3(0, 0.02, 0.02);
			return pushVec;
		}
	}
	// 辺2
	if (0 <= Tpt[2] && Tpt[2] <= 1) {
		if (distanceV[2].Length() <= 0.5f) {
			//transform.position.z += 0.02f;
			//transform.position.x += 0.02f;

			pushVec = VECTOR3(0.02, 0, 0.02);
			return pushVec;
		}
	}
	// 辺3
	if (0 <= Tpt[3] && Tpt[3] <= 1) {
		if (distanceV[3].Length() <= 0.5f) {
			//transform.position.y -= 0.02f;
			//transform.position.z += 0.02f;

			pushVec = VECTOR3(0, -0.02, 0.02);
			return pushVec;
		}
	}
	// 辺4
	if (0 <= Tpt[4] && Tpt[4] <= 1) {
		if (distanceV[4].Length() <= 0.5f) {
			//transform.position.y -= 0.02f;
			//transform.position.x -= 0.02f;

			pushVec = VECTOR3(-0.02, -0.02, 0);
			return pushVec;
		}
	}
	// 辺5
	if (0 <= Tpt[5] && Tpt[5] <= 1) {
		if (distanceV[5].Length() <= 0.5f) {
			//transform.position.y += 0.02f;
			//transform.position.x -= 0.02f;

			pushVec = VECTOR3(-0.02, 0.02, 0);
			return pushVec;
		}
	}
	// 辺6
	if (0 <= Tpt[6] && Tpt[6] <= 1) {
		if (distanceV[6].Length() <= 0.5f) {
			//transform.position.y += 0.02f;
			//transform.position.x += 0.02f;

			pushVec = VECTOR3(0.02, 0.02, 0);
			return pushVec;
		}
	}
	// 辺7
	if (0 <= Tpt[7] && Tpt[7] <= 1) {
		if (distanceV[7].Length() <= 0.5f) {
			//transform.position.y -= 0.02f;
			//transform.position.x += 0.02f;

			pushVec = VECTOR3(0.02, -0.02, 0);
			return pushVec;
		}
	}
	// 辺8
	if (0 <= Tpt[8] && Tpt[8] <= 1) {
		if (distanceV[8].Length() <= 0.5f) {
			//transform.position.z -= 0.02f;
			//transform.position.x -= 0.02f;

			pushVec = VECTOR3(-0.02, 0, -0.02);
			return pushVec;
		}
	}
	// 辺9
	if (0 <= Tpt[9] && Tpt[9] <= 1) {
		if (distanceV[9].Length() <= 0.5f) {
			//transform.position.y += 0.02f;
			//transform.position.z -= 0.02f;

			pushVec = VECTOR3(0, 0.02, -0.02);
			return pushVec;
		}
	}
	// 辺10
	if (0 <= Tpt[10] && Tpt[10] <= 1) {
		if (distanceV[10].Length() <= 0.5f) {
			//transform.position.z -= 0.02f;
			//transform.position.x += 0.02f;

			pushVec = VECTOR3(0.02, 0, -0.02);
			return pushVec;
		}
	}
	// 辺11
	if (0 <= Tpt[11] && Tpt[11] <= 1) {
		if (distanceV[11].Length() <= 0.5f) {
			//transform.position.y -= 0.02f;
			//transform.position.z -= 0.02f;

			pushVec = VECTOR3(0, -0.02, -0.02);
			return pushVec;
		}
	}
	HitSphereToCubeVertices(player);
	return VECTOR3();
}

VECTOR3 Box::HitSphereToCubeVertices(Object3D* player)
{

	// 頂点0
	if (pt[0].Length() <= 0.5f) {
		pushVec = VECTOR3(-0.02, -0.02, 0.02);
		return pushVec;
	}
	// 頂点1
	if (pt[1].Length() <= 0.5f) {
		pushVec = VECTOR3(-0.02, 0.02, 0.02);
		return pushVec;
	}
	// 頂点2
	if (pt[2].Length() <= 0.5f) {
		pushVec = VECTOR3(0.02, 0.02, 0.02);
		return pushVec;
	}
	// 頂点3
	if (pt[3].Length() <= 0.5f) {
		pushVec = VECTOR3(0.02, -0.02, 0.02);
		return pushVec;
	}
	// 頂点4
	if (pt[4].Length() <= 0.5f) {
		pushVec = VECTOR3(-0.02, -0.02, -0.02);
		return pushVec;
	}
	// 頂点5
	if (pt[5].Length() <= 0.5f) {
		pushVec = VECTOR3(-0.02, 0.02, -0.02);
		return pushVec;
	}
	// 頂点6
	if (pt[6].Length() <= 0.5f) {
		pushVec = VECTOR3(0.02, 0.02, -0.02);
		return pushVec;
	}
	// 頂点7
	if (pt[7].Length() <= 0.5f) {
		pushVec = VECTOR3(0.02, -0.02, -0.02);
		return pushVec;
	}

	return VECTOR3();
}



Box::~Box()
{
	if (mesh != nullptr) {
		delete mesh;
		mesh = nullptr;
	}
}

