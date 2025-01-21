#include "MainCamera.h"

MainCamera::MainCamera()
{
	transform.position = VECTOR3(0, 5, -15);
	lookPosition = transform.position + VECTOR3(0, -3, 10);
	direction = VECTOR3(0, 0, 0);
	moveSpeed = 0.5f;
	rotationSpeed = 1.2f;
}

MainCamera::~MainCamera()
{
}

void MainCamera::Update()
{
	direction = VECTOR3(0, 0, 0);	// 方向リセット
	MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Yの回転行列
	MATRIX4X4 rot = XMMatrixRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z);

	// 移動速度、視点速度の設定
	ImGui::SetNextWindowPos(ImVec2(WINDOW_WIDTH - 300, 50));
	ImGui::SetNextWindowSize(ImVec2(280, 100));
	ImGui::Begin("SETTING");
	ImGui::SliderFloat("MoveSpeed", &moveSpeed, 0.1f, 1.0f, "%.1f");
	ImGui::SliderFloat("CameraSpeed", &rotationSpeed, 0.1f, 2.0f, "%.1f");
	ImGui::End();

	// 視点移動
	// カーソルの1フレーム前の座標との差で視点を動かす
	// 右クリックしてる時だけ移動操作可能（参考：Unityのエディタ）
	if (pDI->CheckMouse(KD_DAT, 1))
	{
		mousePos1 = pDI->GetMousePos();

		float diffX = mousePos1.x - mousePos0.x;
		float diffY = mousePos1.y - mousePos0.y;
		transform.rotation.y += (diffX / 10) * rotationSpeed / 180.0f * XM_PI;
		transform.rotation.x += (diffY / 10) * rotationSpeed / 180.0f * XM_PI;

		// カメラ移動処理
		if (pDI->CheckKey(KD_DAT, DIK_W))
		{
			direction += VECTOR3(0, 0, 1) * rot;
		}
		else if (pDI->CheckKey(KD_DAT, DIK_S))
		{
			direction += VECTOR3(0, 0, -1) * rot;
		}
		if (pDI->CheckKey(KD_DAT, DIK_D))
		{
			direction += VECTOR3(1, 0, 0) * rot;
		}
		else if (pDI->CheckKey(KD_DAT, DIK_A))
		{
			direction += VECTOR3(-1, 0, 0) * rot;
		}
		if (pDI->CheckKey(KD_DAT, DIK_E))
		{
			direction += VECTOR3(0, 1, 0) * rot;
		}
		else if (pDI->CheckKey(KD_DAT, DIK_Q))
		{
			direction += VECTOR3(0, -1, 0) * rot;
		}
	}

	VECTOR3 noramalVelocity = normalize(direction);	// 移動方向に移動速度をかける
	transform.position += noramalVelocity * moveSpeed;	// 移動
	lookPosition = transform.position + VECTOR3(0, -3, 10) * rot;	// 注視点方向にマトリクスをかける
	mousePos0 = pDI->GetMousePos();	// 1フレーム前のマウス座標保管
}
