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

	/// <summary>
	/// オブジェクト未選択状態
	/// </summary>
	void NoneUpdate();

	/// <summary>
	/// オブジェクト選択状態
	/// </summary>
	void HasUpdate();

	/// <summary>
	/// オブジェクト選択が選択される時に呼ばれる
	/// </summary>
	/// <param name="ob">選択されたオブジェクト</param>
	void SetObj(Object3D* ob);

	/// <summary>
	/// オブジェクト複製
	/// [Ctrl + D]で選択されているオブジェクトを複製
	/// 複製元オブジェクトの位置、回転、サイズを入れる
	/// </summary>
	/// <param name="ob">複製元オブジェクト</param>
	void DupeObj(Object3D* ob);

	/// <summary>
	/// ステージセーブ
	/// </summary>
	/// <param name="n">ステージ番号</param>
	void Save(int n);

	/// <summary>
	/// ステージ呼び出し
	/// </summary>
	/// <param name="n">ステージ番号</param>
	void Load(int n);

	/// <summary>
	/// ワールド座標変換
	/// </summary>
	void GetWorldPos();

private:
	// sステータス
	// オブジェクト選択状態の時ImGuiで位置、回転、サイズを変える
	enum NormalState
	{
		sNone = 0,	// 何も持っていない
		sHas,		// オブジェクト選択状態
	}nState;

	CDirectInput* pDI = GameDevice()->m_pDI;	// pDIショートカット

	// 各行列取得
	MATRIX4X4 mView;
	MATRIX4X4 mPrj;
	MATRIX4X4 identity;

	CsvReader* csv;
	// 選択状態のオブジェクト
	Object3D* getObj = nullptr;	
	// 3DGizmo
	Object3D* gizmoObj = nullptr;	
	// 3DGizmoXYZ
	Object3D* gizmoXYZ = nullptr;
	
	// マウススクリーン座標
	POINT mousePos;		

	// 近視点
	VECTOR3 nearWorldPos;
	// 遠視点
	VECTOR3 farWorldPos;	
	// 遠視点方向に距離を伸ばした点
	VECTOR3 extendedFarWorldPos;	
	// 方向
	VECTOR3 direction;	

	VECTOR3 objPos;
	VECTOR3 objRot;
	VECTOR3 objScale;
	float e;
	float f;
	float mass;

	int pNum = 0;
};
