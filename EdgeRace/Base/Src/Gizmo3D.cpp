#include "Gizmo3D.h"

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
	// 3DGizmo�\���ʒu
	// ���Window�̍����ɔz�u����
	VECTOR3 windowWorldPos = XMVector3Unproject(VECTOR3(100, WINDOW_HEIGHT - 100, 0.0f), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, GameDevice()->m_mProj, GameDevice()->m_mView, XMMatrixIdentity());
	transform.position = windowWorldPos;
}
