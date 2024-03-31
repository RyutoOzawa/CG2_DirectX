//ゲームクリアシーンクラス
//ゲームをクリアしたときに以降するシーンの処理を行う

#include "GameClearScene.h"
#include"Input.h"
#include"GameSceneManager.h"
#include"Util.h"

void GameClearScene::Initialize()
{
	//テクスチャ
	gameClearTexture = Texture::LoadTexture("gameClearText.png");


	//スプライト
	gameClearSprite = new Sprite();
	gameClearSprite->Initialize(gameClearTexture);
	gameClearSprite->SetAnchorPoint({ 0.5f,0.5f });
	gameClearSprite->SetPos({ WindowsAPI::winW / 2.0f,WindowsAPI::winH / 2.0f });


	//天球
	skydome = std::make_unique<Model>();
	skydome = Model::CreateModel("skydome");

	skydomeObj = std::make_unique<Object3d>();
	skydomeObj->Initialize();
	skydomeObj->SetModel(skydome.get());
	skydomeObj->scale = { 1000,1000,1000 };

	//カメラ初期化
	camera = std::make_unique<Camera>();
	Vector3 eye, target, up;
	eye = { 0,0,0 };
	target = { 0,0,1 };
	up = { 0,1,0 };

	camera->Initialize(eye, target, up);

	//次シーンへのタイマーをセット
	nextSceneTimer = nextSceneTimerMax;
}

void GameClearScene::Finalize()
{
	delete gameClearSprite;
}

void GameClearScene::Update()
{
	//天球を地味に回転
	skydomeObj->rotation.y += PI / 14400.0f;
	skydomeObj->Update();

	//スペースキーでタイトルへ
	if (Input::GetInstance()->IsKeyTrigger(DIK_SPACE) || Input::GetInstance()->IsPadTrigger(XINPUT_GAMEPAD_START))
	{
		//シーンの切り替えを依頼
		sceneManager->ChangeScene("TITLE");
	}

	//自動でシーン切り替えを行う
	if (nextSceneTimer > 0) {
		nextSceneTimer--;
	}
	else {
		//シーンの切り替えを依頼
		sceneManager->ChangeScene("TITLE");
	}
}

void GameClearScene::Draw()
{


	Object3d::BeginDraw(camera.get());

	skydomeObj->Draw();


	Sprite::BeginDraw();

	gameClearSprite->Draw();
}
