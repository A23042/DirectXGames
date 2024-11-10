#pragma once
/// <summary>
/// 直方体の作成
/// サイズから頂点辺面を求める
/// 各面辺頂点と球体の衝突判定を行う
/// </summary>
/// <author>S.Matsunaga</author>

#include "Object3D.h"

class Box : public Object3D {
public:
	Box(VECTOR3 size = VECTOR3(1, 1, 1), VECTOR3 rot = VECTOR3(0, 0, 0));
	~Box();
	void Start() override;
	void Update() override;
	void Draw() override;
	void CubeSize(float x = 0, float y = 0, float z = 0);
	
	// 選択状態切替
	void SetSelect() {
		if (editObj.isSelect == true)
		{
			editObj.isSelect = false;
		}
		else
		{
			editObj.isSelect = true;
		}
	}
	// 選択されているか
	bool IsSelect() { return editObj.isSelect; }
	

	// 押し返すベクトルを返したいからVECTOR3
	// 平面の衝突判定
	VECTOR3 HitSphereToCubeplane(PhysicsObject& pOgj, VECTOR3 &refVec) override;
	// 辺の衝突判定
	VECTOR3 HitSphereToCubeEdge(PhysicsObject& pOgj, VECTOR3& refVec);
	// 頂点の衝突判定
	VECTOR3 HitSphereToCubeVertices(PhysicsObject& pOgj, VECTOR3& refVec);

	// 跳ね返りベクトルの計算(仮途中)
	virtual VECTOR3 ReflectionVec(PhysicsObject& pOgj, VECTOR3 normal);

private:
	VECTOR3 normal[6] = {};		// 法線
	VECTOR3 vertex[8] = {};		// 頂点
	VECTOR3 plane[6] = {};		// 平面の法線	
	VECTOR3 v[12] = {};			// 辺
	VECTOR3 edge[12] = {};		// 辺ベクトル
	VECTOR3 pt[8] = {};			// 各頂点からプレイヤーへのベクトル
	VECTOR3 distanceV[12] = {};	// 各辺からプレイヤーへのベクトル
	VECTOR3 vPos = {};

	VECTOR3 pushVec;
	VECTOR3 HitPoint;
	VECTOR3 refVec;

	float d[6] = {};	// 平面の定数
	float distance[6] = {};	// 面との距離
	float Tpt[12] = {};

	XMMATRIX rotationMatrix;

	CSprite* spr;
};