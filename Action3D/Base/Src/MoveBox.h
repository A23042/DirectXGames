#pragma once
#include "Box.h"

class MoveBox : public Box {
public:
	//MoveBox(float x, float y, float z, float rotX, float rotY, float rotZ);
	MoveBox(
		VECTOR3 size = VECTOR3(1, 1, 1),
		VECTOR3 rot = VECTOR3(0, 0, 0),
		VECTOR3 move = VECTOR3(0,0,0),
		VECTOR3 moveSpeed = VECTOR3(0, 0, 0)
	);
	~MoveBox();
	void Start() override;
	void Update() override;
	VECTOR3 ReflectionVec(PhysicsObject& pOgj, VECTOR3 normal) override;
private:
	VECTOR3 vPos;
	VECTOR3 pushVec;
	VECTOR3 refVec;

	XMMATRIX rotationMatrix;

	VECTOR3 moveMax, moveMin;	// à⁄ìÆêßå¿
	VECTOR3 move;
};