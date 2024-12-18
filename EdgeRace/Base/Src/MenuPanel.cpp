#include "MenuPanel.h"

MenuPanel::MenuPanel()
{
	spr = new CSprite();

	BackGroundImage = new CSpriteImage();
	BackGroundImage->Load("Data/Image/BackGround.png");
	BackGroundImage->m_dwImageHeight = WINDOW_HEIGHT;
	BackGroundImage->m_dwImageWidth = WINDOW_WIDTH;

	char name[64];
	for (int i = 0; i < 3; i++)
	{
		sprintf_s<64>(name, "Data/Image/Text%02d.png", i);
		Button[i] = new CSprite();
		TextImage[i] = new CSpriteImage();
		TextImage[i]->Load(name);
	}

	ObjectManager::SetDrawOrder(this, -1000);
}

MenuPanel::~MenuPanel()
{
}

void MenuPanel::Update()
{
	auto pDI = GameDevice()->m_pDI;
	
	// DirectInput
	// コントローラーのLスティックの入力状態を取る
	float LX = pDI->GetJoyState().lX / 1000.0f;
	float LY = -pDI->GetJoyState().lY / 1000.0f;

	if (abs(LY) >= 0.8f)
	{
		time += SceneManager::DeltaTime();
		if (time >= moveTime)
		{
			if (LY < 0)
			{
				if (++selectNum >= 3)
				{
					selectNum = 0;
				}
			}
			else
			{
				if (--selectNum < 0)
				{
					selectNum = 2;
				}
			}
			time = 0;
		}
	}
}

void MenuPanel::Draw()
{
	//BackGround->DrawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, RGB(150, 150, 150), 1.0f);
	spr->Draw(BackGroundImage, 0, 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT);

	for (int i = 0; i < 3; i++)
	{
		if (i == selectNum)
		{
			Button[i]->DrawRect(40, 200 + TextImage[i]->m_dwImageHeight * (i + 1), 40, 40, RGB(200, 0, 0), 1.0f);
			spr->Draw(TextImage[i], 140, 200 + TextImage[i]->m_dwImageHeight * (i + 1), 0, 0, TextImage[i]->m_dwImageWidth, TextImage[i]->m_dwImageHeight);
		}
		else
		{
			Button[i]->DrawRect(40, 200 + TextImage[i]->m_dwImageHeight * (i + 1), 40, 40, RGB(0, 200, 0), 1.0f);
			spr->Draw(TextImage[i], 100, 200 + TextImage[i]->m_dwImageHeight * (i + 1), 0, 0, TextImage[i]->m_dwImageWidth, TextImage[i]->m_dwImageHeight);
		}
	}
	//Button->DrawRect(100, 100, 30, 30, RGB(0, 200, 0), 1.0f);
}
