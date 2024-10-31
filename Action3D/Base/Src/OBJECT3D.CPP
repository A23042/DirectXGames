#include "Object3D.h"
#include "MeshCollider.h"

Object3D::Object3D()
{
	mesh = nullptr;
	meshCol = nullptr;
}

Object3D::~Object3D()
{
	if (mesh != nullptr) {
		delete mesh;
	}
	if (meshCol != nullptr) {
		delete meshCol;
	}
}

void Object3D::Update()
{
}

void Object3D::Draw()
{
	mesh->Render(transform.matrix());
}

void Object3D::SetPosition(const VECTOR3& pos)
{
	transform.position = pos;
}

void Object3D::SetPosition(float x, float y, float z)
{
	SetPosition(VECTOR3(x, y, z));
}

void Object3D::SetRotation(const VECTOR3& rot)
{
	transform.rotation = rot;
}

void Object3D::SetRotation(float x, float y, float z)
{
	SetRotation(VECTOR3(x, y, z));
}

void Object3D::SetScale(const VECTOR3& sca)
{
	transform.scale = sca;
}

void Object3D::SetScale(float x, float y, float z)
{
	SetScale(VECTOR3(x, y, z));
}

SphereCollider Object3D::Collider()
{
	SphereCollider sphere;
	sphere.center = transform.position + VECTOR3(0, 1, 0);
	sphere.radius = 0.5f;
	return sphere;
}

bool Object3D::HitSphereToMesh(const SphereCollider& sphere, VECTOR3* push)
{
	if (meshCol == nullptr)
		return false;
	MATRIX4X4 mat = transform.matrix();
	std::list<MeshCollider::CollInfo> meshes = meshCol->CheckCollisionSphereList(mat, sphere.center, sphere.radius);
	if (meshes.size() == 0)
		return false;
	if (push != nullptr) { // 座標が必要なのであれば
		VECTOR3 pushVec = VECTOR3(0, 0, 0); // 最終的に押すベクトル
		for (const MeshCollider::CollInfo& m : meshes) { // 当たっているすべてのポリゴンで
			VECTOR3 move = sphere.center - m.hitPosition;
			float len = move.Length(); // 当たった点から中心への距離
			move = move * ((sphere.radius - len) / len);
			VECTOR3 push = m.normal * Dot(move, m.normal); // 押し返したいベクトル
			// 今のpushVecと合成する
			VECTOR3 pushVecNorm = XMVector3Normalize(pushVec); // 合成済みベクトルの向き
			float dot = Dot(push, pushVecNorm);	// その成分の長さ
			if (dot < pushVec.Length()) {
				pushVec += push - pushVecNorm * dot; // その成分を減らしていい
			}
			else {
				pushVec = push;
			}
		}
		*push = pushVec;
	}
	return true;
}

bool Object3D::HitLineToMesh(const VECTOR3& from, const VECTOR3& to, VECTOR3* hit)
{
	if (meshCol == nullptr)
		return false;
	MATRIX4X4 mat = transform.matrix();
	MeshCollider::CollInfo coll;
	bool ret = meshCol->CheckCollisionLine(mat, from, to, &coll);
	if (ret) {
		if (hit != nullptr) {
			*hit = coll.hitPosition;
		}
		return true;
	}
	return false;
}

float Object3D::HitSphereToSphere(const SphereCollider& target, bool withY)
{
	SphereCollider my = Collider();
	VECTOR3 diff = target.center - my.center;
	if (withY == false)
		diff.y = 0.0f;
	float rsum = target.radius + my.radius;
	if (diff.LengthSquare() < rsum * rsum) {
		return rsum - diff.Length();
	}
	return 0.0f;
}

VECTOR3 Object3D::HitSphereToCubeplane(Sphere& sphere, VECTOR3& refVec)
{
	return VECTOR3();
}

/*
// PlayerとBallの衝突判定を書く
VECTOR3 Object3D::HitPlayerToSphere(Sphere& sphere, VECTOR3& pushVec)
{
	// PlayerとBallの距離計算
	VECTOR3 vec = sphere.center - this->sphere.center;
	float rsum = sphere.radius + this->sphere.radius;
	VECTOR3 normal = normalize(vec);

	// 衝突していたら
	// めり込み解除ベクトルの計算
	if (vec.LengthSquare() < rsum * rsum) {
		float pushLen = rsum - vec.Length();		// 押し出す長さ
		pushVec = normal * pushLen;// 押し出すベクトルに長さを掛ける
		// 反射ベクトルの計算
		// 法線方向に反発係数をかける
		// 法線方向に垂直なベクトルに摩擦係数を計算
		VECTOR3 refNormal = dot(sphere.velocity, normal) * normal;
		VECTOR3 refSessen = sphere.velocity - refNormal;
		// 衝突したふたつのオブジェクトの反発係数と摩擦を考慮する
		float e2 = (this->pObj.e + sphere.e) / 2;
		float f2 = (this->pObj.f + sphere.f) / 2;

		// 質量の考慮
		float massA = sphere.mass;
		float massB = this->sphere.mass;
		float sumMass = massA + massB;

		VECTOR3 velocityA = (massA - massB) / sumMass * this->sphere.velocity + (2 * massB) / sumMass * sphere.velocity;
		VECTOR3 velocityB = (2 * massB) / sumMass * this->sphere.velocity + (massB - massA) / sumMass * sphere.velocity;

		VECTOR3 a = -velocityA * e2 + refSessen;
		VECTOR3 b = -velocityB * e2 + refSessen;
		sphere.velocity = b;
		this->sphere.velocity = -a;
		//return a;
	}
	return VECTOR3();
}
*/
BOOL Object3D::HitPlayerToSphere(Sphere& player, VECTOR3& pushVec)
{
	// PlayerとBallの距離計算
	VECTOR3 vec = player.center - this->sphere.center;
	VECTOR3 vecB = this->sphere.center - player.center;

	float rsum = player.radius + this->sphere.radius;
	VECTOR3 normal = normalize(vec);

	// 衝突していたら
	// めり込み解除ベクトルの計算
	if (vec.LengthSquare() > rsum * rsum)
	{
		return false;
	}
	else
	{
		float pushLen = rsum - vec.Length();		// 押し出す長さ
		pushVec = normal * pushLen;// 押し出すベクトルに長さを掛ける

		this->sphere.center -= pushVec / 2;
		player.center += pushVec / 2;

		VECTOR3 vec = player.center - this->sphere.center;
		VECTOR3 normal = normalize(vec);

		// 衝突したふたつのオブジェクトの反発係数と摩擦を考慮する
		float e2 = (this->sphere.e + player.e) / 2;
		float f2 = (this->sphere.f + player.f) / 2;

		// 質量の考慮
		float massA = player.mass;
		float massB = this->sphere.mass;
		float sumMass = massA + massB;

		VECTOR3 constVec = e2 / sumMass * normal;

		VECTOR3 velocityA = -massB * constVec + player.velocity;
		VECTOR3 velocityB = massA * constVec + this->sphere.velocity;

		player.velocity = velocityB;
		this->sphere.velocity = velocityA;

		return true;

	}
}
