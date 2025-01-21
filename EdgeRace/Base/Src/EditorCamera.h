#pragma once
/// <summary>
/// マップエディタで使用するカメラ
/// </summary>
/// <author>S.Matsunaga</author>

#include "Object3D.h"

class SplitScreen;
class MainCamera;
class EditorCamera : public Object3D
{
public:
	EditorCamera();
	~EditorCamera();
	void Start() override;
	void Update() override;
	void Draw() override;

	MATRIX4X4 View(int counter) { return view[counter]; }
	VECTOR3 EyePt(int counter) { return eyePt[counter]; }
	VECTOR3 LookatPt(int counter) { return lookatPt[counter]; }

private:
	CDirectInput* pDI = GameDevice()->m_pDI;	// pDIショートカット
	SplitScreen* ss;
	MainCamera* mainCam;

	POINT mousePos0;		// マウススクリーン座標
	POINT mousePos1;		// マウススクリーン座標

	VECTOR3 cameraPos;
	VECTOR3 lookPosition;
	VECTOR3 direction;	// 移動方向
	
	float moveSpeed;	// 移動速度
	float rotationSpeed;// 回転速度

	float diffX;
	float diffY;

	std::vector<MATRIX4X4> view;
	std::vector<VECTOR3> eyePt;
	std::vector<VECTOR3> lookatPt;
};