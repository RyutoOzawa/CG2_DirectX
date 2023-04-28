#include "GamePlayScene.h"
#include"Texture.h"
#include"DirectX.h"
#include"SpriteManager.h"
#include"GameSceneManager.h"
#include"Collision.h"
#include"FbxLoader.h"
#include"FbxObject3d.h"

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

	defaultModel = std::make_unique<Model>();
	defaultModel = Model::CreateModel();
	defaultModel->textureIndex = reimuGraph;

	triangleModel = std::make_unique<Model>();
	triangleModel = Model::CreateModel("triangle_mat");

	//カメラ初期化
	Vector3 eye(0, 20, -20);	//視点座標
	Vector3 target(0, 0, 6);	//注視点座標
	Vector3 up(0, 1, 0);		//上方向ベクトル

	sprite2->SetPos({240, 240});

	camera->Initialize(eye, target, up);

	skydomeObj = std::make_unique<Object3d>();
	skydomeObj->Initialize();
	skydomeObj->SetModel(skydome.get());

	planeObj = std::make_unique<Object3d>();
	planeObj->Initialize();
	planeObj->SetModel(defaultModel.get());
	planeObj->scale = { 10.0f,0.01f,10.0f };
	planeObj->position.y = -2.0f;

	triangleObj = std::make_unique<Object3d>();
	triangleObj->Initialize();
	triangleObj->SetModel(triangleModel.get());

	rayObj = std::make_unique<Object3d>();
	rayObj->Initialize();
	rayObj->SetModel(defaultModel.get());
	rayObj->scale = { 0.01f,10.0f,0.01f };

	newAudio = std::make_unique<AudioManager>();
	newAudio->SoundLoadWave("Resources/bgm_title.wav");

	//球の初期値を設定
	sphere.pos = { -5,2,-0 };
	sphere.radius = 1.0f;
	//平面の初期値を設定
	plane.normal = { 0,1,0 };
	plane.distance = -2.0f;
	//三角形の初期値を設定
	triangle.p0 = { -1.0f,0,-1.0f };
	triangle.p1 = { -1.0f,0,+1.0f };
	triangle.p2 = { +1.0f,0,-1.0f };
	triangle.normal = { 0.0f,1.0f,0.0f };
	//レイの初期値を設定
	ray.start = { 0,1,0 };
	ray.dir = { 0,-1,0 };


	//モデル名を指定してファイル読み込み
	model1 = std::make_unique<FbxModel>();
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("cube");

	object1 = std::make_unique<FbxObject3d>();
	object1->Initialize();
	object1->SetModel(model1.get());

	//デバイスセット
	FbxObject3d::SetCamera(camera);

	camera->target = { 0,20,0 };
	camera->eye = { 0,0,-200 };

}

void GamePlayScene::Finalize()
{
	//ゲームループで使用した物を解放後、基盤システムの後処理と解放を行う

	//delete sprite;
	//delete skyDome;

	delete camera;

	//-------------ここまでにループ内で使用したものの後処理------------//



}

void GamePlayScene::Update()
{


	//----------------------ゲーム内ループはここから---------------------//





	camera->UpdateMatrix();

	//天球の操作
	ImGui::Begin("skydome");
	ImGui::SliderFloat("rotateY", &skydomeObj->rotation.y, 0.0f, 5.0f);
	ImGui::SliderFloat("posX", &sphere.pos.x, -10.0f, 10.0f);
	ImGui::SliderFloat("posY", &sphere.pos.y, -10.0f, 10.0f);
	ImGui::SliderFloat("posZ", &sphere.pos.z, -10.0f, 10.0f);
	ImGui::SliderFloat("scaleX", &skydomeObj->scale.x, 0.0f, 5.0f);
	ImGui::SliderFloat("scaleY", &skydomeObj->scale.y, 0.0f, 5.0f);
	ImGui::SliderFloat("scaleZ", &skydomeObj->scale.z, 0.0f, 5.0f);
	ImGui::End();

	//レイの操作
	ImGui::Begin("Ray");
	ImGui::Text("cube wo scale de ookiku siteiru tame usiro ha hanntei arimasen");
	ImGui::SliderFloat("posX", &ray.start.x, -10.0f, 10.0f);
	ImGui::SliderFloat("posZ", &ray.start.z, -10.0f, 10.0f);
	ImGui::End();

	Vector2 sprite1Pos = { sprite->GetPosition().x,sprite->GetPosition().y };
	Vector2 sprite2Pos = { sprite2->GetPosition().x,sprite2->GetPosition().y };
	float pos[2] = { sprite1Pos.x,sprite1Pos.y };
	ImGui::Begin("Sprite");

	ImGui::SliderFloat2("sprite1", pos, 0.0f, WindowsAPI::winW);
	sprite->SetPos({ pos[0],pos[1] });


	sprite->Update();
	sprite2->Update();

	ImGui::End();

	//ata

	skydomeObj->position = sphere.pos;
	skydomeObj->Update();
	planeObj->Update();

	rayObj->position = ray.start;
	rayObj->Update();

	object1->Update();

	//----------------------ゲーム内ループはここまで---------------------//


}

void GamePlayScene::Draw()
{
	//-------背景スプライト描画処理-------//
	SpriteManager::GetInstance()->beginDraw();

	//backGroundSprite->Draw();

	//-------3Dオブジェクト描画処理-------//
	Object3d::BeginDraw(camera);

	skydomeObj->Draw();
	planeObj->Draw();
	triangleObj->Draw();
	rayObj->Draw();

	object1->Draw();

	//-------前景スプライト描画処理-------//
	SpriteManager::GetInstance()->beginDraw();

	sprite->Draw();
	sprite2->Draw();
}
