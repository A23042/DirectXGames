#pragma once
#include "Object3D.h"
#include "CsvReader.h"

using namespace std;

class StageEdit : public Object3D {
public:
	StageEdit();
	~StageEdit();
	void Update() override;
	void Draw() override;

	/// <summary>
	/// �I�u�W�F�N�g���I�����
	/// </summary>
	void NoneUpdate();

	/// <summary>
	/// �I�u�W�F�N�g�I�����
	/// </summary>
	void HasUpdate();

	/// <summary>
	/// �I�u�W�F�N�g�I�����I������鎞�ɌĂ΂��
	/// </summary>
	/// <param name="ob">�I�����ꂽ�I�u�W�F�N�g</param>
	void SetObj(Object3D* ob);

	/// <summary>
	/// �I�u�W�F�N�g����
	/// [Ctrl + D]�őI������Ă���I�u�W�F�N�g�𕡐�
	/// �������I�u�W�F�N�g�̈ʒu�A��]�A�T�C�Y������
	/// </summary>
	/// <param name="ob">�������I�u�W�F�N�g</param>
	void DupeObj(Object3D* ob);

	/// <summary>
	/// �X�e�[�W�Z�[�u
	/// </summary>
	/// <param name="n">�X�e�[�W�ԍ�</param>
	void Save(int n);

	/// <summary>
	/// �X�e�[�W�Ăяo��
	/// </summary>
	/// <param name="n">�X�e�[�W�ԍ�</param>
	void Load(int n);

	/// <summary>
	/// ���[���h���W�ϊ�
	/// </summary>
	void GetWorldPos();

private:
	// s�X�e�[�^�X
	// �I�u�W�F�N�g�I����Ԃ̎�ImGui�ňʒu�A��]�A�T�C�Y��ς���
	enum NormalState
	{
		sNone = 0,	// ���������Ă��Ȃ�
		sHas,		// �I�u�W�F�N�g�I�����
	}nState;

	CDirectInput* pDI = GameDevice()->m_pDI;	// pDI�V���[�g�J�b�g

	// �e�s��擾
	MATRIX4X4 mView;
	MATRIX4X4 mPrj;
	MATRIX4X4 identity;

	CsvReader* csv;
	// �I����Ԃ̃I�u�W�F�N�g
	Object3D* getObj = nullptr;	
	// 3DGizmo
	Object3D* gizmoObj = nullptr;	
	// 3DGizmoXYZ
	Object3D* gizmoXYZ = nullptr;
	
	// �}�E�X�X�N���[�����W
	POINT mousePos;		

	// �ߎ��_
	VECTOR3 nearWorldPos;
	// �����_
	VECTOR3 farWorldPos;	
	// �����_�����ɋ�����L�΂����_
	VECTOR3 extendedFarWorldPos;	
	// ����
	VECTOR3 direction;	

	VECTOR3 objPos;
	VECTOR3 objRot;
	VECTOR3 objScale;
	float e;
	float f;
	float mass;

	int pNum = 0;
};
