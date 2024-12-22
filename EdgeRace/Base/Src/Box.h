#pragma once
/// <summary>
/// �����̂̍쐬
/// �T�C�Y���璸�_�Ӗʂ����߂�
/// �e�ʕӒ��_�Ƌ��̂̏Փ˔�����s��
/// </summary>
/// <author>S.Matsunaga</author>

#include "Object3D.h"
#include "BoxCollisionBase.h"

class Box : public BoxCollisionBase {
public:
	Box(VECTOR3 size = VECTOR3(1, 1, 1), VECTOR3 rot = VECTOR3(0, 0, 0));
	~Box();
	void Start() override;
	void Update() override;
	void Draw() override;

private:
	Object3D* child = nullptr;
};