#pragma once
/// <summary>
/// マップエディット機能
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
	/// オブジェクト未選択状態
	/// </summary>
	void NoneUpdate();

	/// <summary>
	/// オブジェクト選択状態
	/// </summary>
	void HasUpdate();

	/// <summary>
	/// 移動用Gizmo表示状態
	/// HasUpdate()の中で呼ぶ
	/// </summary>
	void PosGizmoUpdate();

	/// <summary>
	/// 回転用Gizmo表示状態
	/// </summary>
	void RotGizmoUpdate();
	
	/// <summary>
	/// スケール用Gizmo表示状態
	/// </summary>
	void ScaleGizmoUpdate();

	/// <summary>
	/// 操作中のGizmoを表示
	/// それ以外のGizmoを非表示
	/// </summary>
	/// <param name="gState">Gizmoステータス</param>
	void SetGizmo(int gState);
	
	/// <summary>
	/// オブジェクトが選択される時に呼ばれる
	/// </summary>
	/// <param name="ob">選択されたオブジェクト</param>
	void SelectObj(Object3D* ob);

	/// <summary>
	/// オブジェクトの選択が解除されたときに呼ばれる
	/// </summary>
	void DeselectObj();

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
	/// マウスカーソルをワールド座標変換
	/// ImGui上にカーソルがあるかどうかの判定も取る
	/// </summary>
	/// <returns>ImGuiの上にカーソルがあればfalse</returns>
	bool GetWorldPos();

private:
	// sステータス
	// オブジェクト選択状態の時ImGuiで位置、回転、サイズを変える
	enum NormalState
	{
		sNone = 0,	// 何も持っていない
		sHas,		// オブジェクト選択状態
		sGizmo,		// ギズモ選択状態
	}nState;

	// Gizmo関連のステータス
	enum GizmoState
	{
		sNoneGizmo = 0,	// ギズモ未選択
		sPosGizmo,		// 移動操作時
		sRotGizmo,		// 回転操作
		sScaleGizmo,	// スケール操作
	}gState;

	CDirectInput* pDI = GameDevice()->m_pDI;	// pDIショートカット

	// 各行列取得
	MATRIX4X4 mView;
	MATRIX4X4 mPrj;
	MATRIX4X4 identity;

	// 選択状態のオブジェクト
	Object3D* getObj = nullptr;	
	// 選択状態のGizmo
	Object3D* getGizmo = nullptr;

	// 左下の3DGizmo
	Object3D* gizmoObj = nullptr;	
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

	// マウススクリーン座標
	POINT mousePos = {};

	// 近視点
	VECTOR3 nearWorldPos;
	// 近視点からdirection方向に伸ばした点
	VECTOR3 extendedNearWorldPos;
	// 遠視点
	VECTOR3 farWorldPos;		
	// 近視点から遠視点への方向
	VECTOR3 direction;	
	// 1フレーム前のカーソル近視点座標
	VECTOR3 prevMousePos;

	// ImGu格納用変数
	VECTOR3 objPos;
	VECTOR3 objRot;
	VECTOR3 objScale;
	VECTOR3 moveVolumu = {};	// MoveBoxの移動量
	float moveSpeed = 0;	// MoveBoxの移動速度
	float tempE;
	float tempF;
	float tempMass;
	int pNum = 0;
	int stageNum = 0;

	bool judgeArea = true;	// ImGuiの判定エリア
	bool isNew = false;	// 新規オブジェクト生成か

	float test = 0;
};
