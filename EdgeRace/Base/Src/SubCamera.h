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
	// �J�����ԍ��̐ݒ�
	void SetNum(int num) { cmNum = num; }
	VECTOR3 GetLookPos() { return lookPosition; }
	// �J�����ԍ��擾
	int GetNum() { return cmNum; }
private:
	SplitScreen* ss;
	VECTOR3 lookPosition;
	int cmNum;	// �J�����ԍ�
	bool isAuto;
};