#pragma once
#include "Object3D.h"

class Box : public Object3D {
public:
	//Box(float x = 0, float y = 0, float z = 0, float rotX = 0, float rotY = 0, float rotZ = 0);
	Box(VECTOR3 size = VECTOR3(0,0,0), VECTOR3 rot = VECTOR3(0,0,0));

	~Box();
	void Update() override;
	void CubeSize(float x = 0, float y = 0, float z = 0);
	// 押し返すベクトルを返したいからVECTOR3
	// 平面の衝突判定
	VECTOR3 HitSphereToCubeplane(Sphere& sphere, VECTOR3 &refVec);
	// 辺の衝突判定
	VECTOR3 HitSphereToCubeEdge(Sphere& sphere, VECTOR3& refVec);
	// 頂点の衝突判定
	VECTOR3 HitSphereToCubeVertices(Sphere& sphere, VECTOR3& refVec);

	// 跳ね返りベクトルの計算(仮途中)
	virtual VECTOR3 ReflectionVec(Sphere& sphere, VECTOR3 normal);

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

	VECTOR3 refVec;
};