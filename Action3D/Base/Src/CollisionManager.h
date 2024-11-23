#pragma once
/// <summary>
/// 2024.11.23
/// Õ“Ë”»’è‚ğŠÇ—‚·‚éƒNƒ‰ƒX
/// </summary>
/// <author>S.Matsunaga</author>

#include "Object3D.h"
#include "Ball.h"
#include "Box.h"

class CollisonManager : public Object3D
{
public:
	CollisonManager();
	~CollisonManager();
	void Start() override;
	void Update() override;
	void PushVec(PhysicsObject &sphere, VECTOR3 pushVec, VECTOR3 RefVec);
private:
	std::list<Ball*> balls;
	std::list<Ball*> resolvedBalls;	// Õ“Ë”»’è‚ğI‚¦‚½BallObject
	std::list<Box*> boxes;
};