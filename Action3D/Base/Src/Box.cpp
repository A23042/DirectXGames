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

	pushVec = VECTOR3(0, 0, 0);
	HitPoint = VECTOR3(0, 0, 0);
}

void Box::Update()
{

	std::list<Player*> playeres =
		ObjectManager::FindGameObjects<Player>();
	for (Player* player : playeres) {
		CubeSize(vPos.x, vPos.y, vPos.z);		// 直方体のサイズと位置
		HitSphereToCubeplane(player->sphere);	// 面->辺->頂点の衝突判定
		player->PushVec(-pushVec);	// プレイヤーをめり込んだ量だけもどす

		ImGui::Begin("HitPoint");
		ImGui::InputFloat("X", &HitPoint.x);
		ImGui::InputFloat("Y", &HitPoint.y);
		ImGui::InputFloat("Z", &HitPoint.z);
		ImGui::End();
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

	// 面の法線と定数の計算
	for (int i = 0; i < 6; i++) {
		VECTOR3 v1 = ten[planePoit[i][0]] - ten[planePoit[i][1]];
		VECTOR3 v2 = ten[planePoit[i][2]] - ten[planePoit[i][1]];
		plane[i] = normalize(cross(v1, v2));	// 平面の法線	
		d[i] = dot(-plane[i], ten[planePoit[i][1]]);// 平面の定数
	}
	return ;
}

// 面との衝突
VECTOR3 Box::HitSphereToCubeplane(Sphere& sphere)
{
	pushVec = VECTOR3(0, 0, 0);

	// 球の中心点から各頂点へのベクトル
	for (int i = 0; i < 8; i++) {
		pt[i] = sphere.center - ten[i];
	}

	int TptPoint[12] = {
		{0}, {1}, {2}, {3},
		{0}, {1}, {2}, {3},
		{4}, {5}, {6}, {7}
	};
	// 球の中心点から辺に垂線を下ろしたときに辺の範囲内にあるかどうか
	// 1~0が範囲内
	for (int i = 0; i < 12; i++) {
		Tpt[i] = dot(edge[i], pt[TptPoint[i]]) / edge[i].Length();
	}

	int pair[6][2] = {
		{0, 1}, {0, 4}, {2, 6},
		{8, 9}, {3, 4}, {1, 5}
	};

	// 平面との距離計算
	for (int i = 0; i < 6; i++) {
		distance[i] = abs(dot(plane[i], sphere.center) + d[i]) / plane[i].Length();

		// 衝突していたらあとの距離計算を省く
		if (distance[i] < sphere.radius) {
			// 無限平面に衝突していたら辺に垂線を下ろせるか
			if (Tpt[pair[i][0]] >= 0 && Tpt[pair[i][0]] <= 1 && Tpt[pair[i][1]] >= 0 && Tpt[pair[i][1]] <= 1) {
				HitPoint = sphere.center - plane[i] * distance[i];	// 衝突点
				ReflectionVec(sphere, plane[i]);	// 球体を反射させる
				pushVec = plane[i] * (sphere.radius - distance[i]);	// めり込みを解除するための計算
				return pushVec;
			}
		}
	}

	HitSphereToCubeEdge(sphere);
}

// 辺との衝突
VECTOR3 Box::HitSphereToCubeEdge(Sphere& sphere)
{
	int TptPoint[12] = {
		{0}, {1}, {2}, {3},
		{0}, {1}, {2}, {3},
		{4}, {5}, {6}, {7}
	};

	int pair[12][2] = {
		{0, 1}, {0, 5}, {0, 2}, {0, 4},
		{4, 1}, {1, 5}, {5, 2}, {2, 4},
		{3, 1}, {3, 5}, {3, 2}, {3, 4}
	};

	// 辺と球との距離計算
	for (int i = 0; i < 12; i++) {
		distanceV[i] = edge[i] * dot(edge[i], pt[TptPoint[i]]) / edge[i].Length() - pt[TptPoint[i]];
		if (distanceV[i].Length() < sphere.radius) {
			//　垂線をおろせるか
			if (0 <= Tpt[i] && Tpt[i] <= 1) {
				//VECTOR3 vNormal = normalize(plane[pair[i][0]] + plane[pair[i][1]]) / 2;	// 辺の法線ベクトル
				VECTOR3 vNormal = normalize(distanceV[i]);	// 辺の法線ベクトル

				HitPoint = sphere.center - vNormal * distanceV[i].Length();	// 衝突点
				ReflectionVec(sphere, vNormal);
				pushVec = vNormal * (sphere.radius - distanceV[i].Length());
				return pushVec;
			}
		}
	}
	
	HitSphereToCubeVertices(sphere);
	return VECTOR3();
}

// 頂点との衝突
VECTOR3 Box::HitSphereToCubeVertices(Sphere& sphere)
{
	for (int i = 0; i < 8; i++) {
		if (pt[i].Length() < sphere.radius) {
			ReflectionVec(sphere, normalize(pt[i]));
			pushVec = normalize(ten[i] - sphere.center) * (sphere.radius - pt[i].Length());
			return pushVec;
		}
	}

	return VECTOR3();
}

// 反射させるための関数(未実装)
// 当たったときの法線ベクトルを受け取り跳ね返りベクトルを計算する
VECTOR3 Box::ReflectionVec(Sphere& sphere, VECTOR3 normal)
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

