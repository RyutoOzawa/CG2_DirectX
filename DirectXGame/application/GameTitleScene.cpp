#include "GameTitleScene.h"
#include"Texture.h"
#include"DirectX.h"
using namespace DirectX;
#include"GameSceneManager.h"

void GameTitleScene::Initialize()
{


	//--------------ゲーム内変数初期化--------------//

	//inputのインスタンス取得
	input = Input::GetInstance();

	//テクスチャデータ初期化
	titleTexture = Texture::LoadTexture("dummyTitle.png");

	titleSprite = new Sprite();
	titleSprite->Initialize(titleTexture);

}

void GameTitleScene::Finalize()
{
	//ゲームループで使用した物を解放後、基盤システムの後処理と解放を行う

	//delete sprite;
	//delete skyDome;

	delete titleSprite;
	//-------------ここまでにループ内で使用したものの後処理------------//



}

void GameTitleScene::Update()
{


	//----------------------ゲーム内ループはここから---------------------//

	//スペースキーでメインゲームへ
	if (input->IsKeyTrigger(DIK_SPACE) || input->IsPadTrigger(XINPUT_GAMEPAD_START))
	{
		//シーンの切り替えを依頼
		sceneManager->ChangeScene("GAMEPLAY");
	}

	if (input->IsKeyTrigger(DIK_F1))
	{
		//シーンの切り替えを依頼
		sceneManager->ChangeScene("TESTSCENE");
	}

	//----------------------ゲーム内ループはここまで---------------------//


}

void GameTitleScene::Draw()
{
	//-------スプライト描画処理-------//
	Sprite::BeginDraw();

	titleSprite->Draw();

}
