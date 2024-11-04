#pragma once
#include "Object3D.h"

class EditorCamera : public Object3D
{
public:
	EditorCamera();
	~EditorCamera();
	void Update() override;
	void Draw() override;
private:
	CDirectInput* pDI = GameDevice()->m_pDI;	// pDIショートカット

	POINT mousePos0;		// マウススクリーン座標
	POINT mousePos1;		// マウススクリーン座標

	VECTOR3 lookPosition;
	VECTOR3 direction;	// 移動方向
	
	float moveSpeed;	// 移動速度
	float rotationSpeed;// 回転速度
};