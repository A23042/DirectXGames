#include "Score.h"

Score::Score()
{
}

Score::~Score()
{
}

void Score::Update()
{
	Clear();
	std::list<Object3D*> objs = ObjectManager::FindGameObjects<Object3D>();
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
	ImGui::SetNextWindowPos(ImVec2(WINDOW_WIDTH - 240, 80));
	ImGui::SetNextWindowSize(ImVec2(180, 80));
	ImGui::Begin("Score");
	ImGui::InputInt("P0", &p0Score);
	ImGui::InputInt("P1", &p1Score);
	ImGui::End();
}

void Score::Draw()
{
}
