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
	/// �I�u�W�F�N�g���I����Ԃ�Update
	/// </summary>
	void NoneUpdate();

	/// <summary>
	/// �I�u�W�F�N�g�I����Ԃ�Update
	/// </summary>
	void HasUpdate();

	void GizmoUpdate();

	/// <summary>
	/// �ړ��pGizmo�\����Ԃ�Update
	/// HasUpdate()�̒��ŌĂ�
	/// </summary>
	void PosGizmoUpdate();

	/// <summary>
	/// ��]�pGizmo�\����Ԃ�Update
	/// </summary>
	void RotGizmoUpdate();
	
	/// <summary>
	/// �X�P�[���pGizmo�\����Ԃ�Update
	/// </summary>
	void ScaleGizmoUpdate();

	/// <summary>
	/// ���쒆��Gizmo��\��
	/// ����ȊO��Gizmo���\��
	/// </summary>
	void SetGizmo();
	
	/// <summary>
	/// �I�u�W�F�N�g���I������鎞�ɌĂ΂��
	/// </summary>
	/// <param name="ob">�I�����ꂽ�I�u�W�F�N�g</param>
	void SelectObj(Object3D* ob);

	/// <summary>
	/// �I�u�W�F�N�g�̑I������
	/// obj��null�̏ꍇ�I������Ă���I�u�W�F�N�g���ׂĉ���
	/// obj�̃f�t�H���g��nullptr
	/// </summary>
	/// <param name="obj">�I����������I�u�W�F�N�g</param>
	void DeselectObj(Object3D* obj = nullptr);

	/// <summary>
	/// �I������Ă���I�u�W�F�N�g�̍폜
	/// </summary>
	void DeleteObj();

	/// <summary>
	/// �I�u�W�F�N�g����
	/// [Ctrl + D]�őI������Ă���I�u�W�F�N�g�𕡐�
	/// �������I�u�W�F�N�g�̈ʒu�A��]�A�T�C�Y������
	/// </summary>
	/// <param name="ob">�������I�u�W�F�N�g</param>
	void CloneObj();

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
	/// ���Z�b�g����I�u�W�F�N�g�̔�������
	/// ����������폜
	/// </summary>
	/// <param name="obj">���ׂ�Object�̃��X�g</param>
	/// <returns></returns>
	void CheckResetObj(list<Object3D*> objs);

	/// <summary>
	/// �I�u�W�F�N�g�𐶐�ImGui
	/// </summary>
	void CreateObjImGui();

	void HierarchyImGui();

	void StageImGui();

	void Command();

	/// <summary>
	/// HitLineToMesh�ň�ԋ����̋߂��I�u�W�F�N�g��T��
	/// </summary>
	/// <param name="objs">�T������I�u�W�F�N�g���X�g</param>
	/// <returns>��ԋ����̋߂��I�u�W�F�N�g</returns>
	Object3D* GetClosestHitObject(list<Object3D*> objs, VECTOR3 &hit);

	/// <summary>
	/// �I�u�W�F�N�g�̒��S�_�����߂�
	/// </summary>
	/// <param name="objs">���߂�I�u�W�F�N�g���X�g</param>
	/// <returns>���S�_</returns>
	void GetObjCenter(list<Object3D*> objs);

	void GetNearWorldPosEx();

	/// <summary>
	/// �I�u�W�F�N�g�̃X�P�[���Ɖ�]��ύX���Ă���Œ�
	/// �J�[�\�������݂�Window����ł��甽�Α�����o��
	/// </summary>
	/// <returns>Window����o����</returns>
	bool CursorLoop();

	/// <summary>
	/// �}�E�X�J�[�\�������[���h���W�ϊ�
	/// ImGui��ɃJ�[�\�������邩�ǂ����̔�������
	/// </summary>
	void GetWorldPos();

	/// <summary>
	/// �J�[�\����ImGui��ɂȂ���
	/// </summary>
	/// <returns>ImGui�ゾ������false</returns>
	bool CheckInAreaCursor();

private:
	// s�X�e�[�^�X
	// �I�u�W�F�N�g�I����Ԃ̎�ImGui�ňʒu�A��]�A�T�C�Y��ς���
	enum NormalState
	{
		sNone = 0,	// ���������Ă��Ȃ�
		sHas,		// �I�u�W�F�N�g�I�����
		sGizmo,		// �M�Y���I�����
	}nState;

	// Gizmo�h���b�O���̃X�e�[�^�X
	enum GizmoState
	{
		sNoneGizmo = 0,	// �M�Y�����I��
		sPosGizmo,		// �ړ����쎞
		sRotGizmo,		// ��]����
		sScaleGizmo,	// �X�P�[������
	}gState;

	// �\������Gizmo
	enum GizmoView
	{
		vNone = 0,	// Gizmo��\��
		vPos,		// �ړ��p�\��
		vRot,		// ��]�p�\��
		vScale,		// �X�P�[���\��
	}vGizmo;

	CDirectInput* pDI = GameDevice()->m_pDI;	// pDI�V���[�g�J�b�g

	// �e�s��擾
	MATRIX4X4 mView;
	MATRIX4X4 mPrj;
	MATRIX4X4 identity;

	// �I����Ԃ̃I�u�W�F�N�g
	//Object3D* getObj = nullptr;	
	list<Object3D*> selectObj;

	// �I����Ԃ�Gizmo
	Object3D* getGizmo = nullptr;

	// TODO:�z��ɂ���
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
	Object3D* line = nullptr;

	// �}�E�X�X�N���[�����W
	POINT mousePos = {};

	// �ߎ��_
	VECTOR3 nearWorldPos;
	// �ߎ��_����direction�����ɐL�΂����_
	VECTOR3 nearWorldPosEx;
	// �����_
	VECTOR3 farWorldPos;		
	// �ߎ��_���牓���_�ւ̕���
	VECTOR3 direction;	
	// 1�t���[���O�̃J�[�\���ߎ��_���W
	VECTOR3 prevMousePos;

	VECTOR3 objCenter;	// �����I�u�W�F�N�g�̒��S�_
	list<VECTOR3> oldPos;
	list<VECTOR3> oldRot;
	list<VECTOR3> oldScale;

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
	bool isTestMap = false;	// TestMap�̕ҏW����
	bool isCursorLoop = false;
};
