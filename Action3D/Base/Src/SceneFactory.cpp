#include "SceneFactory.h"
#include <windows.h>
#include <assert.h>
#include "TitleScene.h"
#include "PlayScene.h"
#include "ResultScene.h"
#include "MapEditScene.h"
#include "SplitScreen.h"

SceneBase* SceneFactory::CreateFirst()
{
	SplitScreen* ss = SingleInstantiate<SplitScreen>();
	ss->SetSingleScreen();
	return new TitleScene();
}

SceneBase * SceneFactory::Create(const std::string & name, int num)
{
	if (name == "TitleScene") {
		ObjectManager::FindGameObject<SplitScreen>()->SetSingleScreen();
		return new TitleScene();
	}
	if (name == "PlayScene") {
		ObjectManager::FindGameObject<SplitScreen>()->SetMultiScreen();
		//ObjectManager::FindGameObject<SplitScreen>()->SetSingleScreen();
		return new PlayScene(num);
	}
	if (name == "ResultScene") {
		ObjectManager::FindGameObject<SplitScreen>()->SetSingleScreen();
		return new ResultScene();
	}
	if (name == "MapEditScene") {
		ObjectManager::FindGameObject<SplitScreen>()->SetSingleScreen();
		return new MapEditScene();
	}
	
	assert(false);
	return nullptr;
}
