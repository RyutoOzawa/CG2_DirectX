#pragma once
//ゲームオーバーシーンクラス
//ゲームオーバーになった際に以降する画面の処理を行う

#include"GameBaseScene.h"
#include"Sprite.h"

class GameOverScene : public GameBaseScene
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

	TextureData* gameoverTexture = 0;
	Sprite* gameoverSprite = nullptr;

const uint16_t nextSceneTimerMax = 120;
	uint16_t nextSceneTimer = 0;
};

