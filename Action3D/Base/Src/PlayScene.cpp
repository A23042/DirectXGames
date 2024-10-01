#include "PlayScene.h"
#include "Player.h"
#include "Ground.h"
#include "Door.h"
#include "Dancer.h"
#include <fstream>
#include "CsvReader.h"
#include <assert.h>
#include "Camera.h"
#include "Score.h"
#include "ScoreDraw.h"
#include "Box.h"
#include "Ball.h"

PlayScene::PlayScene()
{
	// �e�L�X�g�t�@�C���̓ǂݕ�
	CsvReader* csv = new CsvReader("Data/map00.csv");
	assert(csv->GetLines() > 0);
	for (int i = 1; i < csv->GetLines(); i++) { // �P�s���ǂ�
		std::string str = csv->GetString(i, 0); // PLAYER������
		Object3D* obj = nullptr;
		if (str == "PLAYER") {
			obj = Instantiate<Player>();
		}
		else if (str == "GROUND") {
			obj = Instantiate<Ground>();
		}
		else if (str == "DOOR") {
			obj = Instantiate<Door>();
		}
		//else if (str == "DANCER") {
		//	obj = Instantiate<Dancer>();
		//}
		else if (str == "BOX") {
			float x = csv->GetFloat(i, 4);
			float y = csv->GetFloat(i, 5);
			float z = csv->GetFloat(i, 6);
			float rotX = csv->GetFloat(i, 7);
			float rotY = csv->GetFloat(i, 8);
			float rotZ = csv->GetFloat(i, 9);

			obj = new Box(x, y, z, rotX, rotY, rotZ);	// �����̂̊e�ӂ̒�����n��
		}
		else if (str == "BALL") {
			obj = Instantiate<Ball>();
		}
		else {
			assert(false);
		}
		float x = csv->GetFloat(i, 1);
		float y = csv->GetFloat(i, 2);
		float z = csv->GetFloat(i, 3);
		obj->SetPosition(x, y, z);
	}
	Instantiate<Camera>();
	Score* sc = ObjectManager::FindGameObject<Score>();
	if (sc == nullptr) {
		sc = Instantiate<Score>();
		sc->DontDestroyMe(); // �V�[�����؂�ւ���Ă������Ȃ�
	}
	sc->Clear();
	Instantiate<ScoreDraw>();
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	// Dancer���S�ł�����TitleScene
	//std::list<Dancer*> dancers = ObjectManager::FindGameObjects<Dancer>();
	//if (dancers.size() == 0) {
		//SceneManager::ChangeScene("TitleScene");
	//}
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_R)) {
		SceneManager::ChangeScene("ResultScene");
	}
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_T)) {
		SceneManager::ChangeScene("TitleScene");
	}
	
}

void PlayScene::Draw()
{
	
}
