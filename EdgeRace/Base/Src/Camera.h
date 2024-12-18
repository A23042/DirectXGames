#pragma once
#include "Object3D.h"
#include "SplitScreen.h"

class Camera : public Object3D {
public:
	Camera();
	~Camera();
	void Update() override;
	void Draw() override;

	MATRIX4X4 View(int counter) { return view[counter]; }
	VECTOR3 EyePt(int counter) { return eyePt[counter]; }
	VECTOR3 LookatPt(int counter) { return lookatPt[counter]; }
private:
	void updateCamera(int counter, VECTOR3 pos, VECTOR3 rot);
	SplitScreen* ssObj;

	VECTOR3 lookPosition;

	int viewType; // ���_�؂�ւ��̔ԍ�

	float changeTime; // ���_�؂�ւ����Ԃ𐔂���
	VECTOR3 changePosStart; // ���_�ړ��̊J�n�_
	VECTOR3 changePosGoal; // ���_�ړ��̏I���_ 
	VECTOR3 changeLookStart; // �����_�ړ��̊J�n�_
	VECTOR3 changeLookGoal; // �����_�ړ��̏I���_ 

	std::vector<MATRIX4X4> view;
	std::vector<VECTOR3> eyePt;
	std::vector<VECTOR3> lookatPt;
};