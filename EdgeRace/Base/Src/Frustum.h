#pragma once
#include "BoxCollisionBase.h"

namespace Frustum
{
	void CreateFrustum(POINT startPos, POINT endPos);
	std::list<Object3D*> CheckAABB();
}