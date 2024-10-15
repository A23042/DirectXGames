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
	VECTOR3 GetWorldPos();
private:
	CsvReader* csv;
	Object3D* obj = nullptr;
	VECTOR3 worldPos;
	POINT mousePos;
	VECTOR2 mPos;
	VECTOR3 screenPos;

	VECTOR3 startPos, endPos;	// start‚©‚çend‚Ü‚ÅRay‚ð”ò‚Î‚·
};
