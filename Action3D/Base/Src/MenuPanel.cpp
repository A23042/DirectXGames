#include "MenuPanel.h"

MenePanel::MenePanel()
{
	BackGround = new CSprite();
	BackGroundImage = new CSpriteImage();
	BackGroundImage->Load("Data/Image/poko.png");
	BackGroundImage->m_dwImageHeight = WINDOW_HEIGHT;
	BackGroundImage->m_dwImageWidth = WINDOW_WIDTH;

	for (int i = 0; i < 5; i++)
	{
		Button[i] = new CSprite();
	}

	ObjectManager::SetDrawOrder(this, -1000);
}

MenePanel::~MenePanel()
{
}

void MenePanel::Update()
{
	auto pDI = GameDevice()->m_pDI;
	
	// DirectInput
	// コントローラーのLスティックの入力状態を取る
	float LX = pDI->GetJoyState().lX / 1000.0f;
	float LY = -pDI->GetJoyState().lY / 1000.0f;

	if (abs(LX) >= 0.8f)
	{
		time += SceneManager::DeltaTime();
		if (time >= moveTime)
		{
			if (LX > 0)
			{
				if (++selectNum >= 5)
				{
					selectNum = 0;
				}
			}
			else
			{
				if (--selectNum < 0)
				{
					selectNum = 4;
				}
			}
			time = 0;
		}
	}
}

void MenePanel::Draw()
{
	//BackGround->DrawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, RGB(150, 150, 150), 1.0f);
	BackGround->Draw(BackGroundImage, 0, 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT);

	for (int i = 0; i < 5; i++)
	{
		if (i == selectNum)
		{
			Button[i]->DrawRect(60 * (i + 1), 400, 30, 30, RGB(200, 0, 0), 1.0f);
		}
		else
		{
			Button[i]->DrawRect(60 * (i + 1), 400, 30, 30, RGB(0, 200, 0), 1.0f);
		}
	}
	//Button->DrawRect(100, 100, 30, 30, RGB(0, 200, 0), 1.0f);
}
