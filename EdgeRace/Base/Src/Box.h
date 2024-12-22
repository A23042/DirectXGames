#pragma once
/// <summary>
/// 直方体の作成
/// サイズから頂点辺面を求める
/// 各面辺頂点と球体の衝突判定を行う
/// </summary>
/// <author>S.Matsunaga</author>

#include "Object3D.h"
#include "BoxCollisionBase.h"

class Box : public BoxCollisionBase {
public:
	Box(VECTOR3 size = VECTOR3(1, 1, 1), VECTOR3 rot = VECTOR3(0, 0, 0));
	~Box();
	void Start() override;
	void Update() override;
	void Draw() override;

private:
	Object3D* child = nullptr;
};