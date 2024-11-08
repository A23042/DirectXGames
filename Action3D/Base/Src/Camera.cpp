#include "Camera.h"
#include "Player.h"
#include "Ball.h"
//                                      後方視点　　　　　　真上視点
static const VECTOR3 CameraPos[] = { VECTOR3(0, 4, -10), VECTOR3(0, 10, -0.5) };
static const VECTOR3 LookPos[] =   { VECTOR3(0, 0,  5), VECTOR3(0,  1,  1  ) };
static const float CHANGE_TIME_LIMIT = 0.5f; // 秒

Camera::Camera()
{
	ObjectManager::SetVisible(this, false);	// 自体非表示
	SetDrawOrder(-1000);	// 最後に処理する
	viewType = 0;
	changeTime = CHANGE_TIME_LIMIT; // 切り替えない

	ssObj = ObjectManager::FindGameObject<SplitScreen>();
	while (view.size() < ssObj->MultiSize())
	{
		MATRIX4X4 m = XMMatrixIdentity();
		view.emplace_back(m);
		VECTOR3 v = VECTOR3(0, 0, 0);
		eyePt.emplace_back(v);
		lookatPt.emplace_back(v);
	}
}

Camera::~Camera()
{
}

void Camera::Update()
{
	if (ssObj->Multi())
	{
		std::list<Player*> pcs = ObjectManager::FindGameObjects<Player>();
		for(Player* pc : pcs)
		// 多画面のとき
		for (int i = 0; i < ssObj->MultiSize(); i++)
		{

			// ２画面のとき
			switch (i)
			{
			case 0:
			{
				// 画面0はPlayer1視点
				//Player* pc = ObjectManager::FindGameObject<Player>();
					if (pc->GetPlNum() == 0)
					{
						if (pc != nullptr)
						{
							updateCamera(i, pc->Position(), pc->Rotation());
						}
						else {
							updateCamera(i, VECTOR3(0, 0, 0), VECTOR3(0, 0, 0));
						}
					}
			}
			break;

			case 1:
			{
				// 画面1はPlayer2視点
				
				if (pc->GetPlNum() == 1)
				{
#if 1
					if (pc != nullptr)
					{
						updateCamera(i, pc->Position(), pc->Rotation());
					}
					else {
						updateCamera(i, VECTOR3(0, 0, 0), VECTOR3(0, 0, 0));
					}
#else
					updateCamera(i, VECTOR3(0, 2, -15), VECTOR3(0, 0, 0));
#endif
				}
			}
			break;
			}

			/*
			// ４画面のとき
			switch (i)
			{
			case 0:
				{
					// 画面0はPlayer視点
					Player* pc = ObjectManager::FindGameObject<Player>();
					if (pc != nullptr)
					{
						updateCamera(i, pc->Position(), pc->Rotation());
					}
					else {
						updateCamera(i, VECTOR3(0,0,0), VECTOR3(0,0,0));
					}
				}
				break;

			case 1:
				{
					// 画面1はEnemyRS視点
					EnemyRS* enm = ObjectManager::FindGameObject<EnemyRS>();
					if (enm != nullptr && enm->Mesh() != nullptr)
					{
						updateCamera(i, enm->Position(), enm->Rotation());
					}
					else {
						updateCamera(i, VECTOR3(0, 0, 0), VECTOR3(0, 0, 0));
					}
				}
				break;

			case 2:
				{
					// 画面2はEnemyGolem視点
					EnemyGolem* enm = ObjectManager::FindGameObject<EnemyGolem>();
					if (enm != nullptr && enm->Mesh() != nullptr)
					{
						updateCamera(i, enm->Position(), enm->Rotation());
					}
					else {
						updateCamera(i, VECTOR3(0, 0, 0), VECTOR3(0, 0, 0));
					}
				}
				break;

			case 3:
				{
					// 画面3は原点視点
					updateCamera(i, VECTOR3(0, 0, 0), VECTOR3(0, 0, 0));
				}
				break;
			}
			*/
		}
	}
	else {
		// １画面のときPlayer視点
		Player* pc = ObjectManager::FindGameObject<Player>();
		if (pc != nullptr)
		{
			updateCamera(0, pc->Position(), pc->Rotation());
		}
		else {
			updateCamera(0, VECTOR3(0, 0, 0), VECTOR3(0, 0, 0));
		}
	}
}

void Camera::Draw()
{
	GameDevice()->m_mView = XMMatrixLookAtLH(
		transform.position, // カメラ座標
		lookPosition, // 注視点
		VECTOR3(0, 1, 0));
}

void Camera::updateCamera(int counter, VECTOR3 pos, VECTOR3 rot)
{
			// プレイヤーの行列を求める
			MATRIX4X4 rotY = XMMatrixRotationY(rot.y);
			MATRIX4X4 trans = XMMatrixTranslation(pos.x, pos.y / 2.0f + 0.0f, pos.z);
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
			VECTOR3 start = pos;
			VECTOR3 end = transform.position;
			// startからendに向かうベクトルを作り、長さに0.2を加える
			VECTOR3 camVec = end - start;
			camVec = XMVector3Normalize(camVec) * (camVec.Length() + 0.2f);
			end = start + camVec;
			{

				std::list<Object3D*> objList = ObjectManager::FindGameObjectsWithTag<Object3D>("STAGEOBJ");
				for (Object3D* g : objList)
				{
					VECTOR3 hit;
					if (g->HitLineToMesh(start, end, &hit)) {
						end = hit;
					}
				}
			}
			//endから0.2手前に置く;
			transform.position = XMVector3Normalize(camVec) * ((end - start).Length() - 0.2f) + start;


			// ------------------------------------------------------------------
			// カメラ座標を配列に設定する
			eyePt[counter] = transform.position;   // カメラ座標
			lookatPt[counter] = lookPosition;      // 注視点
			view[counter] = XMMatrixLookAtLH(	    // ビューマトリックス
				transform.position,
				lookPosition,
				VECTOR3(0, 1, 0));

			// ------------------------------------------------------------------
			// 視点からの距離の２乗をDrawObjectに設定する
			// これは、視点からの距離の降順に描画したいため
			{
				std::list<Object3D*> objList = ObjectManager::FindGameObjects<Object3D>();
				for (Object3D*& obj : objList)
				{
					if (obj != this)
					{
						float distQ = magnitudeSQ(obj->Position() - transform.position);
						ObjectManager::SetEyeDist(obj, distQ, counter);  // 視点からの距離の２乗をDrawObjectに設定
					}
				}
			}
}
