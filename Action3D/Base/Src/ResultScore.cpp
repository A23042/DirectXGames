#include "ResultScore.h"

const float scoreViewTime = 0.5f;
const float scoreUpTime = 0.5f;

ResultScore::ResultScore()
{
	viewP0Score = 0;
	viewP1Score = 0;
	timer = 0.0f;
}

ResultScore::~ResultScore()
{
}

void ResultScore::Start()
{
	sc = ObjectManager::FindGameObject<Score>();
	//sc->CountScore();
}

void ResultScore::Update()
{
#if 1
	timer += SceneManager::DeltaTime();
	//if(sc == nullptr)
	//{
	//	sc = ObjectManager::FindGameObject<Score>();
	//}
	//else
	{
		if (timer >= scoreUpTime + scoreViewTime) {
			if (viewP0Score < sc->GetP0Score())
			{
				viewP0Score += 1;
			}
			if (viewP1Score < sc->GetP1Score())
			{
				viewP1Score += 1;
			}
		}
		if (viewP0Score >= sc->GetP0Score() && viewP1Score >= sc->GetP1Score()) {
			if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_T)) {
				SceneManager::ChangeScene("TitleScene");
			}
		}
	}
#endif
}

void ResultScore::Draw()
{
#if 1
	GameDevice()->m_pFont->Draw(400, 0, "RESULT", 64, RGB(255, 255, 255));
	if (timer >= scoreViewTime) {
		char strP0[64]; // •¶Žš—ñ‚ð—pˆÓ
		sprintf_s<64>(strP0, "Player0Score: %6d", viewP0Score);
		GameDevice()->m_pFont->Draw(400, 200, strP0, 64, RGB(255, 255, 255));
		
		char strP1[64]; // •¶Žš—ñ‚ð—pˆÓ
		sprintf_s<64>(strP1, "Player1Score: %6d", viewP1Score);
		GameDevice()->m_pFont->Draw(400, 400, strP1, 64, RGB(255, 255, 255));
	}
	if (viewP0Score >= sc->GetP0Score() && viewP1Score >= sc->GetP1Score()) {
		GameDevice()->m_pFont->Draw(400, 600, "PUSH T KEY", 64, RGB(255, 255, 255));
	}
#endif
}