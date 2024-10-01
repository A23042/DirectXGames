#pragma once
#include "Object3D.h"
#include "Player.h"

class Box : public Object3D {
public:
	Box(float x, float y, float z, float rotX, float rotY, float rotZ);
	~Box();
	void Update() override;
	void CubeSize(float x, float y, float z);
	// 押し返すベクトルを返したいからVECTOR3
	// 平面の衝突判定
	VECTOR3 HitSphereToCubeplane(Sphere& sphere);
	// 辺の衝突判定
	VECTOR3 HitSphereToCubeEdge(Sphere& sphere);
	// 頂点の衝突判定
	VECTOR3 HitSphereToCubeVertices(Sphere& sphere);

	// 跳ね返りベクトルの計算(仮途中)
	VECTOR3 ReflectionVec(Sphere& sphere, VECTOR3 normal);

private:
	VECTOR3 normal[6] = {};
	VECTOR3 ten[8] = {};
	VECTOR3 plane[6] = {};
	VECTOR3 v[12] = {};
	VECTOR3 edge[12] = {};
	VECTOR3 pt[8] = {};
	float d[6] = {};
	float distance[6] = {};
	VECTOR3 distanceV[12] = {};
	VECTOR3 vPos = {};
	float Tpt[12] = {};

	VECTOR3 pushVec;
	VECTOR3 HitPoint;

	XMMATRIX rotationMatrix;
};