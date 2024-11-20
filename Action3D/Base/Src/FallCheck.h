#pragma once
/// <summary>
/// PlayerとBallの落下判定を取るための基準となるオブジェクト
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