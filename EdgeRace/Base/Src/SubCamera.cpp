#include "SubCamera.h"
#include "SplitScreen.h"

SubCamera::SubCamera(bool autoNum) : isAuto(autoNum)
{
	SetTag("CAMERA");
	mesh = new CFbxMesh();
	mesh->Load("Data/Gizmo/Camera.mesh");
	meshCol = new MeshCollider();
	meshCol->MakeFromMesh(mesh);
	editObj.name = "Camera";
	editObj.isScaling = false;
	//transform.position = VECTOR3(0, 5, -15);
	ObjectManager::AddObj(this);
	cmNum = ObjectManager::GetObjctList<SubCamera>().size();
}

SubCamera::~SubCamera()
{
	SAFE_DELETE(mesh);
	SAFE_DELETE(meshCol);
	ObjectManager::RemoveObj(this);
}

void SubCamera::Update()
{
	MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // YÇÃâÒì]çsóÒ
	MATRIX4X4 rot = XMMatrixRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z);
	transform.position = pObj.center;
	lookPosition = transform.position + VECTOR3(0, 0, 10) * rot;
	if (ss == nullptr)
	{
		ss = ObjectManager::FindGameObject<SplitScreen>();
	}

	if (isAuto)
	{
		int i = 0;
		for (SubCamera* camera : ObjectManager::GetObjctList<SubCamera>())
		{
			if (!ObjectManager::IsVisible(camera))
				continue;
			i++;
			if (camera == this)
			{
				cmNum = i;
				break;
			}
		}
	}
}

void SubCamera::Draw()
{
	if (!isAuto)
	{
		return;
	}

	if (ss != nullptr)
	{
		if (ss->Multi())
		{
			if (ObjectManager::DrawCounter() == 0)
			{
				mesh->Render(transform.matrix());
			}
		}
		else
		{
			mesh->Render(transform.matrix());
		}

	}
}
