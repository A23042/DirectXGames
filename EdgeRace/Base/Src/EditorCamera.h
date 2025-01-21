#pragma once
/// <summary>
/// �}�b�v�G�f�B�^�Ŏg�p����J����
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
	CDirectInput* pDI = GameDevice()->m_pDI;	// pDI�V���[�g�J�b�g
	SplitScreen* ss;
	MainCamera* mainCam;

	POINT mousePos0;		// �}�E�X�X�N���[�����W
	POINT mousePos1;		// �}�E�X�X�N���[�����W

	VECTOR3 cameraPos;
	VECTOR3 lookPosition;
	VECTOR3 direction;	// �ړ�����
	
	float moveSpeed;	// �ړ����x
	float rotationSpeed;// ��]���x

	float diffX;
	float diffY;

	std::vector<MATRIX4X4> view;
	std::vector<VECTOR3> eyePt;
	std::vector<VECTOR3> lookatPt;
};