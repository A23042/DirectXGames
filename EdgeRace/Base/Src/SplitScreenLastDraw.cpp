#include "SplitScreenLastDraw.h"
#include "PlayScene.h"
#include "Camera.h"
#include "Score.h"
#include "SplitScreen.h"
#include "Gizmo3D.h"

const float scoreViewTime = 0.5f;
const float scoreUpTime = 0.1f;

SplitScreenLastDraw::SplitScreenLastDraw()
{
	ObjectManager::DontDestroy(this);		// 消されない
	ObjectManager::SetDrawOrder(this, -100000);	// 一番最後に描画処理する

	D3D11_VIEWPORT vp = {};

	// １画面	--------------------------
	vpSingle = vp;
	vpSingle.Width = WINDOW_WIDTH;
	vpSingle.Height = WINDOW_HEIGHT;
	vpSingle.MinDepth = 0.0f;
	vpSingle.MaxDepth = 1.0f;
	vpSingle.TopLeftX = 0;
	vpSingle.TopLeftY = 0;
	GameDevice()->m_pD3D->m_pDeviceContext->RSSetViewports(1, &vpSingle);

	sp = new CSprite;

	baseSpr = new CSprite();
	gageSpr = new CSprite();

	base = new CSpriteImage();
	gage = new CSpriteImage();
	redArea = new CSpriteImage();

	base->Load("Data/Image/base.png");
	gage->Load("Data/Image/gage.png");
	redArea->Load("Data/Image/redArea.png");

	posX = WINDOW_WIDTH - (WINDOW_WIDTH - base->m_dwImageWidth  / 2);
	posY = WINDOW_HEIGHT - 100;
	width = gage->m_dwImageWidth ;
	height = base->m_dwImageHeight ;
	redArea->m_dwImageWidth = WINDOW_WIDTH;
	redArea->m_dwImageHeight = WINDOW_HEIGHT;
}

SplitScreenLastDraw::~SplitScreenLastDraw()
{
	SAFE_DELETE(sp);
	SAFE_DELETE(baseSpr);
	SAFE_DELETE(gageSpr);
	SAFE_DELETE(base);
	SAFE_DELETE(gage);
	SAFE_DELETE(redArea);

}

void SplitScreenLastDraw::Start()
{
	score = ObjectManager::FindGameObject<Score>();
	ss = ObjectManager::FindGameObject<SplitScreen>();
	cm = ObjectManager::FindGameObject<Camera>();
	data = ObjectManager::FindGameObject<DataHolder>();
	//score->CountScore();
}

void SplitScreenLastDraw::Update()
{
	score->CountScore();
	if (!data->IsPlay())
	{
#if 1
		timer += SceneManager::DeltaTime();
		if (timer >= scoreUpTime + scoreViewTime) {
			upTimer += SceneManager::DeltaTime();
			if (upTimer >= scoreUpTime)
			{
				if (viewP0Score < score->GetP0Score())
				{
					viewP0Score += 1;
				}
				if (viewP1Score < score->GetP1Score())
				{
					viewP1Score += 1;
				}
				upTimer = 0.0f;
			}
		}
		if (viewP0Score >= score->GetP0Score() && viewP1Score >= score->GetP1Score()) {
			if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_T)) {
				SceneManager::ChangeScene("TitleScene");
			}
		}
#endif
	}
	else
	{
		timer = 0;
		viewP0Score = 0;
		viewP1Score = 0;
	}
}


void SplitScreenLastDraw::Draw()
{
	if (cm == nullptr)
	{
		cm = ObjectManager::FindGameObject<Camera>();
	}
	if (ss->Multi())
	{
		// 多画面のとき
		if (ObjectManager::DrawCounter() == ss->MultiSize() - 1)   // 最後の画面のとき
		{
			// １画面に戻す
			GameDevice()->m_pD3D->m_pDeviceContext->RSSetViewports(1, &vpSingle);
			MATRIX4X4 saveProj = GameDevice()->m_mProj;		 	  // プロジェクションマトリックスを待避
			GameDevice()->m_mProj = GameDevice()->m_mProjStandard;	  // 縦横比 標準

			// Ｚバッファのみ全クリヤーする。（画面は消さずに、Ｚバッファのみクリヤーする）
			// この処理は、ここで３Ｄオブジェクトの描画を行うときは必須
			// ２Ｄスプライトのみ描画を行うときは不要
			if (GameDevice()->m_pD3D->m_pTarget_DSTexDSV){
			    GameDevice()->m_pD3D->m_pDeviceContext->ClearDepthStencilView(GameDevice()->m_pD3D->m_pTarget_DSTexDSV, D3D11_CLEAR_DEPTH, 1.0f, 0); // 深度バッファクリア
			}

			if (cm != nullptr)
			{
				GameDevice()->m_vEyePt = cm->EyePt(0); // カメラ座標
				GameDevice()->m_vLookatPt = cm->LookatPt(0); // 注視点
				GameDevice()->m_mView = cm->View(0);   // ビューマトリックス
			}

			// -----------------------------------------------------------------


			// ここに最後に画面全体に描画したい処理を書く
			// 例えば、枠線スプライトや全体ステータスの描画など

			// PlaySceneとEditSceneで処理を分ける
			if (data != nullptr)
			{
				// ゲージ表示
				pls = ObjectManager::FindGameObjects<Player>();
				for (Player* pl : pls)
				{
					switch (pl->GetPlNum())
					{
					case 0:
						baseSpr->Draw(base, posX, posY, 0, 0, width, height);
						gageSpr->Draw(gage, posX, posY, 0, 0, width * pl->GetRate(0), height);
						break;
					case 1:
						baseSpr->Draw(base, WINDOW_WIDTH / 2 + posX, posY, 0, 0, width, height);
						gageSpr->Draw(gage, WINDOW_WIDTH / 2 + posX, posY, 0, 0, width * pl->GetRate(1), height);
						break;
					}
				}

				char strP0[64]; // 文字列を用意
				sprintf_s<64>(strP0, "P0Score: %2d", score->GetP0Score());
				GameDevice()->m_pFont->Draw(40, 40, strP0, 48, RGB(255, 255, 255));

				sprintf_s<64>(strP0, "P1Score: %2d", score->GetP1Score());
				GameDevice()->m_pFont->Draw(WINDOW_WIDTH - 280, 40, strP0, 48, RGB(255, 255, 255));

				// スコア表示
				if (!data->IsPlay())
				{
					sp->DrawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, RGB(0, 0, 0), 0.2f);

					float x = WINDOW_WIDTH / 3;
#if 1
					GameDevice()->m_pFont->Draw(x, 0, "RESULT", 64, RGB(255, 255, 255));
					if (timer >= scoreViewTime) {
						char strP0[64]; // 文字列を用意
						sprintf_s<64>(strP0, "Player0Score: %6d", viewP0Score);
						GameDevice()->m_pFont->Draw(x, 200, strP0, 64, RGB(255, 255, 255));

						char strP1[64]; // 文字列を用意
						sprintf_s<64>(strP1, "Player1Score: %6d", viewP1Score);
						GameDevice()->m_pFont->Draw(x, 400, strP1, 64, RGB(255, 255, 255));
					}
					if (viewP0Score >= score->GetP0Score() && viewP1Score >= score->GetP1Score()) {
						GameDevice()->m_pFont->Draw(x, 600, "PUSH R KEY", 64, RGB(255, 255, 255));
					}
				}
#endif
			}
			else
			{
			}

			//baseSpr->Draw(redArea, 0, 0, 0, 0, redArea->m_dwImageWidth, redArea->m_dwImageHeight, 0.6f);

			// -----------------------------------------------------------------
			GameDevice()->m_mProj = saveProj;	  // プロジェクションマトリックスを元に戻す
		}
	}

}
