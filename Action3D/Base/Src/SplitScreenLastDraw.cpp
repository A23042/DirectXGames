#include "SplitScreenLastDraw.h"
#include "PlayScene.h"
#include "Camera.h"


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

}

SplitScreenLastDraw::~SplitScreenLastDraw()
{

}

void SplitScreenLastDraw::Update()
{
}


void SplitScreenLastDraw::Draw()
{

	SplitScreen* ss = ObjectManager::FindGameObject<SplitScreen>();
	Camera* cm = ObjectManager::FindGameObject<Camera>();
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
			//if (GameDevice()->m_pD3D->m_pTarget_DSTexDSV){
			//    GameDevice()->m_pD3D->m_pDeviceContext->ClearDepthStencilView(GameDevice()->m_pD3D->m_pTarget_DSTexDSV, D3D11_CLEAR_DEPTH, 1.0f, 0); // 深度バッファクリア
			//}

			GameDevice()->m_vEyePt = cm->EyePt(0); // カメラ座標
			GameDevice()->m_vLookatPt = cm->LookatPt(0); // 注視点
			GameDevice()->m_mView = cm->View(0);   // ビューマトリックス

			// -----------------------------------------------------------------


			// ここに最後に画面全体に描画したい処理を書く
			// 例えば、枠線スプライトや全体ステータスの描画など


			// -----------------------------------------------------------------
			GameDevice()->m_mProj = saveProj;	  // プロジェクションマトリックスを元に戻す
		}
	}

}
