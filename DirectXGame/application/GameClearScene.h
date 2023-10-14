#pragma once
//ゲームクリアシーンクラス
//ゲームをクリアしたときに以降するシーンの処理を行う

#include"GameBaseScene.h"
#include"Sprite.h"
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
};

