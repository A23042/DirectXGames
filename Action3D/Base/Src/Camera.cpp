#include "Camera.h"
#include "Player.h"
//                                      後方視点　　　　　　真上視点
static const VECTOR3 CameraPos[] = { VECTOR3(0, 2, -5), VECTOR3(0, 10, -0.5) };
static const VECTOR3 LookPos[] =   { VECTOR3(0, 1,  5), VECTOR3(0,  1,  1  ) };
static const float CHANGE_TIME_LIMIT = 0.5f; // 秒

Camera::Camera()
{
	SetDrawOrder(-1000);
	viewType = 0;
	changeTime = CHANGE_TIME_LIMIT; // 切り替えない
}

Camera::~Camera()
{
}

void Camera::Update()
{
	// ２つの視点を'L'キーによって切り替える
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_L)) {
		changePosStart = CameraPos[viewType];
		changeLookStart = LookPos[viewType];
		viewType += 1;
		if (viewType >= sizeof(CameraPos) / sizeof(CameraPos[0])) {
			viewType = 0;
		}
		changePosGoal = CameraPos[viewType];
		changeLookGoal = LookPos[viewType];
		changeTime = 0.0f;
	}

	// プレイヤーの行列を求める
	Player* player = ObjectManager::FindGameObject<Player>();
	MATRIX4X4 rotY = XMMatrixRotationY(player->Rotation().y);
	MATRIX4X4 trans = XMMatrixTranslation(
		player->Position().x, 0.0f, player->Position().z);
	MATRIX4X4 m = rotY * trans;
	// プレイヤーが回転・移動してない時のカメラ位置に
	// プレイヤーの回転・移動行列を掛けると、
	if (changeTime >= CHANGE_TIME_LIMIT) {
		transform.position = CameraPos[viewType] * m;
		lookPosition = LookPos[viewType] * m;
	}
	else { // 視点切り替え中
		changeTime += 1.0f / 60.0f;
		float timeRate = changeTime / CHANGE_TIME_LIMIT; // 0.0～1.0
		float rate = timeRate;
		VECTOR3 position = (changePosGoal - changePosStart) * rate + changePosStart;
		VECTOR3 look = (changeLookGoal - changeLookStart) * rate + changeLookStart;
		transform.position = position * m;
		lookPosition = look * m;
	}
	// カメラが壁にめり込まないようにする
	VECTOR3 start = player->Position() + VECTOR3(0, 1.5f, 0);
	VECTOR3 end = transform.position;
	// startからendに向かうベクトルを作り、長さに0.2を加える
	VECTOR3 camVec = end - start;
	camVec = XMVector3Normalize(camVec) * (camVec.Length() + 0.2f);
	end = start + camVec;

	std::list<Object3D*> grounds = ObjectManager::FindGameObjectsWithTag<Object3D>("STAGEOBJ");
	for (Object3D* g : grounds)
	{
		VECTOR3 hit;
		if (g->HitLineToMesh(start, end, &hit)) {
			end = hit;
		}
	}
	//endから0.2手前に置く;
	transform.position = XMVector3Normalize(camVec) * ((end - start).Length() - 0.2f)
																				+ start;
}

void Camera::Draw()
{
	GameDevice()->m_mView = XMMatrixLookAtLH(
		transform.position, // カメラ座標
		lookPosition, // 注視点
		VECTOR3(0, 1, 0));
}
