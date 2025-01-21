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
	// ƒJƒƒ‰”Ô†‚Ìİ’è
	void SetNum(int num) { cmNum = num; }
	VECTOR3 GetLookPos() { return lookPosition; }
	// ƒJƒƒ‰”Ô†æ“¾
	int GetNum() { return cmNum; }
private:
	SplitScreen* ss;
	VECTOR3 lookPosition;
	int cmNum;	// ƒJƒƒ‰”Ô†
	bool isAuto;
};