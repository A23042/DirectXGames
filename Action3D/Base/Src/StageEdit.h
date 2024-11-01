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
	Object3D* getObj = nullptr;

	POINT mousePos;

	VECTOR3 nearWorldPos;
	VECTOR3 farWorldPos;
	VECTOR3 extendedFarWorldPos;
	
	bool hasObj = false;
};
