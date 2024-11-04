#pragma once
#include "Object3D.h"

class Gizmo3D : public Object3D
{
public:
	Gizmo3D();
	~Gizmo3D();
	void Update() override;
private:
	VECTOR3 pos;
};