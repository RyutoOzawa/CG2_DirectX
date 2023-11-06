#pragma once
//ゲームクリアシーンクラス
//ゲームをクリアしたときに以降するシーンの処理を行う

#include"GameBaseScene.h"
#include"Sprite.h"
#include"Model.h"
#include"Object3d.h"

class GameClearScene : public GameBaseScene
{
public:
	//初期化
	void Initialize() override;

	//終了
	void Finalize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;
private:

	TextureData* gameClearTexture = 0;
	Sprite* gameClearSprite = nullptr;

	std::unique_ptr<Camera> camera = nullptr;

	std::unique_ptr<Model> skydome = nullptr;
	std::unique_ptr<Object3d> skydomeObj = nullptr;

	const uint16_t nextSceneTimerMax = 120;
	uint16_t nextSceneTimer = 0;
};

