#include "Frustum.h"
#include "HierarchyManager.h"
#include "SplitScreen.h"
#include "Camera.h"

namespace
{
	VECTOR3 normal[6] = {};
	float d[6] = {};
	SplitScreen* ss = nullptr;
	Camera* camera = nullptr;
}

void Frustum::CreateFrustum(POINT startPos, POINT endPos)
{
	MATRIX4X4 mView;
	if (camera == nullptr)
	{
		camera = ObjectManager::FindGameObject<Camera>();
	}
	if (ss == nullptr)
	{
		ss = ObjectManager::FindGameObject<SplitScreen>();
	}
	if (ss != nullptr)
	{
		if (ss->Multi())
		{
			if (camera != nullptr)
			{
				mView = camera->View(0);
			}
		}
		else
		{
			mView = GameDevice()->m_mView;
		}
	}

	MATRIX4X4 mPrj = GameDevice()->m_mProj;
	MATRIX4X4 identity = XMMatrixIdentity();

	// 左上と右下の点を取る
	VECTOR2 min, max;
	min.x = min(startPos.x, endPos.x);
	max.x = max(startPos.x, endPos.x);
	min.y = min(startPos.y, endPos.y);
	max.y = max(startPos.y, endPos.y);

	// 近視点と遠視点
	struct NearFarPos
	{
		VECTOR3 nearPos = VECTOR3();
		VECTOR3 farPos = VECTOR3();
	};

	NearFarPos leftTop;
	leftTop.nearPos = XMVector3Unproject(VECTOR3(min.x, min.y, 0.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	leftTop.farPos = XMVector3Unproject(VECTOR3(min.x, min.y, 1.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);

	NearFarPos rightBottom;
	rightBottom.nearPos = XMVector3Unproject(VECTOR3(max.x, max.y, 0.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	rightBottom.farPos = XMVector3Unproject(VECTOR3(max.x, max.y, 1.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);

	NearFarPos leftBottom;
	leftBottom.nearPos = XMVector3Unproject(VECTOR3(min.x, max.y, 0.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	leftBottom.farPos = XMVector3Unproject(VECTOR3(min.x, max.y, 1.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);

	NearFarPos rightTop;
	rightTop.nearPos = XMVector3Unproject(VECTOR3(max.x, min.y, 0.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	rightTop.farPos = XMVector3Unproject(VECTOR3(max.x, min.y, 1.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);


	// 各面の法線と定数 //左,右,上,下,手前,奥
	normal[0] = normalize(cross(leftTop.farPos - leftTop.nearPos, leftBottom.farPos - leftTop.nearPos));
	d[0] = dot(-normal[0], leftTop.nearPos);

	normal[1] = normalize(cross(rightTop.nearPos - rightTop.farPos, rightBottom.nearPos - rightTop.farPos));
	d[1] = dot(-normal[1], rightTop.farPos);

	normal[2] = normalize(cross(leftTop.nearPos - leftTop.farPos, rightTop.farPos - leftTop.farPos));
	d[2] = dot(-normal[2], leftTop.farPos);

	normal[3] = normalize(cross(leftBottom.farPos - leftBottom.nearPos, rightBottom.farPos - leftBottom.nearPos));
	d[3] = dot(-normal[3], leftBottom.nearPos);

	normal[4] = normalize(cross(leftTop.nearPos - rightTop.nearPos, rightBottom.nearPos - rightTop.nearPos));
	d[4] = dot(-normal[4], rightTop.nearPos);

	normal[5] = normalize(cross(rightTop.farPos - leftTop.farPos, rightBottom.farPos - leftTop.farPos));
	d[5] = dot(-normal[5], leftTop.farPos);
}

std::list<Object3D*> Frustum::CheckAABB(POINT startPos, POINT endPos)
{
	CreateFrustum(startPos, endPos);
	std::list<Object3D*> inFrustum = {};
	// AABBの最小点と最大点
	std::list<Object3D*> objs = HierarchyManager::GetHierarchyList();
	for (Object3D* obj : objs)
	{
		VECTOR3 min = obj->GetMeshColl()->GetBBox().min * obj->Matrix();
		VECTOR3 max = obj->GetMeshColl()->GetBBox().max * obj->Matrix();
		// バウンディングボックスのすべての頂点
		VECTOR3 vertex[8] = {
				VECTOR3(max.x, max.y, min.z),
				VECTOR3(max.x, min.y, min.z),
				VECTOR3(min.x, min.y, min.z),
				VECTOR3(min.x, max.y, min.z),
				VECTOR3(max.x, max.y, max.z),
				VECTOR3(max.x, min.y, max.z),
				VECTOR3(min.x, min.y, max.z),
				VECTOR3(min.x, max.y, max.z),
		};
		// objのバウンディングボックスがフラスタム(視錐台)の内側にいるか判定を取る
		// 一つでも面の外側にいたらfor文を抜ける
		bool outFlag = false;
		for (int i = 0; i < 6; i++)	// フラスタムの各面
		{
			for (int j = 0; j < 8; j++)	// バウンディングボックスの頂点
			{
				float dist = dot(normal[i], vertex[j]) + d[i];;
				if (dist < 0)
				{
					outFlag = true;	// 範囲外
					break;
				}
			}
			if (outFlag)
			{
				break;
			}
		}
		if(!outFlag)
		{
			inFrustum.push_back(obj);
		}
	}
	return inFrustum;
}
