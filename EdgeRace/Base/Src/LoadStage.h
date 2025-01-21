#pragma once
#include "Object3D.h"
#include "CollisionManager.h"

class SplitScreen;
class Camera;
class LoadStage : public Object3D
{
public:
	LoadStage();
	~LoadStage();
	void Start() override;
	void Update() override;
	void Load(int num);
private:
	SplitScreen* ss = nullptr;
	Camera* camera = nullptr;
	VECTOR3 startPos;
	VECTOR3 endPos;
	float timeCount;
	float moveTime;
	bool isLoad = false;
};