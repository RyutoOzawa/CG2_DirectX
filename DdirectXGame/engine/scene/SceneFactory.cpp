#include "SceneFactory.h"
#include"GamePlayScene.h"
#include"GameTitleScene.h"

GameBaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	//受け取ったタグ名のシーンを生成
	GameBaseScene* newScene = nullptr;

	if (sceneName == "TITLE") {
		newScene = new GameTitleScene();
	}
	else if (sceneName == "GAMEPLAY") {
		newScene = new GamePlayScene();
	}

	return newScene;
}
