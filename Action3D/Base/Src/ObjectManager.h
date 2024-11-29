#pragma once
/// <summary>
/// すべてのオブジェクトを管理します
/// シングルトンにしている
/// ここからすべてのGameObjectのUpdate()/Draw()を呼び出します
/// </summary>
/// <author>N.Hanai</author>

#include <list>
#include <string>
class GameObject;
class ScoreArea;

namespace ObjectManager {
	void Start();
	void Update();
	void Draw();
	void Release();
	void ChangeScene();

	/// <summary>
	/// Objectを追加する
	/// この関数は、GameObjectのコンストラクタから呼ばれる
	/// </summary>
	/// <param name="obj"></param>
	void Push(GameObject* obj);

	/// <summary>
	/// Objectを削除する
	/// この関数では、削除の要求をするだけで、
	/// 実際に削除されるのは、Update()が呼ばれる直前
	/// </summary>
	/// <param name="obj"></param>
	void Destroy(GameObject* obj);

	std::list<GameObject*> GetAllObjects();

	/// <summary>
	/// クラス名でオブジェクトを探す
	/// </summary>
	/// <typeparam name="C">クラス</typeparam>
	/// <returns>オブジェクトの実態（存在しなければnullptr）</returns>
	template<class C> C* FindGameObject()
	{
		const std::list<GameObject*> objs = GetAllObjects();

		for (GameObject* node : objs) {
			C* obj = dynamic_cast<C*>(node);
			if (obj != nullptr)
				return obj;
		}
		return nullptr;
	}

	/// 2024.11.20 S.Matsunaga
	/// <summary>
	/// クラス名で自分以外のオブジェクトを探す
	/// </summary>
	/// <typeparam name="C">クラス</typeparam>
	/// <param name="me">自分</param>
	/// <returns>オブジェクトの実態（存在しなければnullptr）</returns>
	template<class C> C* FindOtherGameObject(GameObject* me)
	{
		const std::list<GameObject*> objs = GetAllObjects();

		for (GameObject* node : objs) 
		{
			C* obj = dynamic_cast<C*>(node);
			if (obj != nullptr)
			{
				if (obj != me)
				{
					return obj;
				}
			}
		}
		return nullptr;
	}


	/// <summary>
	/// クラスのオブジェクトをすべて探す
	/// </summary>
	/// <typeparam name="C">クラス名</typeparam>
	/// <returns>オブジェクトの実態list</returns>
	template<class C> std::list<C*> FindGameObjects()
	{
		std::list<C*> out;
		out.clear();

		const std::list<GameObject*> objs = GetAllObjects();

		for (GameObject* node : objs) {
			C* obj = dynamic_cast<C*>(node);
			if (obj != nullptr)
				out.emplace_back(obj);
		}
		return out;
	}

	/// <summary>
	/// クラス名とタグからオブジェクトを探す
	/// </summary>
	/// <typeparam name="C">クラス名</typeparam>
	/// <param name="tag">タグ</param>
	/// <returns>オブジェクトの実態（存在しなければnullptr）</returns>
	template<class C> C* FindGameObjectWithTag(std::string tag)
	{
		const std::list<GameObject*> objs = GetAllObjects();

		for (GameObject* node : objs) {
			C* obj = dynamic_cast<C*>(node);
			if (obj != nullptr) {
				if (obj->IsTag(tag))
					return obj;
			}
		}
		return nullptr;
	}

	/// <summary>
	/// クラス名とタグからオブジェクトをすべて探す
	/// </summary>
	/// <typeparam name="C">クラス名</typeparam>
	/// <param name="tag">タグ</param>
	/// <returns>オブジェクトの実態list</returns>
	template<class C> std::list<C*> FindGameObjectsWithTag(std::string tag)
	{
		std::list<C*> out;
		out.clear();

		const std::list<GameObject*> objs = GetAllObjects();

		for (GameObject* node : objs) {
			C* obj = dynamic_cast<C*>(node);
			if (obj != nullptr) {
				if (obj->IsTag(tag))
					out.emplace_back(obj);
			}
		}
		return out;
	}

	// 2024.11.06 S.Matsunaga
	/// <summary>
	/// クラス名と探さないタグ以外オブジェクトをすべて探す
	/// </summary>
	/// <typeparam name="C">クラス名</typeparam>
	/// <param name="tag">除外するタグ</param>
	/// <returns>オブジェクトの実態list</returns>
	template<class C> std::list<C*> FindGameObjectsWithOutTag(std::string tag)
	{
		std::list<C*> out;
		out.clear();

		const std::list<GameObject*> objs = GetAllObjects();

		for (GameObject* node : objs) {
			C* obj = dynamic_cast<C*>(node);
			if (obj != nullptr) {
				if (!obj->IsTag(tag))
					out.emplace_back(obj);
			}
		}
		return out;
	}

	/// <summary>
	/// 描画のプライオリティを設定する
	/// 数値が少ない順に描画されるので、２Ｄでは奥に表示される
	/// ２Ｄで手前に表示したい時、３Ｄで後に描画したい時は、値を高くする
	/// プライオリティが同じものの順番は保証されない
	/// プライオリティのデフォルトは０です
	/// </summary>
	/// <param name="obj">プライオリティを設定するオブジェクト</param>
	/// <param name="order">描画プライオリティ</param>
	void SetDrawOrder(GameObject* _obj, int _order);

	/// <summary>
	/// Updateの優先順位を付ける
	/// </summary>
	/// <param name="_obj"></param>
	/// <param name="_priority"></param>
	void SetPriority(GameObject* _obj, int _priority);

	/// <summary>
	/// GameObjectを削除する
	/// </summary>
	/// <param name="obj">GameObjectのインスタンス</param>
	void DeleteGameObject(GameObject* obj);

	/// <summary>
	/// 全てのGameObjectを削除する
	/// </summary>
	void DeleteAllGameObject();

	void DontDestroy(GameObject* obj, bool dont = true);

	/// <summary>
	/// Updateを実行するか設定する
	/// </summary>
	/// <param name="obj">GameObjectのインスタンス</param>
	/// <param name="active">実行する場合はtrue</param>
	void SetActive(GameObject* obj, bool active = true);

	/// <summary>
	/// Drawを実行するか設定する
	/// </summary>
	/// <param name="obj">GameObjectのインスタンス</param>
	/// <param name="visible">Drawする場合はtrue</param>
	void SetVisible(GameObject* obj, bool visible = true);

	/// <summary>
	/// Drawに描画回数を設定する
	/// </summary>
	/// <param name="times">描画回数</param>
	void SetDrawTimes(int times);

	/// <summary>
	/// 現在の描画回数を返す
	/// </summary>
	/// <returns></returns>
	int DrawCounter();

	/// <summary>
	/// 指定のオブジェクトが存在するかを調べる
	/// Activeでも、非Activeでも、存在していればtrueとなる
	/// </summary>
	/// <param name="obj">GameObjectのインスタンス</param>
	/// <returns>存在すれtrue</returns>
	bool IsExist(GameObject* obj);

	/// 2024.11.07 S.Matsunaga
	/// <summary>
	/// 指定のオブジェクトが表示中か調べる
	/// </summary>
	/// <param name="obj">GameObjectのインスタンス</param>
	/// <returns>表示中ならtrue</returns>
	bool IsVisible(GameObject* obj);

	void SetEyeDist(GameObject* obj, const float& distQIn, const int idx);

	/// 2024.11.07 S.Matsunaga
	/// <summary>
	/// クラスの表示中オブジェクトをすべて探す
	/// </summary>
	/// <typeparam name="C">クラス名</typeparam>
	/// <returns>オブジェクトの実態list</returns>
	template<class C> std::list<C*> FindGameObjectsVisible()
	{
		std::list<C*> out;
		out.clear();

		const std::list<GameObject*> objs = GetAllObjects();

		for (GameObject* node : objs) {
			if (IsVisible(node))
			{
				C* obj = dynamic_cast<C*>(node);
				if (obj != nullptr)
					out.emplace_back(obj);
			}
		}
		return out;
	}

	/// 2024.11.25 S.Matsunaga
	/// <summary>
	/// 現在存在するオブジェクトをオブジェクトごとの型リストで管理する
	/// 衝突判定で呼び出すために使用する
	/// </summary>
	/// <typeparam name="C">任意のオブジェクトクラス型</typeparam>
	template<class C> std::list<C*> anyObjList;	// 任意のオブジェクトリスト

	/// 2024.11.25 S.Matsunaga
	/// <summary>
	/// 任意のオブジェクトリストに追加する
	/// 追加したらデストラクタでRemoveObjを呼んで配列から消してください
	/// </summary>
	/// <typeparam name="C">任意のオブジェクト型</typeparam>
	/// <param name="obj">追加するオブジェクト</param>
	template<class C> void AddObj(C* obj)
	{
		anyObjList<C>.push_back(obj);
	}

	/// 2024.11.25 S.Matsunaga
	/// <summary>
	/// 指定されたオブジェクトを配列から削除する
	/// </summary>
	/// <typeparam name="C">任意のオブジェクトクラス</typeparam>
	/// <param name="obj">削除するオブジェクト</param>
	template<class C> void RemoveObj(C* obj)
	{
		if (std::find(anyObjList<C>.begin(), anyObjList<C>.end(), obj) != anyObjList<C>.end())
		{
			anyObjList<C>.remove(obj);
		}
	}


	/// 2024.11.26 S.Matsunaga
	/// <summary>
	/// スコアエリア用のリスト
	/// </summary>
	/// <typeparam name="C">エリア型</typeparam>
	template<class C> std::list<C*> scArea;

	/// <summary>
	/// エリアの追加
	/// 親のScoreAreaに追加して管理する
	/// </summary>
	/// <typeparam name="C"></typeparam>
	/// <param name="area"></param>
	template<class C> void AddScArea(C* area)
	{
		scArea<ScoreArea>.push_back(area);
	}

	/// <summary>
	/// リストからエリアを削除
	/// </summary>
	/// <typeparam name="C"></typeparam>
	/// <param name="area"></param>
	template<class C> void RemoveArea(C* area)
	{
		if (std::find(scArea<ScoreArea>.begin(), scArea<ScoreArea>.end(), area) != scArea<ScoreArea>.end())
		{
			scArea<ScoreArea>.remove(area);
		}
	}

};
