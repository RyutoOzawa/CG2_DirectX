//ゲームオーバーシーンクラス
//ゲームオーバーになった際に以降する画面の処理を行う

#include "GameOverScene.h"
#include"Input.h"
#include"GameSceneManager.h"

void GameOverScene::Initialize()
{
	//テクスチャデータ
	gameoverTexture = Texture::LoadTexture("gameover.png");

	//スプライト
	gameoverSprite = new Sprite();
	gameoverSprite->Initialize(gameoverTexture);

	//次シーンへのタイマーをセット
	nextSceneTimer = nextSceneTimerMax;
}

void GameOverScene::Finalize()
{
	delete gameoverSprite;
}

void GameOverScene::Update()
{
	if (nextSceneTimer > 0) {
		nextSceneTimer--;
	}
	else {
		//シーンの切り替えを依頼
		sceneManager->ChangeScene("TITLE");
	}

	//スペースキーでメインゲームへ
	if (Input::GetInstance()->IsKeyTrigger(DIK_SPACE) || Input::GetInstance()->IsPadTrigger(XINPUT_GAMEPAD_START))
	{
		//シーンの切り替えを依頼
		sceneManager->ChangeScene("TITLE");
	}
}

void GameOverScene::Draw()
{
	Sprite::BeginDraw();

	gameoverSprite->Draw();

}
