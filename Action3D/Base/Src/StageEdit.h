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
	void NoneUpdate();
	void HasUpdate();
	void SetObj(Object3D* ob);
	VECTOR3 GetWorldPos();
private:
	// �ʏ�̃X�e�[�^�X
	// �I�u�W�F�N�g�I����Ԃ̎�ImGui�ňʒu�A��]�A�T�C�Y��ς���
	enum NormalState
	{
		sNone = 0,	// ���������Ă��Ȃ�
		sHas,		// �I�u�W�F�N�g�I�����
	}nState;

	CsvReader* csv;
	//Object3D* obj = nullptr;
	Object3D* getObj = nullptr;

	POINT mousePos;

	VECTOR3 nearWorldPos;
	VECTOR3 farWorldPos;
	VECTOR3 extendedFarWorldPos;
	
	VECTOR3 direction;

	VECTOR3 objPos;
	int objPosX = 0;
	int objPosY = 0;
	int objPosZ = 0;

	VECTOR3 objRot;
	int objRotX = 0;
	int objRotY = 0;
	int objRotZ = 0;

	VECTOR3 objScale;
	int objScaleX = 0;
	int objScaleY = 0;
	int objScaleZ = 0;
};
