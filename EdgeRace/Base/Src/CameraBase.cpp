#include "CameraBase.h"
#include "Camera.h"
#include "Camera2.h"

CameraBase::CameraBase()
{
    // 左画面用のビューポート
    vp[0].TopLeftX = 0;
    vp[0].TopLeftY = 0;
    vp[0].Width = WINDOW_WIDTH / 2;  // 画面の左半分
    vp[0].Height = WINDOW_HEIGHT;
    vp[0].MinDepth = 0.0f;
    vp[0].MaxDepth = 1.0f;
    new Camera();

    // 右画面用のビューポート
    vp[1].TopLeftX = WINDOW_WIDTH / 2;
    vp[1].TopLeftY = 0;
    vp[1].Width = WINDOW_WIDTH / 2;  // 画面の右半分
    vp[1].Height = WINDOW_HEIGHT;
    vp[1].MinDepth = 0.0f;
    vp[1].MaxDepth = 1.0f;
    //for (int i = 0; i < 2; i++) {
    //    GameDevice()->m_pD3D->m_pDeviceContext->RSSetViewports(1, &vp[i]);
    //    // 各ビューポート用にカメラを切り替えて描画処理を行う
    //    // 描画するオブジェクトやシーンをここでレンダリング
    //    new Camera();
    //}
    new Camera2();

}

CameraBase::~CameraBase()
{
}

void CameraBase::Update()
{
    
}

void CameraBase::Draw()
{
    GameDevice()->m_pD3D->m_pDeviceContext->RSSetViewports(1, &vp[0]);
    GameDevice()->m_pD3D->m_pDeviceContext->RSSetViewports(1, &vp[1]);
}
