#pragma once
#include "Object3D.h"
#include "Direct3D.h"
#include "Score.h"

/// ‰æ–Ê•ªŠ„ƒNƒ‰ƒX
/// </summary>
class SplitScreenLastDraw : public Object3D
{
public:
	SplitScreenLastDraw();
	~SplitScreenLastDraw();

	void Start() override;
	void Update() override;
	void Draw() override;

private:
	D3D11_VIEWPORT vpSingle;
	Score* score;
	
	int p0Score = 0;
	int p1Score = 0;

	float timer = 0.0f;

	int viewP0Score = 0;
	int viewP1Score = 0;
};