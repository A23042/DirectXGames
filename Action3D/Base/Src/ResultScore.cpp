#include "ResultScore.h"
#include "Score.h"

const float scoreViewTime = 0.5f;
const float scoreUpTime = 0.5f;

ResultScore::ResultScore()
{
	viewScore = 0;
	timer = 0.0f;
}

ResultScore::~ResultScore()
{
}

void ResultScore::Update()
{
	timer += SceneManager::DeltaTime();
	Score* sc = ObjectManager::FindGameObject<Score>();
	if (timer >= scoreUpTime+scoreViewTime) {
		if (viewScore < sc->GetScore())
			viewScore += 1;
	}
	if (viewScore >= sc->GetScore()) {
		if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_T)) {
			SceneManager::ChangeScene("TitleScene");
		}
	}
}

void ResultScore::Draw()
{
	GameDevice()->m_pFont->Draw(400, 0, "RESULT", 64, RGB(255, 255, 255));
	if (timer >= scoreViewTime) {
		char str[64]; // •¶Žš—ñ‚ð—pˆÓ
		sprintf_s<64>(str, "SCORE: %6d", viewScore);
		GameDevice()->m_pFont->Draw(
			600, 200, str, 64, RGB(255, 255, 255));
	}
	Score* sc = ObjectManager::FindGameObject<Score>();
	if (viewScore >= sc->GetScore()) {
		GameDevice()->m_pFont->Draw(600, 0, "PUSH T KEY", 64, RGB(255, 255, 255));
	}
}