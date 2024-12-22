#include "EditUI.h"

EditUI::EditUI()
{
	spr = new CSprite();

	char name[64];
	for (int i = 0; i < 5; i++)
	{
		sprintf_s<64>(name, "Data/Image/EditText%02d.png", i);
		sprImages.push_back(new CSpriteImage());
		sprImages.back()->Load(name);
	}
	ObjectManager::SetDrawOrder(this, -1000);
}

EditUI::~EditUI()
{
	SAFE_DELETE(spr);
	auto itr = sprImages.begin();
	for (auto itr = sprImages.begin(); itr != sprImages.end(); itr++)
	{
		SAFE_DELETE(*itr);
	}
}

void EditUI::Draw()
{
	int i = 1;
	for (CSpriteImage* sprImage : sprImages)
	{
		spr->Draw(sprImage, WINDOW_WIDTH - sprImage->m_dwImageWidth - 20, WINDOW_HEIGHT - sprImage->m_dwImageHeight - sprImage->m_dwImageHeight * i, 0, 0, sprImage->m_dwImageWidth, sprImage->m_dwImageHeight);
		i++;
	}
}
