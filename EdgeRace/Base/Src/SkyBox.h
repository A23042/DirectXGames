#pragma once

#include "Object3D.h"

class SkyBox : public Object3D
{
public:
	SkyBox();
	~SkyBox();
	void Update() override;
	void Draw() override;
private:
	VECTOR3 lightDir;
};