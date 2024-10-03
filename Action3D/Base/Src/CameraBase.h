#pragma once
#include "Object3D.h"

class CameraBase : public Object3D {
public:
	CameraBase();
	~CameraBase();
	void Update() override;
	void Draw() override;
private:
	ID3D11DeviceContext* d3dDeviceContext;   //D3D11�f�o�C�X�R���e�L�X�g�A�������ς݂Ƃ���
	D3D11_VIEWPORT vp[2];
};