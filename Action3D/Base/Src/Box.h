#pragma once
#include "Object3D.h"

class Box : public Object3D {
public:
	Box(float x, float y, float z);
	~Box();
	void Update() override;
	void CubeSize(float x, float y, float z);
	// ‰Ÿ‚µ•Ô‚·ƒxƒNƒgƒ‹‚ð•Ô‚µ‚½‚¢‚©‚çVECTOR3
	VECTOR3 HitSphereToCubeplane(Object3D* player);
	VECTOR3 HitSphereToCubeEdge(Object3D* player);
	VECTOR3 HitSphereToCubeVertices(Object3D* player);

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