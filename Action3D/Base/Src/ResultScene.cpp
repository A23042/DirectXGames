#include "ResultScene.h"
#include "ResultScore.h"

ResultScene::ResultScene()
{
	Instantiate<ResultScore>();
}

ResultScene::~ResultScene()
{
}

void ResultScene::Update()
{
}

void ResultScene::Draw()
{
	GameDevice()->m_pFont->Draw(
		20, 20, "ResultScene", 16, RGB(255, 0, 0));
}
