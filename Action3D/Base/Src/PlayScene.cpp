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
#include "MoveBox.h"

PlayScene::PlayScene()
{
	// テキストファイルの読み方
	CsvReader* csv = new CsvReader("Data/map00.csv");
	assert(csv->GetLines() > 0);
	for (int i = 1; i < csv->GetLines(); i++) { // １行ずつ読む
		std::string str = csv->GetString(i, 0); // PLAYERが入る
		Object3D* obj = nullptr;
		if (str == "#") {
			continue;
		}else if (str == "PLAYER") {
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
			//float x = csv->GetFloat(i, 4);
			//float y = csv->GetFloat(i, 5);
			//float z = csv->GetFloat(i, 6);
			//float rotX = csv->GetFloat(i, 7);
			//float rotY = csv->GetFloat(i, 8);
			//float rotZ = csv->GetFloat(i, 9);
			VECTOR3 size = VECTOR3(csv->GetFloat(i, 4), csv->GetFloat(i, 5), csv->GetFloat(i, 6));
			VECTOR3 rot = VECTOR3(csv->GetFloat(i, 7), csv->GetFloat(i, 8), csv->GetFloat(i, 9));
			obj = new Box(size, rot);	// 直方体の各辺の長さと回転量を渡す
		}
		else if (str == "MBox") {
			/*float x = csv->GetFloat(i, 4);
			float y = csv->GetFloat(i, 5);
			float z = csv->GetFloat(i, 6);
			float rotX = csv->GetFloat(i, 7);
			float rotY = csv->GetFloat(i, 8);
			float rotZ = csv->GetFloat(i, 9);
			float moveX = csv->GetFloat(i, 10);
			float moveX = csv->GetFloat(i, 11);
			float moveX = csv->GetFloat(i, 12);*/
			VECTOR3 size = VECTOR3(csv->GetFloat(i, 4), csv->GetFloat(i, 6), csv->GetFloat(i, 6));
			VECTOR3 rot = VECTOR3(csv->GetFloat(i, 7), csv->GetFloat(i, 8), csv->GetFloat(i, 9));
			VECTOR3 move = VECTOR3(csv->GetFloat(i, 10), csv->GetFloat(i, 11), csv->GetFloat(i, 12));
			VECTOR3 moveSpeed = VECTOR3(csv->GetFloat(i, 13), csv->GetFloat(i, 14), csv->GetFloat(i, 15));
			obj = new MoveBox(size, rot, move, moveSpeed);	// 直方体の各辺の長さと回転量、移動量を渡す
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
		obj->Position();
	}
	Instantiate<Camera>();
	Score* sc = ObjectManager::FindGameObject<Score>();
	if (sc == nullptr) {
		sc = Instantiate<Score>();
		sc->DontDestroyMe(); // シーンが切り替わっても消えない
	}
	sc->Clear();
	Instantiate<ScoreDraw>();
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	// Dancerが全滅したらTitleScene
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
