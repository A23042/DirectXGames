#pragma once
#include "Object3D.h"

class GizmoXYZ : public Object3D
{
public:
	GizmoXYZ();
	~GizmoXYZ();
	void Update() override;
private:
	// x,y,z,centerギズモ用のメッシュ
	CFbxMesh* meshes[4];
	MeshCollider* meshCols[3];
};
