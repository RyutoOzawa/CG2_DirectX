#include "SceneFactory.h"
#include"GamePlayScene.h"
#include"GameTitleScene.h"
#include"GameOverScene.h"
#include"GameClearScene.h"
#include"TestScene.h"

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
	else if (sceneName == "GAMEOVER") {
		newScene = new GameOverScene();
	}
	else if (sceneName == "TESTSCENE") {
		newScene = new TestScene();
	}
	else if (sceneName == "GAMECLEAR") {
		newScene = new GameClearScene();
	}

	return newScene;
}
