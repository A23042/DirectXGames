#include "ObjectManager.h"
#include "GameObject.h"
#include <algorithm>
#include "Time.h"

namespace {
	const int distQNum = 2;	// 画面分割最大数
	struct UpdateObject {
		GameObject* object;
		bool initialized;
		bool destroyMe;
		bool dontDestroy;
		int priority;
		bool active;
		UpdateObject() : object(nullptr), initialized(false), destroyMe(false), dontDestroy(false), priority(0), active(true) {}
	};
	struct DrawObject {
		GameObject* object;
		int order;
		float distQ[distQNum];	// 視点からの距離の２乗[画面分割最大数]
		bool visible;
		DrawObject() : object(nullptr), order(0), visible(true) {}
	};
	std::list<UpdateObject> updateObjects;
	std::list<DrawObject> drawObjects;
	bool needSortUpdate;
	bool needSortDraw;
	int drawTimes = 2;	// 描画回数(画面分割数)
	int drawCounter;	// 描画カウンター(何画面目の描画か)
};


void deleteDrawObject(GameObject* obj)
{
	for (auto it = drawObjects.begin(); it != drawObjects.end(); ) {
		if ((*it).object == obj) {
			it = drawObjects.erase(it);
		}
		else {
			it++;
		}
	}
}

void ObjectManager::Start()
{
}

void ObjectManager::Update()
{
	if (needSortUpdate) {
		updateObjects.sort([](UpdateObject& a, UpdateObject& b) {return a.priority > b.priority; });
		needSortUpdate = false;
	}
	for (auto it = updateObjects.begin(); it != updateObjects.end(); ) {
		UpdateObject& node = *it;
		GameObject* obj = node.object;
		if (!node.initialized) {
			obj->Start();
			node.initialized = true;
		}
		if (node.active) {
			obj->Update();
		}
		if (node.destroyMe) {
			deleteDrawObject(obj);
			delete obj;
			it = updateObjects.erase(it);
			continue;
		}
		it++;
	}
}

void ObjectManager::Draw()
{
	for(drawCounter = 0; drawCounter < drawTimes; drawCounter++)	// drawTimes回数描画処理を行う
	{
		if (needSortDraw) {
			drawObjects.sort([](DrawObject& a, DrawObject& b) {return a.order > b.order; });
			needSortDraw = false;
		}
		for (DrawObject node : drawObjects) {
			if (node.visible) {
				node.object->Draw();
			}
		}
	}
}

void ObjectManager::Release()
{
	DeleteAllGameObject();
}

void ObjectManager::ChangeScene()
{
	for (auto it = updateObjects.begin(); it != updateObjects.end();) {
		UpdateObject& node = *it;
		if (!node.dontDestroy) {
			deleteDrawObject(node.object);
			delete node.object;
			it = updateObjects.erase(it);
		} else
			it++;
	}
}

std::list<GameObject*> ObjectManager::GetAllObjects()
{
	std::list<GameObject*> objs;
	for (UpdateObject obj : updateObjects) {
		objs.push_back(obj.object);
	}
	return objs;
}

void ObjectManager::Push(GameObject* obj)
{
	{
		UpdateObject node;
		node.object = obj;
		updateObjects.push_back(node);
		needSortUpdate = true;
	}
	{
		DrawObject node;
		node.object = obj;
		drawObjects.push_back(node);
		needSortDraw = true;
	}
}

void ObjectManager::Destroy(GameObject* obj)
{
	for (UpdateObject& ou : updateObjects) {
		if (ou.object == obj)
			ou.destroyMe = true;
	}
}

void ObjectManager::SetDrawOrder(GameObject* obj, int _order)
{
	for (DrawObject& od : drawObjects) {
		if (od.object == obj) {
			od.order = _order;
		}
	}
	needSortDraw = true;
}

void ObjectManager::SetPriority(GameObject* _obj, int _priority)
{
	for (UpdateObject& ou : updateObjects) {
		if (ou.object == _obj) {
			ou.priority = _priority;
		}
	}
	needSortUpdate = true;
}

void ObjectManager::DeleteGameObject(GameObject* obj)
{
	deleteDrawObject(obj);
	for (auto it = updateObjects.begin(); it != updateObjects.end(); ) {
		UpdateObject& node = (*it);
		if (node.object == obj) {
			delete obj;
			it = updateObjects.erase(it);
		}
		else
			it++;
	}
}

void ObjectManager::DeleteAllGameObject()
{
	for (auto it = updateObjects.begin(); it != updateObjects.end();) {
		UpdateObject& node = *it;
		delete node.object;
		it = updateObjects.erase(it);
	}
	updateObjects.clear();

	for (auto it = drawObjects.begin(); it != drawObjects.end();) {
		it = drawObjects.erase(it);
	}
	drawObjects.clear();
}

void ObjectManager::DontDestroy(GameObject* obj, bool dont)
{
	for (auto it = updateObjects.begin(); it != updateObjects.end(); it++) {
		UpdateObject& node = *it;
		if (node.object == obj) {
			node.dontDestroy = dont;
		}
	}
}

void ObjectManager::SetActive(GameObject* obj, bool active)
{
	for (auto it = updateObjects.begin(); it != updateObjects.end(); it++) {
		UpdateObject& node = *it;
		if (node.object == obj) {
			node.active = active;
		}
	}
}

void ObjectManager::SetVisible(GameObject* obj, bool visible)
{
	for (DrawObject& od : drawObjects) {
		if (od.object == obj) {
			od.visible = visible;
		}
	}
}

void ObjectManager::SetDrawTimes(int times)
{
	drawTimes = times;
}

int ObjectManager::DrawCounter()
{
	return drawCounter;
}

bool ObjectManager::IsExist(GameObject* obj)
{
	for (DrawObject& od : drawObjects) {
		if (od.object == obj) {
			return true;
		}
	}
	return false;
}

bool ObjectManager::IsVisible(GameObject* obj)
{
	for (DrawObject& od : drawObjects) {
		if (od.object == obj) {
			if (od.visible)
				return true;
		}
	}
	return false;
}

void ObjectManager::SetEyeDist(GameObject* obj, const float& distQIn, const int idx)
{
	for (DrawObject& od : drawObjects) {
		if (od.object == obj && od.visible) {
			od.distQ[idx] = distQIn;
		}
	}
	needSortDraw = true;
}