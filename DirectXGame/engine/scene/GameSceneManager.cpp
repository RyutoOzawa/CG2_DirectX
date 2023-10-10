#include "GameSceneManager.h"
#include<cassert>
#include"FadeSceneTransition.h"

GameSceneManager::GameSceneManager()
{

}

GameSceneManager::~GameSceneManager()
{
	
}

GameSceneManager* GameSceneManager::GetInstance()
{
	static GameSceneManager* instance = nullptr;
	if (instance == nullptr) {
		instance = new GameSceneManager;
	}
	return instance;
}

void GameSceneManager::Update()
{





	//シーン遷移が生成されているなら遷移させる
	if (sceneTransition) {
		sceneTransition->Update();

		//シーン遷移フェーズがchangeならシーンの変更
		if (sceneTransition->GetPhase() == TransitionPhase::Change) {
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
			activeScene->Update();
			//シーンを開ける
			sceneTransition->Open();

		}

		//シーン遷移管理フラグが降りていればシーン遷移を消す
		if (!sceneTransition->IsTransition()) {
			delete sceneTransition;
			sceneTransition = nullptr;
		}
	}
	else {

		//実行シーンの更新
		activeScene->Update();
	}
}

void GameSceneManager::Draw()
{
	if (activeScene) {
		activeScene->Draw();
	}

	if (sceneTransition) {

		sceneTransition->Draw();
	}
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

	//TODO:changesceneの引数に対応した遷移を行う(デフォルトはフェード)
	sceneTransition = new FadeSceneTransition();
	sceneTransition->Initialize();
	//シーン遷移の開始
	sceneTransition->Close();
}
