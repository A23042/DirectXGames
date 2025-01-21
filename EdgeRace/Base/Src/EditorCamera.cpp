#include "EditorCamera.h"
#include "SplitScreen.h"
#include "SubCamera.h"
#include "MainCamera.h"

EditorCamera::EditorCamera()
{
	transform.position = VECTOR3(0, 5, -15);
	lookPosition = transform.position + VECTOR3(0, -3, 10);
	GameDevice()->m_mView = XMMatrixLookAtLH(
		transform.position, // カメラ座標
		lookPosition, // 注視点
		VECTOR3(0, 1, 0));

	direction = VECTOR3(0, 0, 0);
	moveSpeed = 0.5f;
	rotationSpeed = 1.2f;
	cameraPos = transform.position;

	//float fAspect = (float)WINDOW_WIDTH / WINDOW_HEIGHT;
	//GameDevice()->m_mProj = XMMatrixPerspectiveFovLH(XMConvertToRadians(50.0f), fAspect, 0.1f, 500.0f);
}

EditorCamera::~EditorCamera()
{
}

void EditorCamera::Start()
{
	ss = ObjectManager::FindGameObject<SplitScreen>();
	mainCam = ObjectManager::FindGameObject<MainCamera>();
	while (view.size() < ss->MultiSize())
	{
		MATRIX4X4 m = XMMatrixIdentity();
		view.emplace_back(m);
		VECTOR3 v = VECTOR3(0, 0, 0);
		eyePt.emplace_back(v);
		lookatPt.emplace_back(v);
	}

}

void EditorCamera::Update()
{

	if (ss->Multi())
	{
		for (int i = 0; i < ss->MultiSize(); i++)
		{
			// ２画面のとき
			switch (i)
			{
			case 0:
				transform.position = mainCam->Position();
				lookPosition = mainCam->GetLookPos();

				eyePt[i] = transform.position;   // カメラ座標
				lookatPt[i] = lookPosition;      // 注視点
				view[i] = XMMatrixLookAtLH(	    // ビューマトリックス
					transform.position,
					lookPosition,
					VECTOR3(0, 1, 0));
				break;
			case 1:
				SubCamera * sub = ObjectManager::FindGameObject<SubCamera>();
				transform.position = sub->Position();
				lookPosition = sub->GetLookPos();

				eyePt[i] = transform.position;   // カメラ座標
				lookatPt[i] = VECTOR3();      // 注視点
				view[i] = XMMatrixLookAtLH(	    // ビューマトリックス
					transform.position,
					lookPosition,
					VECTOR3(0, 1, 0));
				break;
			}
		}
	}
	else
	{
		transform.position = mainCam->Position();
		lookPosition = mainCam->GetLookPos();

		eyePt[0] = transform.position;   // カメラ座標
		lookatPt[0] = lookPosition;      // 注視点
		view[0] = XMMatrixLookAtLH(	    // ビューマトリックス
			transform.position,
			lookPosition,
			VECTOR3(0, 1, 0));
	}

	
}

void EditorCamera::Draw()
{
	GameDevice()->m_mView = XMMatrixLookAtLH(
		transform.position, // カメラ座標
		lookPosition, // 注視点
		VECTOR3(0, 1, 0));
}
