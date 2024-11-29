#pragma once
/// <summary>
/// ���ׂẴI�u�W�F�N�g���Ǘ����܂�
/// �V���O���g���ɂ��Ă���
/// �������炷�ׂĂ�GameObject��Update()/Draw()���Ăяo���܂�
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
	/// Object��ǉ�����
	/// ���̊֐��́AGameObject�̃R���X�g���N�^����Ă΂��
	/// </summary>
	/// <param name="obj"></param>
	void Push(GameObject* obj);

	/// <summary>
	/// Object���폜����
	/// ���̊֐��ł́A�폜�̗v�������邾���ŁA
	/// ���ۂɍ폜�����̂́AUpdate()���Ă΂�钼�O
	/// </summary>
	/// <param name="obj"></param>
	void Destroy(GameObject* obj);

	std::list<GameObject*> GetAllObjects();

	/// <summary>
	/// �N���X���ŃI�u�W�F�N�g��T��
	/// </summary>
	/// <typeparam name="C">�N���X</typeparam>
	/// <returns>�I�u�W�F�N�g�̎��ԁi���݂��Ȃ����nullptr�j</returns>
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
	/// �N���X���Ŏ����ȊO�̃I�u�W�F�N�g��T��
	/// </summary>
	/// <typeparam name="C">�N���X</typeparam>
	/// <param name="me">����</param>
	/// <returns>�I�u�W�F�N�g�̎��ԁi���݂��Ȃ����nullptr�j</returns>
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
	/// �N���X�̃I�u�W�F�N�g�����ׂĒT��
	/// </summary>
	/// <typeparam name="C">�N���X��</typeparam>
	/// <returns>�I�u�W�F�N�g�̎���list</returns>
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
	/// �N���X���ƃ^�O����I�u�W�F�N�g��T��
	/// </summary>
	/// <typeparam name="C">�N���X��</typeparam>
	/// <param name="tag">�^�O</param>
	/// <returns>�I�u�W�F�N�g�̎��ԁi���݂��Ȃ����nullptr�j</returns>
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
	/// �N���X���ƃ^�O����I�u�W�F�N�g�����ׂĒT��
	/// </summary>
	/// <typeparam name="C">�N���X��</typeparam>
	/// <param name="tag">�^�O</param>
	/// <returns>�I�u�W�F�N�g�̎���list</returns>
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
	/// �N���X���ƒT���Ȃ��^�O�ȊO�I�u�W�F�N�g�����ׂĒT��
	/// </summary>
	/// <typeparam name="C">�N���X��</typeparam>
	/// <param name="tag">���O����^�O</param>
	/// <returns>�I�u�W�F�N�g�̎���list</returns>
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
	/// �`��̃v���C�I���e�B��ݒ肷��
	/// ���l�����Ȃ����ɕ`�悳���̂ŁA�Q�c�ł͉��ɕ\�������
	/// �Q�c�Ŏ�O�ɕ\�����������A�R�c�Ō�ɕ`�悵�������́A�l����������
	/// �v���C�I���e�B���������̂̏��Ԃ͕ۏ؂���Ȃ�
	/// �v���C�I���e�B�̃f�t�H���g�͂O�ł�
	/// </summary>
	/// <param name="obj">�v���C�I���e�B��ݒ肷��I�u�W�F�N�g</param>
	/// <param name="order">�`��v���C�I���e�B</param>
	void SetDrawOrder(GameObject* _obj, int _order);

	/// <summary>
	/// Update�̗D�揇�ʂ�t����
	/// </summary>
	/// <param name="_obj"></param>
	/// <param name="_priority"></param>
	void SetPriority(GameObject* _obj, int _priority);

	/// <summary>
	/// GameObject���폜����
	/// </summary>
	/// <param name="obj">GameObject�̃C���X�^���X</param>
	void DeleteGameObject(GameObject* obj);

	/// <summary>
	/// �S�Ă�GameObject���폜����
	/// </summary>
	void DeleteAllGameObject();

	void DontDestroy(GameObject* obj, bool dont = true);

	/// <summary>
	/// Update�����s���邩�ݒ肷��
	/// </summary>
	/// <param name="obj">GameObject�̃C���X�^���X</param>
	/// <param name="active">���s����ꍇ��true</param>
	void SetActive(GameObject* obj, bool active = true);

	/// <summary>
	/// Draw�����s���邩�ݒ肷��
	/// </summary>
	/// <param name="obj">GameObject�̃C���X�^���X</param>
	/// <param name="visible">Draw����ꍇ��true</param>
	void SetVisible(GameObject* obj, bool visible = true);

	/// <summary>
	/// Draw�ɕ`��񐔂�ݒ肷��
	/// </summary>
	/// <param name="times">�`���</param>
	void SetDrawTimes(int times);

	/// <summary>
	/// ���݂̕`��񐔂�Ԃ�
	/// </summary>
	/// <returns></returns>
	int DrawCounter();

	/// <summary>
	/// �w��̃I�u�W�F�N�g�����݂��邩�𒲂ׂ�
	/// Active�ł��A��Active�ł��A���݂��Ă����true�ƂȂ�
	/// </summary>
	/// <param name="obj">GameObject�̃C���X�^���X</param>
	/// <returns>���݂���true</returns>
	bool IsExist(GameObject* obj);

	/// 2024.11.07 S.Matsunaga
	/// <summary>
	/// �w��̃I�u�W�F�N�g���\���������ׂ�
	/// </summary>
	/// <param name="obj">GameObject�̃C���X�^���X</param>
	/// <returns>�\�����Ȃ�true</returns>
	bool IsVisible(GameObject* obj);

	void SetEyeDist(GameObject* obj, const float& distQIn, const int idx);

	/// 2024.11.07 S.Matsunaga
	/// <summary>
	/// �N���X�̕\�����I�u�W�F�N�g�����ׂĒT��
	/// </summary>
	/// <typeparam name="C">�N���X��</typeparam>
	/// <returns>�I�u�W�F�N�g�̎���list</returns>
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
	/// ���ݑ��݂���I�u�W�F�N�g���I�u�W�F�N�g���Ƃ̌^���X�g�ŊǗ�����
	/// �Փ˔���ŌĂяo�����߂Ɏg�p����
	/// </summary>
	/// <typeparam name="C">�C�ӂ̃I�u�W�F�N�g�N���X�^</typeparam>
	template<class C> std::list<C*> anyObjList;	// �C�ӂ̃I�u�W�F�N�g���X�g

	/// 2024.11.25 S.Matsunaga
	/// <summary>
	/// �C�ӂ̃I�u�W�F�N�g���X�g�ɒǉ�����
	/// �ǉ�������f�X�g���N�^��RemoveObj���Ă�Ŕz�񂩂�����Ă�������
	/// </summary>
	/// <typeparam name="C">�C�ӂ̃I�u�W�F�N�g�^</typeparam>
	/// <param name="obj">�ǉ�����I�u�W�F�N�g</param>
	template<class C> void AddObj(C* obj)
	{
		anyObjList<C>.push_back(obj);
	}

	/// 2024.11.25 S.Matsunaga
	/// <summary>
	/// �w�肳�ꂽ�I�u�W�F�N�g��z�񂩂�폜����
	/// </summary>
	/// <typeparam name="C">�C�ӂ̃I�u�W�F�N�g�N���X</typeparam>
	/// <param name="obj">�폜����I�u�W�F�N�g</param>
	template<class C> void RemoveObj(C* obj)
	{
		if (std::find(anyObjList<C>.begin(), anyObjList<C>.end(), obj) != anyObjList<C>.end())
		{
			anyObjList<C>.remove(obj);
		}
	}


	/// 2024.11.26 S.Matsunaga
	/// <summary>
	/// �X�R�A�G���A�p�̃��X�g
	/// </summary>
	/// <typeparam name="C">�G���A�^</typeparam>
	template<class C> std::list<C*> scArea;

	/// <summary>
	/// �G���A�̒ǉ�
	/// �e��ScoreArea�ɒǉ����ĊǗ�����
	/// </summary>
	/// <typeparam name="C"></typeparam>
	/// <param name="area"></param>
	template<class C> void AddScArea(C* area)
	{
		scArea<ScoreArea>.push_back(area);
	}

	/// <summary>
	/// ���X�g����G���A���폜
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
