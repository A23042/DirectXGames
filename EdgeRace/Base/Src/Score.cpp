#include "Score.h"

Score::Score()
{
}

Score::~Score()
{
}

void Score::Update()
{
#if 0
	// ImGui表示用スコア
	Clear();
	std::list<Object3D*> balls = ObjectManager::FindGameObjectsWithTag<Object3D>("PLAYER");
	for (Object3D* obj : balls)
	{
		switch (obj->pObj.pNum)
		{
		case 0:
			p0ScoreTemp += obj->pObj.score;
			break;
		case 1:
			p1ScoreTemp += obj->pObj.score;
			break;
		default:
			break;
		}
	}
	ImGui::SetNextWindowPos(ImVec2(WINDOW_WIDTH - 240, 80));
	ImGui::SetNextWindowSize(ImVec2(180, 80));
	ImGui::Begin("Score");
	ImGui::InputInt("P0", &p0ScoreTemp);
	ImGui::InputInt("P1", &p1ScoreTemp);
	ImGui::End();
#endif
}

void Score::Draw()
{
}

void Score::CountScore()
{
	p0Score = 0;
	p1Score = 0;

	std::list<Object3D*> objs = ObjectManager::FindGameObjectsWithTag<Object3D>("PLAYER");
	for (Object3D* obj : objs)
	{
		switch (obj->pObj.pNum)
		{
		case 0:
			p0Score += obj->pObj.score;
			break;
		case 1:
			p1Score += obj->pObj.score;
			break;
		default:
			break;
		}
	}

}
