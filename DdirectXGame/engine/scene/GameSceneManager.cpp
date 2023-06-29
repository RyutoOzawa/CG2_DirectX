#include "GameSceneManager.h"
#include<cassert>

GameSceneManager::GameSceneManager()
{

}

GameSceneManager::~GameSceneManager()
{
	
}

GameSceneManager* GameSceneManager::GetInstance()
{
	static GameSceneManager instance;
	return &instance;
}

void GameSceneManager::Update()
{
	
	//次のシーンが予約されているなら
 	if (nextScene) {
		//旧シーン終了
		if (activeScene) {
			activeScene->Finalize();
			delete activeScene;
		}

		//シーン切り替え
		activeScene = nextScene;
		nextScene = nullptr;

		//シーン側からシーン切り替えを依頼できるように、シーンマネージャをセットする
		activeScene->SetSceneManager(this);

		//次シーン初期化
		activeScene->Initialize();

	}
	//実行シーンの更新
	activeScene->Update();
}

void GameSceneManager::Draw()
{
	activeScene->Draw();
}

void GameSceneManager::Finalize()
{
	//現在のシーンの終了と開放
	activeScene->Finalize();
	delete activeScene;
}

void GameSceneManager::ChangeScene(const std::string& sceneName)
{
	//nullチェック
	assert(sceneFactory);
	assert(nextScene == nullptr);
	//次シーンの生成
	nextScene = sceneFactory->CreateScene(sceneName);
}
