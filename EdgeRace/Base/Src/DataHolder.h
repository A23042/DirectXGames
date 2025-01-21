#pragma once
// 2024.12.3 S.Matsunaga データを保持するクラス

#include "Object3D.h"

class DataHolder : public Object3D
{
public:
	DataHolder();
	~DataHolder();
	void Update() override {};
	void Draw() override {};

	// Play中ステータスセット
	void SetPlay(bool isPlay) { this->isPlay = isPlay; }
	// Play中かどうか
	bool IsPlay() { return isPlay; }

private:
	bool isPlay = false;
};