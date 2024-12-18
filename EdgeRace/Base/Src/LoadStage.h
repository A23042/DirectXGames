#pragma once
#include "Object3D.h"
#include "CollisionManager.h"

class LoadStage : public Object3D
{
public:
	LoadStage();
	~LoadStage();
	void Start() override;
	void Update() override;
	void Load(int num);
private:
	CollisonManager* collManager = nullptr;
};