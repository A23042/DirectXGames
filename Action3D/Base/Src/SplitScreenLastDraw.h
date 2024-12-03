#pragma once
#include "Object3D.h"
#include "Direct3D.h"

class Score;
class SplitScreen;
class Camera;
class DataHolder;

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
	Score* score = nullptr;
	SplitScreen* ss = nullptr;
	Camera* cm = nullptr;
	DataHolder* data = nullptr;
	CSprite* sp;

	int p0Score = 0;
	int p1Score = 0;

	float timer = 0.0f;
	float upTimer = 0.0f;

	int viewP0Score = 0;
	int viewP1Score = 0;
};