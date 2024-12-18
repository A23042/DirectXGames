#pragma once
#include "Object3D.h"
#include "Direct3D.h"

/// ��ʕ����N���X
/// </summary>
class SplitScreen : public Object3D
{
public:
	SplitScreen();
	~SplitScreen();

	void Start() override;
	void Draw() override;

	void SetSingleScreen();
	void SetMultiScreen();
	int  MultiSize() { return (int)vpMulti.size(); }
	bool Multi() { return multi; }
private:
	bool multi;
	D3D11_VIEWPORT vpSingle;
	std::vector<D3D11_VIEWPORT> vpMulti;
	int  multiNo;

};