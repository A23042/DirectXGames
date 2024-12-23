#pragma once
/// <summary>
/// StageEditで使用するGizmoのクラス
/// GizmoXYZをCenterに、親として他のGizmoを作る
/// </summary>
/// <author>S.Matsunaga</author>

#include "Object3D.h"



// 親ギズモ
class GizmoXYZ : public Object3D
{
public:
	GizmoXYZ();
	~GizmoXYZ();
	void Update() override;
};

// 移動用ギズモ
class PosGizmoX : public Object3D
{
public:
	PosGizmoX(Object3D* parent);
	~PosGizmoX();
	void Update() override;
};
class PosGizmoY : public Object3D
{
public:
	PosGizmoY(Object3D* parent);
	~PosGizmoY();
	void Update() override;
};
class PosGizmoZ : public Object3D
{
public:
	PosGizmoZ(Object3D* parent);
	~PosGizmoZ();
	void Update() override;
};

// 回転用ギズモ
class RotGizmoC : public Object3D
{
public:
	RotGizmoC(Object3D* parent);
	~RotGizmoC();
	void Update() override;
};
class RotGizmoX : public Object3D
{
public:
	RotGizmoX(Object3D* parent);
	~RotGizmoX();
	void Update() override;
};
class RotGizmoY : public Object3D
{
public:
	RotGizmoY(Object3D* parent);
	~RotGizmoY();
	void Update() override;
};
class RotGizmoZ : public Object3D
{
public:
	RotGizmoZ(Object3D* parent);
	~RotGizmoZ();
	void Update() override;
};

// サイズ用ギズモ
class ScaleGizmoX : public Object3D
{
public:
	ScaleGizmoX(Object3D* parent);
	~ScaleGizmoX();
	void Update() override;
};
class ScaleGizmoY : public Object3D
{
public:
	ScaleGizmoY(Object3D* parent);
	~ScaleGizmoY();
	void Update() override;
};
class ScaleGizmoZ : public Object3D
{
public:
	ScaleGizmoZ(Object3D* parent);
	~ScaleGizmoZ();
	void Update() override;
};