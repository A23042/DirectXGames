#pragma once
/// <summary>
/// “¾“_‰ÁŽZƒGƒŠƒA
/// </summary>
/// <author>S.Matsunaga</author>
/// 
#include "BoxCollisionBase.h"

class ScoreArea : public BoxCollisionBase
{
public:
	ScoreArea();
	~ScoreArea();
	//void Update() override;
	void Draw() override;
	virtual void ScoreCount(PhysicsObject& pObj) 
	{
		pObj.score = 0;
	}

protected:
	VECTOR3 vPos = {};
	CSprite* spr;
private:
	int score = 0;
	int n = 1;
};

class ScoreArea1 : public ScoreArea
{
public:
	ScoreArea1(VECTOR3 size = VECTOR3(1, 0.1f, 1), VECTOR3 rot = VECTOR3(0, 0, 0));
	~ScoreArea1();
	//void Update() override;
	void ScoreCount(PhysicsObject& pObj) override
	{
		pObj.score = 1;
	}
private:
	int n = 1;
};

class ScoreArea2 : public ScoreArea
{
public:
	ScoreArea2(VECTOR3 size = VECTOR3(1, 0.1f, 1), VECTOR3 rot = VECTOR3(0, 0, 0));
	~ScoreArea2();
	//void Update() override;
	void ScoreCount(PhysicsObject& pObj)override
	{
		pObj.score = 2;
	}
private:
	int n = 2;
	VECTOR3 vPos = {};

};

class ScoreArea3 : public ScoreArea
{
public:
	ScoreArea3(VECTOR3 size = VECTOR3(1, 0.1f, 1), VECTOR3 rot = VECTOR3(0, 0, 0));
	~ScoreArea3();
	//void Update() override;
	void ScoreCount(PhysicsObject& pObj)override
	{
		pObj.score = 3;
	}
private:
	int n = 3;
	VECTOR3 vPos = {};

};