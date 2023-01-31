#pragma once

class GameSceneManager;

class GameBaseScene
{
public:
	//初期化
	virtual void Initialize() = 0;

	//終了
	virtual void Finalize() = 0;

	//更新
	virtual void Update() = 0;

	//描画
	virtual void Draw() = 0;

	virtual ~GameBaseScene() = default;

	virtual void SetSceneManager(GameSceneManager* sceneManager) { this->sceneManager = sceneManager; }

protected:
	//借りてくるシーンマネージャ
	GameSceneManager* sceneManager = nullptr;
};

