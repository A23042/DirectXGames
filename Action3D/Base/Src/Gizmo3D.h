#pragma once
/// <summary>
/// マップエディタの左下用のGizmo
/// </summary>
/// <author>S.Matsunaga</author>

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