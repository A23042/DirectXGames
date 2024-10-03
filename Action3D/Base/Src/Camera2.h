#pragma once
#include "Object3D.h"

class Camera2 : public Object3D {
public:
	Camera2();
	~Camera2();
	void Update() override;
	void Draw() override;
private:
	VECTOR3 lookPosition;

	int viewType; // ���_�؂�ւ��̔ԍ�

	float changeTime; // ���_�؂�ւ����Ԃ𐔂���
	VECTOR3 changePosStart; // ���_�ړ��̊J�n�_
	VECTOR3 changePosGoal; // ���_�ړ��̏I���_ 
	VECTOR3 changeLookStart; // �����_�ړ��̊J�n�_
	VECTOR3 changeLookGoal; // �����_�ړ��̏I���_ 
};