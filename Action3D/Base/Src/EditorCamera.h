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
	CDirectInput* pDI = GameDevice()->m_pDI;	// pDI�V���[�g�J�b�g

	POINT mousePos0;		// �}�E�X�X�N���[�����W
	POINT mousePos1;		// �}�E�X�X�N���[�����W

	VECTOR3 lookPosition;
	VECTOR3 direction;	// �ړ�����
	
	float moveSpeed;	// �ړ����x
	float rotationSpeed;// ��]���x
};