#include "ScoreDraw.h"
#include "Score.h"

ScoreDraw::ScoreDraw()
{
}

ScoreDraw::~ScoreDraw()
{
}

void ScoreDraw::Update()
{
}

void ScoreDraw::Draw()
{
#if 0
	Score* sc = ObjectManager::FindGameObject<Score>();
	char str[64]; // 文字列を用意
	sprintf_s<64>(str, "SCORE: %6d", sc->GetScore());
	GameDevice()->m_pFont->Draw(
		600, 20, str, 32, RGB(255, 255, 255));
#endif
}
