#pragma once
#include "Object3D.h"

class GizmoX;
class GizmoY;
class GizmoZ;

// �e�M�Y��
class GizmoXYZ : public Object3D
{
public:
	GizmoXYZ();
	~GizmoXYZ();
	void Update() override;
};

// �ړ��p�M�Y��
class PosGizmoX : public GizmoXYZ
{
public:
	PosGizmoX(Object3D* parent);
	~PosGizmoX();
	void Update() override;
};
class PosGizmoY : public GizmoXYZ
{
public:
	PosGizmoY(Object3D* parent);
	~PosGizmoY();
	void Update() override;
};
class PosGizmoZ : public GizmoXYZ
{
public:
	PosGizmoZ(Object3D* parent);
	~PosGizmoZ();
	void Update() override;
};

// ��]�p�M�Y��
class RotGizmoX : public GizmoXYZ
{
public:
	RotGizmoX(Object3D* parent);
	~RotGizmoX();
	void Update() override;
};
class RotGizmoY : public GizmoXYZ
{
public:
	RotGizmoY(Object3D* parent);
	~RotGizmoY();
	void Update() override;
};
class RotGizmoZ : public GizmoXYZ
{
public:
	RotGizmoZ(Object3D* parent);
	~RotGizmoZ();
	void Update() override;
};

// �T�C�Y�p�M�Y��
class ScaleGizmoX : public GizmoXYZ
{
public:
	ScaleGizmoX(Object3D* parent);
	~ScaleGizmoX();
	void Update() override;
};
class ScaleGizmoY : public GizmoXYZ
{
public:
	ScaleGizmoY(Object3D* parent);
	~ScaleGizmoY();
	void Update() override;
};
class ScaleGizmoZ : public GizmoXYZ
{
public:
	ScaleGizmoZ(Object3D* parent);
	~ScaleGizmoZ();
	void Update() override;
};
