#include "Gizmo3D.h"
#include "EditorCamera.h"
#include "SplitScreen.h"

Gizmo3D::Gizmo3D()
{
	mesh = new CFbxMesh();
	mesh->Load("Data/Gizmo/gizmo.mesh");
	transform.scale = VECTOR3(0.15f, 0.15f, 0.15f);
	transform.rotation = VECTOR3(0, 180.0f / 180 * XM_PI, 0);
}

Gizmo3D::~Gizmo3D()
{
	SAFE_DELETE(mesh);
}

void Gizmo3D::Update()
{
	EditorCamera* cm = ObjectManager::FindGameObject<EditorCamera>();
	// 3DGizmo表示位置
	// 常にWindowの左下に配置する
	if (cm != nullptr)
	{
		SplitScreen* ss = ObjectManager::FindGameObject<SplitScreen>();
		if (ss->Multi())
		{
			VECTOR3 windowWorldPos = XMVector3Unproject(VECTOR3(100, WINDOW_HEIGHT - 100, 0.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, GameDevice()->m_mProj, cm->View(0), XMMatrixIdentity());
			transform.position = windowWorldPos;
		}
		else
		{
			VECTOR3 windowWorldPos = XMVector3Unproject(VECTOR3(100, WINDOW_HEIGHT - 100, 0.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, GameDevice()->m_mProj, GameDevice()->m_mView, XMMatrixIdentity());
			transform.position = windowWorldPos;

		}
	}
}
