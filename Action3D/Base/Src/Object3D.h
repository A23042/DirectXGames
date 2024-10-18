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
	}
	const MATRIX4X4 matrix() {
		MATRIX4X4 scaleM = XMMatrixScaling(
			scale.x, scale.y, scale.z);
		MATRIX4X4 rotX = XMMatrixRotationX(
			rotation.x);
		MATRIX4X4 rotY = XMMatrixRotationY(
			rotation.y);
		MATRIX4X4 rotZ = XMMatrixRotationZ(
			rotation.z);
		MATRIX4X4 trans = XMMatrixTranslation(
			position.x, position.y, position.z);
		return scaleM * rotZ * rotX * rotY * trans;
	}
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

	// 物理演算適用させるオブジェクト
	struct PhysicsObject
	{
		VECTOR3 velocity = VECTOR3(0, 0, 0);	// 速度
		VECTOR3 center = VECTOR3(0, 0, 0);		// 中心点
		float e;	// 反発係数
		float f;	// 摩擦
	}pObj;

	// 球体用の構造体
	struct Sphere : PhysicsObject
	{
		float radius = 0.0f;	// 半径
		float mass = 0.0f;
	}sphere;

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

	/// <summary>
	/// 直線とメッシュの当たり判定をする
	/// 当たった場合は、当たった座標を返す
	/// </summary>
	/// <param name="from">直線の始点</param>
	/// <param name="to">直線の終点</param>
	/// <param name="hit">交点を格納する場所</param>
	/// <returns>当たった場合にtrue</returns>
	virtual bool HitLineToMesh(const VECTOR3& from, const VECTOR3& to, VECTOR3* hit = nullptr);

	/// <summary>
	/// 球と球の当たり判定をする
	/// 自分の球は、Collider()で取得する
	/// </summary>
	/// <param name="target">相手の球</param>
	/// <param name="withY">falseにするとYの座標差を無視する</param>
	/// <returns>重なり量</returns>
	virtual float HitSphereToSphere(const SphereCollider& target, bool withY=true);

	// 球体とBoxの衝突判定
	// 定義だけ書いてBox.cppで中をオーバーライド
	virtual VECTOR3 HitSphereToCubeplane(Sphere& sphere, VECTOR3& refVec);

	// BallとPlayerの衝突判定
	virtual VECTOR3 HitPlayerToSphere(Sphere& sphere, VECTOR3& pushVec);
protected:
	CFbxMesh* mesh;
	MeshCollider* meshCol;
	Transform transform;
};