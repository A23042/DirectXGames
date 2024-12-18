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

	int viewType; // 視点切り替えの番号

	float changeTime; // 視点切り替え時間を数える
	VECTOR3 changePosStart; // 視点移動の開始点
	VECTOR3 changePosGoal; // 視点移動の終了点 
	VECTOR3 changeLookStart; // 注視点移動の開始点
	VECTOR3 changeLookGoal; // 注視点移動の終了点 

	std::vector<MATRIX4X4> view;
	std::vector<VECTOR3> eyePt;
	std::vector<VECTOR3> lookatPt;
};