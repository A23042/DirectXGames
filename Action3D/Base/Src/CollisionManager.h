#pragma once
/// <summary>
/// 2024.11.23
/// �Փ˔�����Ǘ�����N���X
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

	/// <summary>
	/// �r���Ő������ꂽBall�����X�g�ɒǉ�����
	/// </summary>
	/// <param name="ball">�ǉ�����Ball</param>
	void AddBall(Ball* ball);

	/// <summary>
	/// list�ō�����Ball�����ׂĕԂ�
	/// </summary>
	/// <returns>Ball�̊i�[���ꂽlist</returns>
	std::list<Ball*> GetBalls() { return balls; }

	/// <summary>
	/// Ball���X�g����w�肳�ꂽBall������Ώ���
	/// </summary>
	/// <param name="ball">����Ball</param>
	void RemoveBall(Ball* ball);
private:
	std::list<Ball*> balls;
	std::list<Ball*> resolvedBalls;	// �Փ˔�����I����BallObject
	std::list<Box*> boxes;
};