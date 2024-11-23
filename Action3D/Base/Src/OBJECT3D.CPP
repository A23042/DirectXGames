#include "Object3D.h"
#include "MeshCollider.h"

Object3D::Object3D()
{
	mesh = nullptr;
	meshCol = nullptr;
}

Object3D::~Object3D()
{
	if (mesh != nullptr) 
	{
		delete mesh;
	}
	if (meshCol != nullptr) 
	{
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

/*
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
*/
VECTOR3 Object3D::HitSphereToCubeplane(PhysicsObject& pOgj, VECTOR3& refVec)
{
	return VECTOR3();
}


bool Object3D::HitSphereToSphere(PhysicsObject& target)
{
	// targetとBallの距離計算
	VECTOR3 vec = target.center - this->pObj.center;

	float rsum = target.radius + this->pObj.radius;
	// P1とP2の法線ベクトル(正規化)
	VECTOR3 normal = normalize(vec);

	// 衝突していたら
	// めり込み解除ベクトルの計算
	if (vec.LengthSquare() > rsum * rsum)
	{
		// 衝突していない場合は終了
		return false;
	}
	else
	{
		VECTOR3 pushVec = VECTOR3();
		float pushLen = rsum - vec.Length();	// 押し出す長さ
		pushVec = normal * pushLen;				// 押し出すベクトルに長さを掛ける
		this->pObj.center -= pushVec / 2;
		target.center += pushVec / 2;

		// 反発係数
		float aveE = (target.e + this->pObj.e) / 2;

		// 質量
		float p1Mass = target.mass;
		float p2Mass = this->pObj.mass;
		float sumMass = p1Mass + p2Mass;

		// targetとBallのVelocity
		VECTOR3 p1Velocity = target.velocity;
		VECTOR3 p2Velocity = this->pObj.velocity;

		// Ball(P2)に渡すベクトル(法線方向の速度)
		VECTOR3 p1OverVec = (dot(normal, p1Velocity) * normal);
		// target(P1)の残ったベクトル
		VECTOR3 p1RestVec = p1Velocity - p1OverVec;
				
		// P1に渡すベクトル(法線方向の速度)
		VECTOR3 p2OverVec = (dot(normal, p2Velocity) * normal);
		// P2の残ったベクトル
		VECTOR3 p2RestVec = p2Velocity - p2OverVec;
		
		// 法線方向に質量を考慮した速度を計算する
		// P1の法線方向速度
		VECTOR3 newNomalP1Vec = ((p1Mass - p2Mass) * p1OverVec + 2 * p2Mass * p2OverVec) / sumMass;
		// P2の法線方向速度
		VECTOR3 newNomalP2Vec = ((p2Mass - p1Mass) * p2OverVec + 2 * p1Mass * p1OverVec) / sumMass;

		// 衝突後の速度
		VECTOR3 resultP1Vec = (p1RestVec + newNomalP1Vec) * aveE;
		VECTOR3 resultP2Vec = (p2RestVec + newNomalP2Vec) * aveE;

		target.velocity = resultP1Vec;
		this->pObj.velocity = resultP2Vec;
		return true;
	}
}

bool Object3D::HitSphereToSphere(PhysicsObject& checkSphere, PhysicsObject& targetSphere)
{
	// targetとBallの距離計算
	VECTOR3 vec = targetSphere.center - checkSphere.center;

	float rsum = targetSphere.radius + checkSphere.radius;
	// P1とP2の法線ベクトル(正規化)
	VECTOR3 normal = normalize(vec);

	// 衝突していたら
	// めり込み解除ベクトルの計算
	if (vec.LengthSquare() > rsum * rsum)
	{
		// 衝突していない場合は終了
		return false;
	}
	else
	{
		VECTOR3 pushVec = VECTOR3();
		float pushLen = rsum - vec.Length();	// 押し出す長さ
		pushVec = normal * pushLen;				// 押し出すベクトルに長さを掛ける
		checkSphere.center -= pushVec / 2;
		targetSphere.center += pushVec / 2;

		// 反発係数
		float aveE = (targetSphere.e + checkSphere.e) / 2;

		// 質量
		float p1Mass = targetSphere.mass;
		float p2Mass = checkSphere.mass;
		float sumMass = p1Mass + p2Mass;

		// targetSphereとBallのVelocity
		VECTOR3 p1Velocity = targetSphere.velocity;
		VECTOR3 p2Velocity = checkSphere.velocity;

		// Ball(P2)に渡すベクトル(法線方向の速度)
		VECTOR3 p1OverVec = (dot(normal, p1Velocity) * normal);
		// targetSphere(P1)の残ったベクトル
		VECTOR3 p1RestVec = p1Velocity - p1OverVec;

		// P1に渡すベクトル(法線方向の速度)
		VECTOR3 p2OverVec = (dot(normal, p2Velocity) * normal);
		// P2の残ったベクトル
		VECTOR3 p2RestVec = p2Velocity - p2OverVec;

		// 法線方向に質量を考慮した速度を計算する
		// P1の法線方向速度
		VECTOR3 newNomalP1Vec = ((p1Mass - p2Mass) * p1OverVec + 2 * p2Mass * p2OverVec) / sumMass;
		// P2の法線方向速度
		VECTOR3 newNomalP2Vec = ((p2Mass - p1Mass) * p2OverVec + 2 * p1Mass * p1OverVec) / sumMass;

		// 衝突後の速度
		VECTOR3 resultP1Vec = (p1RestVec + newNomalP1Vec) * aveE;
		VECTOR3 resultP2Vec = (p2RestVec + newNomalP2Vec) * aveE;

		targetSphere.velocity = resultP1Vec;
		checkSphere.velocity = resultP2Vec;
		return true;
	}

}

bool Object3D::CheckSphereAABBCollision(PhysicsObject& pOgj)
{
	return false;
}

const MATRIX4X4 Transform::matrix()
{
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
	//return scaleM * rotZ * rotX * rotY * trans;
	MATRIX4X4 mat = scaleM * rotZ * rotX * rotY * trans;
	// 親がいる場合は親のマトリクスを掛ける
	if (parent != nullptr)
	{
		mat = mat * parent->Matrix();
	}
	return mat;
}
