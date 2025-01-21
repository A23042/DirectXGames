#pragma once
#include "Object3D.h"
#include "Direct3D.h"
#include "Player.h"

class Score;
class SplitScreen;
class Camera;
class DataHolder;
class Gizmo3D;
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
	CSprite* sp = nullptr;
	Gizmo3D* gizmo = nullptr;

	int p0Score = 0;
	int p1Score = 0;

	float timer = 0.0f;
	float upTimer = 0.0f;

	int viewP0Score = 0;
	int viewP1Score = 0;

	std::list<Player*> pls;

	CSprite* baseSpr = nullptr;
	CSprite* gageSpr = nullptr;

	CSpriteImage* base = nullptr;
	CSpriteImage* gage = nullptr;
	CSpriteImage* redArea = nullptr;
	float posX = 0.0f;
	float posY = 0.0f;
	float rate[2] = {};
	float width = 0.0f;
	float height = 0.0f;

};