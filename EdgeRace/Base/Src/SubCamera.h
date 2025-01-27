#pragma once

#include "Object3D.h"

class SplitScreen;
class SubCamera : public Object3D
{
public:
	SubCamera(bool autoNum = true);
	~SubCamera();
	void Update() override;
	void Draw() override;
	// カメラ番号の設定
	void SetNum(int num) { cmNum = num; }
	VECTOR3 GetLookPos() { return lookPosition; }
	// カメラ番号取得
	int GetNum() { return cmNum; }
private:
	SplitScreen* ss;
	VECTOR3 lookPosition;
	int cmNum;	// カメラ番号
	bool isAuto;
};