#pragma once
/// <summary>
/// �}�b�v�G�f�B�b�g�@�\
/// </summary>
/// <author>S.Matsunaga</author>


#include "Object3D.h"
#include "CsvReader.h"

using namespace std;
using namespace ObjectManager;

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
	/// �ړ��pGizmo�\�����
	/// HasUpdate()�̒��ŌĂ�
	/// </summary>
	void PosGizmoUpdate();

	/// <summary>
	/// ��]�pGizmo�\�����
	/// </summary>
	void RotGizmoUpdate();
	
	/// <summary>
	/// �X�P�[���pGizmo�\�����
	/// </summary>
	void ScaleGizmoUpdate();

	/// <summary>
	/// ���쒆��Gizmo��\��
	/// ����ȊO��Gizmo���\��
	/// </summary>
	/// <param name="gState">Gizmo�X�e�[�^�X</param>
	void SetGizmo(int gState);
	
	/// <summary>
	/// �I�u�W�F�N�g���I������鎞�ɌĂ΂��
	/// </summary>
	/// <param name="ob">�I�����ꂽ�I�u�W�F�N�g</param>
	void SelectObj(Object3D* ob);

	/// <summary>
	/// �I�u�W�F�N�g�̑I�����������ꂽ�Ƃ��ɌĂ΂��
	/// </summary>
	void DeselectObj();

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
	/// �}�E�X�J�[�\�������[���h���W�ϊ�
	/// ImGui��ɃJ�[�\�������邩�ǂ����̔�������
	/// </summary>
	/// <returns>ImGui�̏�ɃJ�[�\���������false</returns>
	bool GetWorldPos();

private:
	// s�X�e�[�^�X
	// �I�u�W�F�N�g�I����Ԃ̎�ImGui�ňʒu�A��]�A�T�C�Y��ς���
	enum NormalState
	{
		sNone = 0,	// ���������Ă��Ȃ�
		sHas,		// �I�u�W�F�N�g�I�����
		sGizmo,		// �M�Y���I�����
	}nState;

	// Gizmo�֘A�̃X�e�[�^�X
	enum GizmoState
	{
		sNoneGizmo = 0,	// �M�Y�����I��
		sPosGizmo,		// �ړ����쎞
		sRotGizmo,		// ��]����
		sScaleGizmo,	// �X�P�[������
	}gState;

	CDirectInput* pDI = GameDevice()->m_pDI;	// pDI�V���[�g�J�b�g

	// �e�s��擾
	MATRIX4X4 mView;
	MATRIX4X4 mPrj;
	MATRIX4X4 identity;

	// �I����Ԃ̃I�u�W�F�N�g
	Object3D* getObj = nullptr;	
	// �I����Ԃ�Gizmo
	Object3D* getGizmo = nullptr;

	// ������3DGizmo
	Object3D* gizmoObj = nullptr;	
	// 3DGizmoXYZ
	Object3D* gizmoC = nullptr;
	Object3D* posGizmoX = nullptr;
	Object3D* posGizmoY = nullptr;
	Object3D* posGizmoZ = nullptr;
	Object3D* rotGizmoC = nullptr;
	Object3D* rotGizmoX = nullptr;
	Object3D* rotGizmoY = nullptr;
	Object3D* rotGizmoZ = nullptr;
	Object3D* scaleGizmoX = nullptr;
	Object3D* scaleGizmoY = nullptr;
	Object3D* scaleGizmoZ = nullptr;
	Object3D* fallCheck = nullptr;

	// �}�E�X�X�N���[�����W
	POINT mousePos = {};

	// �ߎ��_
	VECTOR3 nearWorldPos;
	// �ߎ��_����direction�����ɐL�΂����_
	VECTOR3 extendedNearWorldPos;
	// �����_
	VECTOR3 farWorldPos;		
	// �ߎ��_���牓���_�ւ̕���
	VECTOR3 direction;	
	// 1�t���[���O�̃J�[�\���ߎ��_���W
	VECTOR3 prevMousePos;

	// ImGu�i�[�p�ϐ�
	VECTOR3 objPos;
	VECTOR3 objRot;
	VECTOR3 objScale;
	VECTOR3 moveVolumu = {};	// MoveBox�̈ړ���
	float moveSpeed = 0;	// MoveBox�̈ړ����x
	float tempE;
	float tempF;
	float tempMass;
	int pNum = 0;
	int stageNum = 0;

	bool judgeArea = true;	// ImGui�̔���G���A
	bool isNew = false;	// �V�K�I�u�W�F�N�g������

	float test = 0;
};
