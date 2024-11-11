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

	/// <summary>
	/// Boxの頂点、辺のベクトル、平面の法線を求める
	/// </summary>
	/// <param name="x">X軸の辺の長さ</param>
	/// <param name="y">Y軸の辺の長さ</param>
	/// <param name="z">Z軸の辺の長さ</param>
	void CubeSize(float x = 0, float y = 0, float z = 0);
	
	/// <summary>
	/// EditSceneで選択状態を切り替える
	/// 呼び出されると現在の選択状態を変える
	/// </summary>
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
	
	/// <summary>
	/// EditSceneで選択されているか
	/// </summary>
	/// <returns>選択されていたらtrue</returns>
	bool IsSelect() { return editObj.isSelect; }

	// 押し返すベクトルを返したいからVECTOR3
	
	/// <summary>
	/// 球体と平面の衝突判定
	/// 衝突していたらめり込み解除ベクトルを返す
	/// 衝突していなければ辺との衝突判定に移る
	/// </summary>
	/// <param name="pOgj">衝突判定を取る球体の構造体</param>
	/// <param name="refVec">反射ベクトル</param>
	/// <returns>めり込み解除ベクトル</returns>
	VECTOR3 HitSphereToCubeplane(PhysicsObject& pOgj, VECTOR3 &refVec) override;

	/// <summary>
	/// 球体と辺との衝突判定
	/// 衝突していたらめり込み解除ベクトルを返す
	/// 衝突していなければ頂点との衝突判定に移る
	/// </summary>
	/// <param name="pOgj">衝突判定を取る球体の構造体</param>
	/// <param name="refVec">反射ベクトル</param>
	/// <returns>めり込み解除ベクトル</returns>
	VECTOR3 HitSphereToCubeEdge(PhysicsObject& pOgj, VECTOR3& refVec);
	
	/// <summary>
	/// 球体と頂点の衝突判定
	/// 衝突していたらめり込み解除ベクトルを返す
	/// 衝突していなければVECTOR3(0,0,0)を返す
	/// </summary>
	/// <param name="pOgj">衝突判定を取る球体の構造体</param>
	/// <param name="refVec">反射ベクトル</param>
	/// <returns>めり込み解除ベクトル</returns>
	VECTOR3 HitSphereToCubeVertices(PhysicsObject& pOgj, VECTOR3& refVec);

	// 跳ね返りベクトルの計算
	virtual VECTOR3 ReflectionVec(PhysicsObject& pOgj, VECTOR3 normal);
	
	/// <summary>
	/// 球体とAABBで簡易的な衝突判定
	/// 衝突していたらHitSphereCubeplaneを回すようにする
	/// 衝突していても衝突点や反射ベクトルなどを計算しない
	/// </summary>
	/// <param name="pOgj">衝突判定を取る球体の構造体</param>
	/// <returns>衝突していたらtrue</returns>
	bool CheckSphereAABBCollision(PhysicsObject& pOgj) override;

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

	VECTOR3 min;  // AABBの最小点
	VECTOR3 max;  // AABBの最大点

	float d[6] = {};	// 平面の定数
	float distance[6] = {};	// 面との距離
	float Tpt[12] = {};

	XMMATRIX rotationMatrix;

	CSprite* spr;

	bool isStart = false;
};