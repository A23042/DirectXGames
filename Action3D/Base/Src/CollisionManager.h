#pragma once
/// <summary>
/// 2024.11.23
/// 衝突判定を管理するクラス
/// </summary>
/// <author>S.Matsunaga</author>

#include "Object3D.h"
#include "Ball.h"
#include "Box.h"
#include "ScoreArea.h"

class CollisonManager : public Object3D
{
public:
	CollisonManager();
	~CollisonManager();
	void Start() override;
	void Update() override;
	void PushVec(PhysicsObject &sphere, VECTOR3 pushVec, VECTOR3 RefVec);

#if 0
	/// <summary>
	/// 途中で生成されたBallをリストに追加する
	/// </summary>
	/// <param name="ball">追加するBall</param>
	void AddBall(Ball* ball);

	void AddBox(Box* box);

	void AddArea(ScoreArea* area);
	/// <summary>
	/// listで今あるBallをすべて返す
	/// </summary>
	/// <returns>Ballの格納されたlist</returns>
	std::list<Ball*> GetBalls() { return balls; }

	std::list<Box*> GetBoxes() { return boxes; }

	std::list<ScoreArea*> GetAreaes() { return areaes; }

	/// <summary>
	/// Ballリストから指定されたBallがあれば消す
	/// </summary>
	/// <param name="ball">消すBall</param>
	void RemoveBall(Ball* ball);
	void BoxBallDestroy()
	{
		balls.clear();
		boxes.clear();
		areaes.clear();
	}
#endif
private:
	//std::list<Ball*> balls;
	std::list<Ball*> resolvedBalls;	// 衝突判定を終えたBallObject
	//std::list<Box*> boxes; 
	//std::list<ScoreArea*> areaes;
};