#pragma once
#include "Object3D.h"

class Box : public Object3D {
public:
	Box();
	~Box();
	void Update() override;
	VECTOR3 HitSphereToCube(Object3D* player);
	VECTOR3 Hitv(Object3D* player);
private:
	VECTOR3 normal[6];
	VECTOR3 ten[8];
	VECTOR3 plane[6];
	VECTOR3 v[12];
	VECTOR3 edge[12];
	VECTOR3 pt[8];
	float d[6];
	float distance[6];

	VECTOR3 ten0, ten1, ten2, ten3, ten4, ten5, ten6, ten7;
	VECTOR3 plane0, plane1, plane2, plane3, plane4, plane5;
	VECTOR3 v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12;
	VECTOR3 edge0, edge1, edge2, edge3, edge4, edge5, edge6, edge7, edge8, edge9, edge10, edge11;
	VECTOR3 pt0, pt1, pt2, pt3, pt4, pt5, pt6, pt7;
	VECTOR3 pushVec;
};