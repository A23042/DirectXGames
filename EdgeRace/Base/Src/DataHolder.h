#pragma once
// 2024.12.3 S.Matsunaga �f�[�^��ێ�����N���X

#include "Object3D.h"

class DataHolder : public Object3D
{
public:
	DataHolder();
	~DataHolder();
	void Update() override {};
	void Draw() override {};

	// Play���X�e�[�^�X�Z�b�g
	void SetPlay(bool isPlay) { this->isPlay = isPlay; }
	// Play�����ǂ���
	bool IsPlay() { return isPlay; }

private:
	bool isPlay = false;
};