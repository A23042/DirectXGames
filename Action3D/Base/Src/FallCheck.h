#pragma once
/// <summary>
/// Player��Ball�̗����������邽�߂̊�ƂȂ�I�u�W�F�N�g
/// </summary>
/// <author>S.Matsunaga</author>

#include "Object3D.h"

class FallCheck : public Object3D
{
public:
	FallCheck(bool playScene = false);
	~FallCheck();
	void Update() override;
private:
	float checkHeight;
	bool isPlayScene;
};