#include "Box.h"
#include "Player.h"
#include "Ball.h"
// 動かないBoxオブジェクト

// 初期化リスト　生成と同時に初期値が入るコンストラクタに書くよりも高速
Box::Box(VECTOR3 size, VECTOR3 rot) : vPos(size / 2)
{
	SetTag("STAGEOBJ");
	editObj.name = "Box";
	mesh = new CFbxMesh();
	mesh->Load("Data/Object/box00.mesh");

	meshCol = new MeshCollider();
	meshCol->MakeFromMesh(mesh);
	
	//vPos = VECTOR3(size.x / 2, size.y / 2, size.z / 2);
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
}

void Box::Start()
{
	pObj.center = transform.position;
	//CubeSize(vPos.x, vPos.y, vPos.z);
}

void Box::Update()
{
	transform.position = pObj.center;
	vPos = transform.scale / 2;
	CubeSize(vPos.x, vPos.y, vPos.z);

	// 衝突判定の関数呼び出しはそれぞれのクラスで行う
#if 0
	// プレイヤーとの衝突判定
	std::list<Player*> playeres =
		ObjectManager::FindGameObjects<Player>();
	for (Player* player : playeres) {
		refVec = VECTOR3(0, 0, 0);
		CubeSize(vPos.x, vPos.y, vPos.z);		// 直方体のサイズと位置
		pushVec = HitSphereToCubeplane(player->sphere, refVec);	// 面->辺->頂点の衝突判定
		player->PushVec(-pushVec, refVec);	// プレイヤーをめり込んだ量だけもどす
	}
	// Ballとの衝突判定
	std::list<Ball*> balls = ObjectManager::FindGameObjects<Ball>();
	for (Ball* ball : balls) {
		refVec = VECTOR3(0, 0, 0);
		CubeSize(vPos.x, vPos.y, vPos.z);		// 直方体のサイズと位置
		pushVec = HitSphereToCubeplane(ball->sphere, refVec);	// 面->辺->頂点の衝突判定
		ball->PushVec(-pushVec, refVec);	// プレイヤーをめり込んだ量だけもどす
	}
#endif
}

void Box::Draw()
{
	mesh->Render(transform.matrix());

	// 選択されている場合自身のアウトライを表示させる
	if(editObj.isSelect)
	{
		// 各辺の頂点パーツ
		int edgePoint[12][2] = {
			{0, 1}, {1, 2}, {2, 3}, {3, 0},//正面：右、　下、　左、　下
			{0, 4}, {1, 5}, {2, 6}, {3, 7},//側面：右上、右下、左下、左上
			{4, 5}, {5, 6}, {6, 7}, {7, 4} //背面：右、　下、　左、　下、
		};

		// 辺ベクトル作成
		for (int i = 0; i < 12; i++) {
			edge[i] = ten[edgePoint[i][1]] - ten[edgePoint[i][0]];
			spr->DrawLine3D(ten[edgePoint[i][1]], ten[edgePoint[i][0]], RGB(0, 255, 50), 1.0f);
		}
	}
}

void Box::CubeSize(float x, float y, float z)
{
	// 立方体の各頂点座標
	ten[0] = VECTOR3(x, y, -z);
	ten[1] = VECTOR3(x, -y, -z);
	ten[2] = VECTOR3(-x, -y, -z);
	ten[3] = VECTOR3(-x, y, -z);
	ten[4] = VECTOR3(x, y, z);
	ten[5] = VECTOR3(x, -y, z);
	ten[6] = VECTOR3(-x, -y, z);
	ten[7] = VECTOR3(-x, y, z);

	// 回転させるテスト
	//transform.rotation.y += 1.0f / 180.0f * XM_PI;
	
	// 回転を顧慮する
	rotationMatrix = XMMatrixRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z);

	// 各頂点に回転行列を掛ける
	for (int i = 0; i < 8; i++) {
		ten[i] *= rotationMatrix;
	}

	// transform.positionを各頂点に加算
	for (int i = 0; i < 8; i++) {
		ten[i] += transform.position;
	}
	
	// 各辺の頂点パーツ
	int edgePoint[12][2] = {
		{0, 1}, {1, 2}, {2, 3}, {3, 0},//正面：右、　下、　左、　下
		{0, 4}, {1, 5}, {2, 6}, {3, 7},//側面：右上、右下、左下、左上
		{4, 5}, {5, 6}, {6, 7}, {7, 4} //背面：右、　下、　左、　下、
	};

	
	// 辺ベクトル作成
	for (int i = 0; i < 12; i++) {
		edge[i] = ten[edgePoint[i][1]] - ten[edgePoint[i][0]];
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
VECTOR3 Box::HitSphereToCubeplane(PhysicsObject& tObj, VECTOR3 &refVec)
{
	pushVec = VECTOR3(0, 0, 0);

	// 球の中心点から各頂点へのベクトル
	for (int i = 0; i < 8; i++) {
		pt[i] = tObj.center - ten[i];
	}

	int TptPoint[12] = {
		{0}, {1}, {2}, {3},
		{0}, {1}, {2}, {3},
		{4}, {5}, {6}, {7}
	};
	// 球の中心点から辺に垂線を下ろしたときに辺の範囲内にあるかどうか
	// 1~0が範囲内
	for (int i = 0; i < 12; i++) {
		Tpt[i] = dot(edge[i], pt[TptPoint[i]]) / edge[i].LengthSquare();
	}

	int pair[6][2] = {
		{0, 1}, {0, 4}, {2, 6},
		{8, 9}, {3, 4}, {1, 5}
	};

	// 平面との距離計算
	for (int i = 0; i < 6; i++) {
		distance[i] = abs(dot(plane[i], tObj.center) + d[i]) / plane[i].Length();

		// 衝突していたらあとの距離計算を省く
		if (distance[i] <= tObj.radius) {
			// 無限平面に衝突していたら辺に垂線を下ろせるか
			if (Tpt[pair[i][0]] >= 0 && Tpt[pair[i][0]] <= 1 && Tpt[pair[i][1]] >= 0 && Tpt[pair[i][1]] <= 1) {
				HitPoint = tObj.center - plane[i] * distance[i];	// 衝突点
				refVec = ReflectionVec(tObj, plane[i]);	// 球体を反射させる
				pushVec = plane[i] * (tObj.radius - distance[i]);	// めり込みを解除するための計算
				return pushVec;	// めり込み解除の量ベクトル
			}
		}
	}

	HitSphereToCubeEdge(tObj, refVec);
}

// 辺との衝突
VECTOR3 Box::HitSphereToCubeEdge(PhysicsObject& tObj, VECTOR3& refVec)
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
		distanceV[i] = edge[i] * dot(edge[i], pt[TptPoint[i]]) / edge[i].LengthSquare() - pt[TptPoint[i]];
		if (distanceV[i].Length() <= tObj.radius) {
			//　垂線をおろせるか
			if (0 <= Tpt[i] && Tpt[i] <= 1) {
				//VECTOR3 vNormal = normalize(plane[pair[i][0]] + plane[pair[i][1]]) / 2;	// 辺の法線ベクトル
				VECTOR3 vNormal = normalize(distanceV[i]);	// 辺の法線ベクトル

				HitPoint = tObj.center - vNormal * distanceV[i].Length();	// 衝突点
				refVec = ReflectionVec(tObj, vNormal);
				pushVec = vNormal * (tObj.radius - distanceV[i].Length());
				return pushVec;
			}
		}
	}
	
	HitSphereToCubeVertices(tObj, refVec);
	return VECTOR3();
}

// 頂点との衝突
VECTOR3 Box::HitSphereToCubeVertices(PhysicsObject& tObj, VECTOR3& refVec)
{
	for (int i = 0; i < 8; i++) {
		if (pt[i].Length() < tObj.radius) {
			refVec = ReflectionVec(tObj, normalize(pt[i]));
			pushVec = normalize(ten[i] - tObj.center) * (tObj.radius - pt[i].Length());
			return pushVec;
		}
	}

	return VECTOR3();
}

// 反射させるための関数
// 当たったときの法線ベクトルを受け取り跳ね返りベクトルを計算する
VECTOR3 Box::ReflectionVec(PhysicsObject& tObj, VECTOR3 normal)
{
	// 法線方向に反発係数をかける
	// 法線方向に垂直なベクトルに摩擦係数を計算
	VECTOR3 refNormal = dot(tObj.velocity, normal) * normal;

	VECTOR3 refSessen = tObj.velocity - refNormal;
	// 衝突したふたつのオブジェクトの反発係数と摩擦を考慮する
	float e2 = (this->pObj.e + tObj.e) / 2;
	float f2 = (this->pObj.f + tObj.f) / 2;
	VECTOR3 b = -refNormal * e2 + refSessen * f2;

	// 順番の修正
	// 埋め込みを解除->反射	〇
	// 反射->埋め込み解除		×
	return VECTOR3(b);
}

Box::~Box()
{
	if (mesh != nullptr) {
		delete mesh;
		mesh = nullptr;
	}
}


