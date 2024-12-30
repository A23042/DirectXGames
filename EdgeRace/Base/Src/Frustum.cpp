#include "Frustum.h"
#include "HierarchyManager.h"

namespace
{
	VECTOR3 normal[6] = {};
	float d[6] = {};
}

void Frustum::CreateFrustum(POINT startPos, POINT endPos)
{
	
	MATRIX4X4 mView = GameDevice()->m_mView;
	MATRIX4X4 mPrj = GameDevice()->m_mProj;
	MATRIX4X4 identity = XMMatrixIdentity();

	// 近視点(0)と遠視点(1)
	VECTOR3 start0Pos = XMVector3Unproject(VECTOR3(startPos.x, startPos.y, 0.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	VECTOR3 start1Pos = XMVector3Unproject(VECTOR3(startPos.x, startPos.y, 1.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);

	VECTOR3 end0Pos =   XMVector3Unproject(VECTOR3(endPos.x, endPos.y, 0.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);
	VECTOR3 end1Pos =   XMVector3Unproject(VECTOR3(endPos.x, endPos.y, 1.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, mPrj, mView, identity);

	struct NearFarPos
	{
		VECTOR3 nearPos = VECTOR3();
		VECTOR3 farPos = VECTOR3();
	};

	NearFarPos leftTop;
	leftTop.nearPos = start0Pos;
	leftTop.farPos = start1Pos;

	NearFarPos leftBottom;
	leftBottom.nearPos = VECTOR3(start0Pos.x,end0Pos.y,start0Pos.z);
	leftBottom.farPos = VECTOR3(start1Pos.x, end1Pos.y, start1Pos.z);

	NearFarPos rightTop;
	rightTop.nearPos = VECTOR3(end0Pos.x, start0Pos.y, end0Pos.z);
	rightTop.farPos = VECTOR3(end1Pos.x, start1Pos.y, end1Pos.z);

	NearFarPos rightBottom;
	rightBottom.nearPos = end0Pos;
	rightBottom.farPos = end1Pos;

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

std::list<Object3D*> Frustum::CheckAABB()
{
	std::list<Object3D*> inFrustum = {};
	// AABBの最小点と最大点
	std::list<Object3D*> objs = HierarchyManager::GetHierarchyList();
	for (Object3D* obj : objs)
	{
		VECTOR3 min = obj->GetMeshColl()->GetBBox().min;
		VECTOR3 max = obj->GetMeshColl()->GetBBox().max;

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
		//distance[i] = abs(dot(plane[i], tObj.center) + d[i]) / plane[i].Length();
		// objのバウンディングボックスがフラスタム(視錐台)の内側にいるか判定を取る
		int icount = 0;
		for (int i = 0; i < 6; i++)
		{
			int jcount = 0;
			for (int j = 0; j < 8; j++)
			{
				float dist = 0.0f;
				dist = dot(normal[i], vertex[j]) + d[i];
				if (dist > 0)
				{
					jcount++;
				}
			}
			if (jcount == 8)
			{
				icount++;
			}
		}
		if (icount == 6)
		{
			inFrustum.push_back(obj);
		}
	}

	return inFrustum;
}
