#pragma once
/// <summary>
/// ���_���Z�G���A
/// </summary>
/// <author>S.Matsunaga</author>
/// 
#include "Box.h"

class ScoreArea : public Box
{
public:
	ScoreArea();
	~ScoreArea();
	void Update() override;
	void Draw() override;
	virtual void ScoreCount(PhysicsObject& pObj) 
	{
		pObj.score = 0;
	}
	/// <summary>
	/// ���̂�AABB�ŊȈՓI�ȏՓ˔���
	/// �Փ˂��Ă�����HitSphereCubeplane���񂷂悤�ɂ���
	/// �Փ˂��Ă��Ă��Փ˓_�┽�˃x�N�g���Ȃǂ��v�Z���Ȃ�
	/// </summary>
	/// <param name="pOgj">�Փ˔������鋅�̂̍\����</param>
	/// <returns>�Փ˂��Ă�����true</returns>
	virtual bool CheckSphereAABBCollision(PhysicsObject& pObj) override;

protected:
	VECTOR3 vPos = {};
private:
	int score = 0;
	int n = 1;
};

class ScoreArea1 : public ScoreArea
{
public:
	ScoreArea1(VECTOR3 size = VECTOR3(1, 0.1, 1), VECTOR3 rot = VECTOR3(0, 0, 0));
	~ScoreArea1();
	void Update() override;
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
	ScoreArea2(VECTOR3 size = VECTOR3(1, 0.1, 1), VECTOR3 rot = VECTOR3(0, 0, 0));
	~ScoreArea2();
	void Update() override;
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
	ScoreArea3(VECTOR3 size = VECTOR3(1, 0.1, 1), VECTOR3 rot = VECTOR3(0, 0, 0));
	~ScoreArea3();
	void Update() override;
	void ScoreCount(PhysicsObject& pObj)override
	{
		pObj.score = 3;
	}
private:
	int n = 3;
	VECTOR3 vPos = {};

};