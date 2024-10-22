#include "SplitScreen.h"
#include "PlayScene.h"
#include "Camera.h"
#include "SplitScreenLastDraw.h"

SplitScreen::SplitScreen()
{
	ObjectManager::DontDestroy(this);		// SplitScreen�͏�����Ȃ�
	ObjectManager::SetActive(this, false);		// SplitScreen�͍X�V���Ȃ�
	ObjectManager::SetDrawOrder(this, 100000);	// ��ԍŏ��ɕ`�揈������

	// ��ʕ����̍Ō�ɑS��ʂɕ`�悷�鏈���i�K�{�ł͂Ȃ��j
	//SingleInstantiate <SplitScreenLastDraw>();	// SplitScreenLastDraw�͈�������Ȃ��BNoDestroy�B


	D3D11_VIEWPORT vp = {};

	// �P���	--------------------------
	vpSingle = vp;
	vpSingle.Width = WINDOW_WIDTH;
	vpSingle.Height = WINDOW_HEIGHT;
	vpSingle.MinDepth = 0.0f;
	vpSingle.MaxDepth = 1.0f;
	vpSingle.TopLeftX = 0;
	vpSingle.TopLeftY = 0;


	// �����(�Q���)	---------------------------
	// ��������ʁ@[0]
	vpMulti.emplace_back(vp);
	vpMulti.back().Width = WINDOW_WIDTH / 2;
	vpMulti.back().Height = WINDOW_HEIGHT;
	vpMulti.back().MinDepth = 0.0f;
	vpMulti.back().MaxDepth = 1.0f;
	vpMulti.back().TopLeftX = 0;
	vpMulti.back().TopLeftY = 0;

	// �E������ʁ@[1]
	vpMulti.emplace_back(vp);
	vpMulti.back().Width = WINDOW_WIDTH / 2;
	vpMulti.back().Height = WINDOW_HEIGHT;
	vpMulti.back().MinDepth = 0.0f;
	vpMulti.back().MaxDepth = 1.0f;
	vpMulti.back().TopLeftX = WINDOW_WIDTH / 2;
	vpMulti.back().TopLeftY = 0;

	/*
		// �����(�S���)	---------------------------
		// ���㔼����ʁ@[0]
		vpMulti.emplace_back(vp);
		vpMulti.back().Width  = WINDOW_WIDTH/2;
		vpMulti.back().Height = WINDOW_HEIGHT/2;
		vpMulti.back().MinDepth = 0.0f;
		vpMulti.back().MaxDepth = 1.0f;
		vpMulti.back().TopLeftX = 0;
		vpMulti.back().TopLeftY = 0;

		// ����������ʁ@[1]
		vpMulti.emplace_back(vp);
		vpMulti.back().Width  = WINDOW_WIDTH/2;
		vpMulti.back().Height = WINDOW_HEIGHT/2;
		vpMulti.back().MinDepth = 0.0f;
		vpMulti.back().MaxDepth = 1.0f;
		vpMulti.back().TopLeftX = 0;
		vpMulti.back().TopLeftY = WINDOW_HEIGHT/2;

		// �E�㔼����ʁ@[2]
		vpMulti.emplace_back(vp);
		vpMulti.back().Width  = WINDOW_WIDTH/2;
		vpMulti.back().Height = WINDOW_HEIGHT/2;
		vpMulti.back().MinDepth = 0.0f;
		vpMulti.back().MaxDepth = 1.0f;
		vpMulti.back().TopLeftX = WINDOW_WIDTH/2;
		vpMulti.back().TopLeftY = 0;

		// �E��������ʁ@[3]
		vpMulti.emplace_back(vp);
		vpMulti.back().Width  = WINDOW_WIDTH/2;
		vpMulti.back().Height = WINDOW_HEIGHT/2;
		vpMulti.back().MinDepth = 0.0f;
		vpMulti.back().MaxDepth = 1.0f;
		vpMulti.back().TopLeftX = WINDOW_WIDTH/2;
		vpMulti.back().TopLeftY = WINDOW_HEIGHT/2;
	*/
	// -----------------------------------------------
	// �ŏ��͑S��ʂ��Z�b�g
	multi = false;
	multiNo = 0;
	GameDevice()->m_pD3D->m_pDeviceContext->RSSetViewports(1, &vpSingle);

}

SplitScreen::~SplitScreen()
{
}

void SplitScreen::Start()
{

}

void SplitScreen::SetSingleScreen()
{
	multi = false;
	GameDevice()->m_mProj = GameDevice()->m_mProjStandard;	  // �c���� �W��
	ObjectManager::SetDrawTimes(1);
}

void SplitScreen::SetMultiScreen()
{
	multi = true;
	GameDevice()->m_mProj = GameDevice()->m_mProjVerticalLong;	  // �c���� �c��(�Q��ʂ̂Ƃ�)
	//GameDevice()->m_mProj = GameDevice()->m_mProjStandard;	  // �c���� �W��
	ObjectManager::SetDrawTimes((int)vpMulti.size());
}


void SplitScreen::Draw()
{
	Camera* cm = ObjectManager::FindGameObject<Camera>();
	if (multi)
	{
		// �����
		GameDevice()->m_pD3D->m_pDeviceContext->RSSetViewports(1, &vpMulti[ObjectManager::DrawCounter()]);

		// ���̉��ObjectManager::DrawCount()�ɑΉ������J�������W��GameMain�ɐݒ肷��
		if (cm != nullptr)
		{
			GameDevice()->m_vEyePt = cm->EyePt(ObjectManager::DrawCounter()); // �J�������W
			GameDevice()->m_vLookatPt = cm->LookatPt(ObjectManager::DrawCounter()); // �����_
			GameDevice()->m_mView = cm->View(ObjectManager::DrawCounter());  // �r���[�}�g���b�N�X
		}
	}
	else
	{
		// �P���
		GameDevice()->m_pD3D->m_pDeviceContext->RSSetViewports(1, &vpSingle);

		// �J�������W��GameMain�ɐݒ肷��
		if (cm != nullptr)
		{
			GameDevice()->m_vEyePt = cm->EyePt(0); // �J�������W
			GameDevice()->m_vLookatPt = cm->LookatPt(0); // �����_
			GameDevice()->m_mView = cm->View(0);   // �r���[�}�g���b�N�X
		}
	}
}
