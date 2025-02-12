#pragma once
#include "GameObject.h"
#include "FbxMesh.h"
#include "MeshCollider.h"

class Transform {
public:
	VECTOR3 position;
	VECTOR3 rotation;
	VECTOR3 scale;
	Transform() {
		position = VECTOR3(0, 0, 0);
		rotation = VECTOR3(0, 0, 0);
		scale = VECTOR3(1, 1, 1);
		parent = nullptr;
	}
	void SetParent(Object3D* obj)
	{
		parent = obj;
	}
	Object3D* GetParent() { return parent; }
	const MATRIX4X4 matrix();
private:
	Object3D* parent;
};

class SphereCollider {
public:
	VECTOR3 center;
	float radius;
	SphereCollider() {
		center = VECTOR3(0, 0, 0);
		radius = 0.0f;
	}
};

class Object3D : public GameObject {
public:
	Object3D();
	virtual ~Object3D();
	virtual void Update() override;
	virtual void Draw() override;

	const VECTOR3 Position() {
		return transform.position;
	};
	const VECTOR3 Rotation() {
		return transform.rotation;
	};
	const VECTOR3 Scale() {
		return transform.scale;
	};
	const MATRIX4X4 Matrix() {
		return transform.matrix();
	};

	// S.Matsunaga
	// 物理演算や衝突判定を適用させるオブジェクト用構造体
	struct PhysicsObject
	{
		VECTOR3 velocity = VECTOR3(0, 0, 0);	// 速度
		VECTOR3 center = VECTOR3(0, 0, 0);		// 中心点
		float e = 0.9f;	// 反発係数
		float f = 0.01f;	// 摩擦
		float radius = 0.5f;	// 球体用半径
		float mass = 1.0f;
		bool isPlayer = false;
		int pNum = 0;
		int score = 0;

	}pObj;

	// S.Matsunaga
	// Mapエディタ使用中に使う構造体
	struct EditObject
	{
		std::string name = "";	// オブジェクトの名前
		bool isSelect = false;	// 選択状態かどうか
		bool isScaling = true;	// スケールの変更を許可するか
	}editObj;

	void SetPosition(const VECTOR3& pos);
	void SetPosition(float x, float y, float z);
	void SetRotation(const VECTOR3& pos);
	void SetRotation(float x, float y, float z);
	void SetScale(const VECTOR3& pos);
	void SetScale(float x, float y, float z);

	virtual SphereCollider Collider();

	/// <summary>
	/// 球とメッシュの当たり判定をする
	/// 当たった場合にのみ、pushに押し返す場所を返す
	/// </summary>
	/// <param name="sphere">球体</param>
	/// <param name="push">押し返す場所を格納する場所</param>
	/// <returns>当たった場合にtrue</returns>
	virtual bool HitSphereToMesh(const SphereCollider& sphere, VECTOR3* push = nullptr);
	virtual bool HitSphereToMesh(const PhysicsObject& pObj, VECTOR3* push = nullptr, VECTOR3* refVec = nullptr);

	/// <summary>
	/// 直線とメッシュの当たり判定をする
	/// 当たった場合は、当たった座標を返す
	/// </summary>
	/// <param name="from">直線の始点</param>
	/// <param name="to">直線の終点</param>
	/// <param name="hit">交点を格納する場所</param>
	/// <returns>当たった場合にtrue</returns>
	virtual bool HitLineToMesh(const VECTOR3& from, const VECTOR3& to, VECTOR3* hit = nullptr);
#if 0
	/// <summary>
	/// 球と球の当たり判定をする
	/// 自分の球は、Collider()で取得する
	/// </summary>
	/// <param name="target">相手の球</param>
	/// <param name="withY">falseにするとYの座標差を無視する</param>
	/// <returns>重なり量</returns>
	virtual float HitSphereToSphere(const SphereCollider& target, bool withY=true);
#endif

	/// 2024.9.24 S.Matsunaga
	/// <summary>
	/// 球体とBoxの衝突判定
	/// 定義だけ書いてBox.cppで中をオーバーライド
	/// </summary>
	/// <param name="pOgj">衝突判定する球体の構造体</param>
	/// <param name="refVec">反射ベクトル</param>
	/// <returns>めり込み解除ベクトル</returns>
	virtual VECTOR3 HitSphereToCubeplane(PhysicsObject& pOgj, VECTOR3& refVec);

	/// 2024.11.05 S.Matsunaga
	/// <summary>
	/// 球と球の衝突判定
	/// </summary>
	/// <param name="target">ターゲットオブジェクトの構造体</param>
	/// <returns>衝突していたらtrue</returns>
	virtual bool HitSphereToSphere(PhysicsObject& target);

	/// 2024.11.23 S.Matsunaga
	/// <summary>
	/// 球と球の衝突判定
	/// </summary>
	/// <param name="target">ターゲットオブジェクトの構造体</param>
	/// <returns>衝突していたらtrue</returns>
	virtual bool HitSphereToSphere(PhysicsObject& checkSphere, PhysicsObject& target);


	/// 2024.11.11 S.Matsunaga
	/// <summary>
	/// 球体とAABBで簡易的な衝突判定
	/// 衝突していたらHitSphereCubeplaneを回すようにする
	/// 衝突していても衝突点や反射ベクトルなどを計算しない
	/// </summary>
	/// <param name="pOgj">衝突判定を取る球体の構造体</param>
	/// <returns>衝突していたらtrue</returns>
	virtual bool CheckSphereAABBCollision(PhysicsObject& pOgj);

	CFbxMesh* GetMesh() { return mesh; }

	MeshCollider* GetMeshColl() {return meshCol; }

protected:
	CFbxMesh* mesh;
	MeshCollider* meshCol;
	Transform transform;
};