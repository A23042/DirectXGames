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
		if (str == "1") {
			str = csv->GetString(i, 1);
			if (str == "#") {
				continue;
			}
			else if (str == "PLAYER") {
				float e = csv->GetFloat(i, 5);
				float f = csv->GetFloat(i, 6);
				float mass = csv->GetFloat(i, 7);
				obj = Instantiate<Player>();
				obj->sphere.e = e;
				obj->sphere.f = f;
				obj->sphere.mass = mass;
			}
			else if (str == "BOX") {
				VECTOR3 size = VECTOR3(csv->GetFloat(i, 5), csv->GetFloat(i, 6), csv->GetFloat(i, 7));
				VECTOR3 rot = VECTOR3(csv->GetFloat(i, 8), csv->GetFloat(i, 9), csv->GetFloat(i, 10));
				float e = csv->GetFloat(i, 11);
				float f = csv->GetFloat(i, 12);
				obj = new Box(size, rot);	// 直方体の各辺の長さと回転量を渡す
				obj->pObj.e = e;
				obj->pObj.f = f;
			}
			else if (str == "MBox") {
				VECTOR3 size = VECTOR3(csv->GetFloat(i, 5), csv->GetFloat(i, 6), csv->GetFloat(i, 7));
				VECTOR3 rot = VECTOR3(csv->GetFloat(i, 8), csv->GetFloat(i, 9), csv->GetFloat(i, 10));
				VECTOR3 move = VECTOR3(csv->GetFloat(i, 11), csv->GetFloat(i, 12), csv->GetFloat(i, 13));
				VECTOR3 moveSpeed = VECTOR3(csv->GetFloat(i, 14), csv->GetFloat(i, 15), csv->GetFloat(i, 16));
				float e = csv->GetFloat(i, 17);
				float f = csv->GetFloat(i, 18);
				obj = new MoveBox(size, rot, move, moveSpeed);	// 直方体の各辺の長さと回転量、移動量を渡す
				obj->pObj.e = e;
				obj->pObj.f = f;
			}
			else if (str == "BALL") {
				float e = csv->GetFloat(i, 5);
				float f = csv->GetFloat(i, 6);
				float mass = csv->GetFloat(i, 7);
				obj = Instantiate<Ball>();
				obj->sphere.e = e;
				obj->sphere.f = f;
				obj->sphere.mass = mass;
			}
			else {
				assert(false);
			}
			float x = csv->GetFloat(i, 2);
			float y = csv->GetFloat(i, 3);
			float z = csv->GetFloat(i, 4);
			obj->SetPosition(x, y, z);
			obj->Position();
		}else if (str == "0") {
			continue;
		}
		
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
