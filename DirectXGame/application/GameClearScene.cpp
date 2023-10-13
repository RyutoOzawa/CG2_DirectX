//ゲームクリアシーンクラス
//ゲームをクリアしたときに以降するシーンの処理を行う

#include "GameClearScene.h"
#include"Input.h"
#include"GameSceneManager.h"

void GameClearScene::Initialize()
{
	//テクスチャ
	gameClearTexture = Texture::LoadTexture("gameclear.png");

	//スプライト
	gameClearSprite = new Sprite();
	gameClearSprite->Initialize(gameClearTexture);
}

void GameClearScene::Finalize()
{
	delete gameClearSprite;
}

void GameClearScene::Update()
{
	//スペースキーでタイトルへ
	if (Input::GetInstance()->IsKeyTrigger(DIK_SPACE) || Input::GetInstance()->IsPadTrigger(XINPUT_GAMEPAD_START))
	{
		//シーンの切り替えを依頼
		sceneManager->ChangeScene("TITLE");
	}
}

void GameClearScene::Draw()
{
	Sprite::BeginDraw();

	gameClearSprite->Draw();

}
