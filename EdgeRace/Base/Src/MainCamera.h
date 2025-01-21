#pragma once

#include "Object3D.h"

class MainCamera : public Object3D
{
public:
	MainCamera();
	~MainCamera();
	void Update() override;
	void Draw() override {};
	VECTOR3 GetLookPos() { return lookPosition; }
private:
	CDirectInput* pDI = GameDevice()->m_pDI;	// pDIショートカット

	POINT mousePos0;		// マウススクリーン座標
	POINT mousePos1;		// マウススクリーン座標

	VECTOR3 lookPosition;
	VECTOR3 direction;	// 移動方向

	float moveSpeed;	// 移動速度
	float rotationSpeed;// 回転速度
};