#pragma once
#include "Object3D.h"

class GizmoX;
class GizmoY;
class GizmoZ;


class GizmoXYZ : public Object3D
{
public:
	GizmoXYZ();
	~GizmoXYZ();
	void Update() override;
};

class GizmoX : public GizmoXYZ
{
public:
	GizmoX();
	~GizmoX();
	void Update() override;
};

class GizmoY : public GizmoXYZ
{
public:
	GizmoY();
	~GizmoY();
	void Update() override;
};
class GizmoZ : public GizmoXYZ
{
public:
	GizmoZ();
	~GizmoZ();
	void Update() override;
};
