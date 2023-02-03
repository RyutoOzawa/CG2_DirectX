#include "GamePlayScene.h"
#include"Texture.h"
#include"DirectX.h"
#include"SpriteManager.h"
#include"GameSceneManager.h"

using namespace DirectX;

void GamePlayScene::Initialize()
{


	//--------------ゲーム内変数初期化--------------//

	//inputのインスタンス取得
	input = Input::GetInstance();

	//テクスチャデータ初期化

	backGroundTexture = Texture::LoadTexture(L"Resources/dummyPlayGame.png");
	marioGraph = Texture::LoadTexture(L"Resources/mario.jpg");
	reimuGraph = Texture::LoadTexture(L"Resources/reimu.png");
	backGroundSprite = std::make_unique<Sprite>();
	sprite = std::make_unique<Sprite>();
	sprite2 = std::make_unique<Sprite>();
	backGroundSprite->Initialize(backGroundTexture);
	sprite->Initialize(marioGraph);
	sprite2->Initialize(reimuGraph);

	skydome = std::make_unique<Model>();


	skydome = Model::CreateModel("skydome");

	//カメラ初期化
	Vector3 eye(0, 0, -20);	//視点座標
	Vector3 target(0, 0, 6);	//注視点座標
	Vector3 up(0, 1, 0);		//上方向ベクトル

	camera.Initialize(eye, target, up);

	skydomeObj = std::make_unique<Object3d>();
	skydomeObj->Initialize();
	skydomeObj->SetModel(skydome.get());

	newAudio = std::make_unique<AudioManager>();
	newAudio->SoundLoadWave("Resources/bgm_title.wav");

}

void GamePlayScene::Finalize()
{
	//ゲームループで使用した物を解放後、基盤システムの後処理と解放を行う

	//delete sprite;
	//delete skyDome;

	//-------------ここまでにループ内で使用したものの後処理------------//



}

void GamePlayScene::Update()
{


	//----------------------ゲーム内ループはここから---------------------//



	

	camera.UpdateMatrix();

	//ADで天球の回転
	ImGui::Begin("debug");

	ImGui::SliderFloat("Skydome:rotateY", &skydomeObj->rotation.y, 0.0f, 5.0f);
	ImGui::SliderFloat("Skydome:posX", &skydomeObj->position.x, 0.0f, 5.0f);
	ImGui::SliderFloat("Skydome:posY", &skydomeObj->position.y, 0.0f, 5.0f);
	ImGui::SliderFloat("Skydome:scaleX", &skydomeObj->scale.x, 0.0f, 5.0f);
	ImGui::SliderFloat("Skydome:scaleY", &skydomeObj->scale.y, 0.0f, 5.0f);
	ImGui::SliderFloat("Skydome:scaleZ", &skydomeObj->scale.z, 0.0f, 5.0f);

	if (ImGui::Button("music")) {
		newAudio->SoundPlayWave();
	}

	ImGui::End();

	skydomeObj->Update();

	//----------------------ゲーム内ループはここまで---------------------//


}

void GamePlayScene::Draw()
{
	//-------背景スプライト描画処理-------//
	SpriteManager::GetInstance()->beginDraw();

	backGroundSprite->Draw();

	//-------3Dオブジェクト描画処理-------//
	Object3d::BeginDraw(camera);

	skydomeObj->Draw();

	//-------前景スプライト描画処理-------//
	SpriteManager::GetInstance()->beginDraw();

	sprite->Draw();
	sprite2->Draw();
}
