#include "Box.h"

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
		HitSphereToCubeplane(player->sphere);
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
			{2, 0, 1}, {1, 4, 5}, {6, 3, 2}, {5, 4, 6},
		//　　上　　　　　下
			{7, 0, 3}, {2, 1, 5}
	};

	for (int i = 0; i < 6; i++) {
		VECTOR3 v1 = ten[planePoit[i][0]] - ten[planePoit[i][1]];
		VECTOR3 v2 = ten[planePoit[i][2]] - ten[planePoit[i][1]];
		plane[i] = normalize(cross(v1, v2));

		d[i] = dot(-plane[i], ten[planePoit[i][1]]);
	}
	return ;
}

VECTOR3 Box::HitSphereToCubeplane(Player::Sphere sphere)
{
	pushVec = VECTOR3(0, 0, 0);

	// 球の中心点から各頂点へのベクトル
	for (int i = 0; i < 8; i++) {
		pt[i] = sphere.center - ten[i];
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
		distance[i] = abs(dot(plane[i], sphere.center) + d[i]) / plane[i].Length();
	}

	int pair[6][2] = {
		{0, 1}, {0, 4}, {2, 6},
		{8, 9}, {3, 4}, {1, 5}
	};

	for (int i = 0; i < 6; i++) {
		if (distance[i] < 0.5f) {
			if (Tpt[pair[i][0]] >= 0 && Tpt[pair[i][0]] <= 1 && Tpt[pair[i][1]] >= 0 && Tpt[pair[i][1]] <= 1) {
				// ここに当たった面の法線ベクトルを書く
				ReflectionVec(sphere, plane[i]);
				pushVec = plane[i] * (sphere.radius - distance[i]);
				return pushVec;
			}
		}
	}
	HitSphereToCubeEdge(sphere);
}

VECTOR3 Box::HitSphereToCubeEdge(Player::Sphere sphere)
{
	int TptPoint[12] = {
		{0}, {1}, {2}, {3},
		{0}, {1}, {2}, {3},
		{4}, {5}, {6}, {7}
	};

	// 辺と球との距離計算
	for (int i = 0; i < 12; i++) {
		distanceV[i] = edge[i] * dot(edge[i], pt[TptPoint[i]]) / edge[i].Length() - pt[TptPoint[i]];
	}

	int pair[12][2] = {
		{0, 1}, {0, 5}, {0, 2}, {0, 4},
		{4, 1}, {1, 5}, {5, 2}, {2, 4},
		{3, 1}, {3, 5}, {3, 2}, {3, 4}
	};

	for (int i = 0; i < 12; i++) {
		if (0 <= Tpt[i] && Tpt[i] <= 1) {
			if (distanceV[i].Length() < 0.5f) {
				VECTOR3 vNormal = (plane[pair[i][0]] + plane[pair[i][1]]) / 2;
				ReflectionVec(sphere, normalize(vNormal));
				pushVec = normalize(vNormal) * (sphere.radius - distanceV[i].Length());
				return pushVec;
			}
		}
	}
	
	HitSphereToCubeVertices(sphere);
	return VECTOR3();
}

VECTOR3 Box::HitSphereToCubeVertices(Player::Sphere sphere)
{
	for (int i = 0; i < 8; i++) {
		if (pt[i].Length() < 0.5f) {
			ReflectionVec(sphere, normalize(pt[i]));
			pushVec = normalize(ten[i] - sphere.center) * (sphere.radius - pt[i].Length());
			return pushVec;
		}
	}

	return VECTOR3();
}

// 反射させるための関数
// 当たったときの法線ベクトルを受け取り跳ね返りベクトルを計算する
VECTOR3 Box::ReflectionVec(Player::Sphere sphere, VECTOR3 normal)
{
	float ip = dot(sphere.velocity, normal);
	VECTOR3 a = ip * normal * 2;
	VECTOR3 b = sphere.velocity - a;
	sphere.velocity = b;
	return VECTOR3();
}



Box::~Box()
{
	if (mesh != nullptr) {
		delete mesh;
		mesh = nullptr;
	}
}

