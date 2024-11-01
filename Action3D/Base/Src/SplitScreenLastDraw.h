#pragma once
#include "Object3D.h"
#include "Direct3D.h"

/// 画面分割クラス
/// </summary>
class SplitScreenLastDraw : public Object3D
{
public:
	SplitScreenLastDraw();
	~SplitScreenLastDraw();

	void Update() override;
	void Draw() override;

private:
	D3D11_VIEWPORT vpSingle;
};