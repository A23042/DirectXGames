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
	/// オブジェクト未選択状態のUpdate
	/// </summary>
	void NoneUpdate();

	/// <summary>
	/// オブジェクト選択状態のUpdate
	/// </summary>
	void HasUpdate();

	void GizmoUpdate();

	/// <summary>
	/// 移動用Gizmo表示状態のUpdate
	/// HasUpdate()の中で呼ぶ
	/// </summary>
	void PosGizmoUpdate();

	/// <summary>
	/// 回転用Gizmo表示状態のUpdate
	/// </summary>
	void RotGizmoUpdate();
	
	/// <summary>
	/// スケール用Gizmo表示状態のUpdate
	/// </summary>
	void ScaleGizmoUpdate();

	/// <summary>
	/// 操作中のGizmoを表示
	/// それ以外のGizmoを非表示
	/// </summary>
	void SetGizmo();
	
	/// <summary>
	/// オブジェクトが選択される時に呼ばれる
	/// </summary>
	/// <param name="ob">選択されたオブジェクト</param>
	void SelectObj(Object3D* ob);

	/// <summary>
	/// オブジェクトの選択解除
	/// objがnullの場合選択されているオブジェクトすべて解除
	/// objのデフォルトはnullptr
	/// </summary>
	/// <param name="obj">選択解除するオブジェクト</param>
	void DeselectObj(Object3D* obj = nullptr);

	/// <summary>
	/// 選択されているオブジェクトの削除
	/// </summary>
	void DeleteObj();

	/// <summary>
	/// オブジェクト複製
	/// [Ctrl + D]で選択されているオブジェクトを複製
	/// 複製元オブジェクトの位置、回転、サイズを入れる
	/// </summary>
	/// <param name="ob">複製元オブジェクト</param>
	void CloneObj();

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
	/// リセットするオブジェクトの判定を取る
	/// 見つかったら削除
	/// </summary>
	/// <param name="obj">調べるObjectのリスト</param>
	/// <returns></returns>
	void CheckResetObj(list<Object3D*> objs);

	/// <summary>
	/// オブジェクトを生成ImGui
	/// </summary>
	void CreateObjImGui();

	void HierarchyImGui();

	void StageImGui();

	void Command();

	/// <summary>
	/// HitLineToMeshで一番距離の近いオブジェクトを探す
	/// </summary>
	/// <param name="objs">探索するオブジェクトリスト</param>
	/// <returns>一番距離の近いオブジェクト</returns>
	Object3D* GetClosestHitObject(list<Object3D*> objs, VECTOR3 &hit);

	/// <summary>
	/// オブジェクトの中心点を求める
	/// </summary>
	/// <param name="objs">求めるオブジェクトリスト</param>
	/// <returns>中心点</returns>
	void GetObjCenter(list<Object3D*> objs);

	void GetNearWorldPosEx();

	/// <summary>
	/// オブジェクトのスケールと回転を変更している最中
	/// カーソルが現在のWindowからでたら反対側から出す
	/// </summary>
	/// <returns>Windowから出たか</returns>
	bool CursorLoop();

	/// <summary>
	/// マウスカーソルをワールド座標変換
	/// ImGui上にカーソルがあるかどうかの判定も取る
	/// </summary>
	void GetWorldPos();

	/// <summary>
	/// カーソルがImGui上にないか
	/// </summary>
	/// <returns>ImGui上だったらfalse</returns>
	bool CheckInAreaCursor();

private:
	// sステータス
	// オブジェクト選択状態の時ImGuiで位置、回転、サイズを変える
	enum NormalState
	{
		sNone = 0,	// 何も持っていない
		sHas,		// オブジェクト選択状態
		sGizmo,		// ギズモ選択状態
	}nState;

	// Gizmoドラッグ中のステータス
	enum GizmoState
	{
		sNoneGizmo = 0,	// ギズモ未選択
		sPosGizmo,		// 移動操作時
		sRotGizmo,		// 回転操作
		sScaleGizmo,	// スケール操作
	}gState;

	// 表示するGizmo
	enum GizmoView
	{
		vNone = 0,	// Gizmo非表示
		vPos,		// 移動用表示
		vRot,		// 回転用表示
		vScale,		// スケール表示
	}vGizmo;

	CDirectInput* pDI = GameDevice()->m_pDI;	// pDIショートカット

	// 各行列取得
	MATRIX4X4 mView;
	MATRIX4X4 mPrj;
	MATRIX4X4 identity;

	// 選択状態のオブジェクト
	//Object3D* getObj = nullptr;	
	list<Object3D*> selectObj;

	// 選択状態のGizmo
	Object3D* getGizmo = nullptr;

	// TODO:配列にする
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
	Object3D* line = nullptr;

	// マウススクリーン座標
	POINT mousePos = {};

	// 近視点
	VECTOR3 nearWorldPos;
	// 近視点からdirection方向に伸ばした点
	VECTOR3 nearWorldPosEx;
	// 遠視点
	VECTOR3 farWorldPos;		
	// 近視点から遠視点への方向
	VECTOR3 direction;	
	// 1フレーム前のカーソル近視点座標
	VECTOR3 prevMousePos;

	VECTOR3 objCenter;	// 複数オブジェクトの中心点
	list<VECTOR3> oldPos;
	list<VECTOR3> oldRot;
	list<VECTOR3> oldScale;

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
	bool isTestMap = false;	// TestMapの編集中か
	bool isCursorLoop = false;
};
