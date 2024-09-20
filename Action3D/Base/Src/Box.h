#pragma once
#include "Object3D.h"
#include "Player.h"

class Box : public Object3D {
public:
	Box(float x, float y, float z);
	~Box();
	void Update() override;
	void CubeSize(float x, float y, float z);
	// 押し返すベクトルを返したいからVECTOR3
	// 平面の衝突判定
	VECTOR3 HitSphereToCubeplane(Player* player);
	// 辺の衝突判定
	VECTOR3 HitSphereToCubeEdge(Player* player);
	// 頂点の衝突判定
	VECTOR3 HitSphereToCubeVertices(Player* player);
	// 跳ね返りベクトルの計算
	VECTOR3 ReflectionVec(Player* player, VECTOR3 normal);

private:
	VECTOR3 normal[6];
	VECTOR3 ten[8];
	VECTOR3 plane[6];
	VECTOR3 v[12];
	VECTOR3 edge[12];
	VECTOR3 pt[8];
	float d[6];
	float distance[6];
	VECTOR3 distanceV[12];
	VECTOR3 vPos;
	float Tpt[12];

	VECTOR3 pushVec;
};