#include "GamePlayScene.h"
#include"Texture.h"
#include"DirectX.h"
#include"GameSceneManager.h"
#include"Collision.h"
#include"FbxLoader.h"
#include"FbxObject3d.h"
#include"Util.h"
#include"SphereCollider.h"

using namespace DirectX;

void GamePlayScene::Initialize()
{


	//--------------ゲーム内変数初期化--------------//

	//inputのインスタンス取得
	input = Input::GetInstance();

	//衝突マネージャにインスタンス取得
	collisionManager = CollisionManager::GetInstance();



	//テクスチャデータ初期化

	backGroundTexture = Texture::LoadTexture("dummyPlayGame.png");
	marioGraph = Texture::LoadTexture("mario.jpg");
	reimuGraph = Texture::LoadTexture("reimu.png");
	particleGraph = Texture::LoadTexture("particle.png");
	reticleGraph = Texture::LoadTexture("reticle.png");
	backGroundSprite = std::make_unique<Sprite>();
	backGroundSprite->Initialize(backGroundTexture);

	Enemy::EnemyInitialize(particleGraph);


	skydome = std::make_unique<Model>();
	skydome = Model::CreateModel("skydome");

	defaultModel = std::make_unique<Model>();
	defaultModel = Model::CreateModel();
	defaultModel->textureIndex = reimuGraph;

	playerBulletModel = std::make_unique<Model>();
	playerBulletModel = Model::CreateModel("PlayerBullet");

	//カメラ初期化
	Vector3 eye(0, 20, -20);	//視点座標
	Vector3 target(0, 0, 6);	//注視点座標
	Vector3 up(0, 1, 0);		//上方向ベクトル

	currentCamera = new Camera();
	currentCamera->Initialize(eye, target, up);

	skydomeObj = std::make_unique<Object3d>();
	skydomeObj->Initialize();
	skydomeObj->SetModel(skydome.get());
	skydomeObj->scale = { 1000,1000,1000 };;

	particleMan = std::make_unique<ParticleManager>();
	particleMan->Initialize(particleGraph);

	player = std::make_unique<Player>();
	player->Initialize(defaultModel.get(), reticleGraph);
	player->SetBulletModel(playerBulletModel.get());
	
	//当たり判定テスト用オブジェクト
	colTestObj = std::make_unique<Object3d>();
	colTestObj->Initialize();
	colTestObj->SetModel(defaultModel.get());
	//colTestObj->SetCollider(new SphereCollider({ 0,0,0 }, 1.0f));
	colTestObj->Update();

	
	for (int i = 0; i < 100; i++) {
		//X,Y,Zすべて[-5.0f,+5.0f]でランダムに分布
		const float randPos = 5.0f;
		Vector3 pos{};
		pos.x = Random(-randPos, randPos);
		pos.y = Random(-randPos, randPos);
		pos.z = Random(-randPos, randPos);
		//x,y,zすべて±0.05で分布
		const float randVel = 0.05f;
		Vector3 vel{};
		vel.x = Random(-randVel, randVel);
		vel.y = Random(-randVel, randVel);
		vel.z = Random(-randVel, randVel);
		//重力に見立ててYのみ-0.001f~0でランダムに分布
		Vector3 acc{};
		const float randAcc = 0.001f;
		acc.y = Random(-randAcc, 0.0f);

		//追加
		particleMan->Add(60, pos, vel, acc,1.0f,0.0f);
	}

	newAudio = std::make_unique<AudioManager>();
	newAudio->SoundLoadWave("Resources/bgm_title.wav");

	//モデル名を指定してファイル読み込み
	//model1 = std::make_unique<FbxModel>();
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	object1 = std::make_unique<FbxObject3d>();
	object1->Initialize();
	object1->SetModel(model1.get());

	//デバイスセット
	FbxObject3d::SetCamera(currentCamera);


	currentCamera->target = { 0,0,0 };
	//	camera->eye = { 0,0,-20 };
	currentCamera->eye = { 0,0,-50 };


	railCamera = new RailCamera();
	railCamera->Initialize({0,0,0}, {0,0,0});

	//obj3dクラスにカメラをセット
	Object3d::SetCamera(railCamera->GetCamera());

}

void GamePlayScene::Finalize()
{
	//ゲームループで使用した物を解放後、基盤システムの後処理と解放を行う

	//delete sprite;
	//delete skyDome;
	delete currentCamera;

	//delete railCamera;

	//-------------ここまでにループ内で使用したものの後処理------------//



}

void GamePlayScene::Update()
{


	//----------------------ゲーム内ループはここから---------------------//


	//レールカメラの更新
	railCamera->Update();

	//レールカメラを親にする
	player->parent = railCamera->GetObject3d();
	player->Update(&enemys);

	for (std::unique_ptr<Enemy>& enemy : enemys) {
		enemy->Update();
	}

	if (ImGui::Button("Enemy spwan")) {
		EnemySpawn();
	}

	if (Input::GetInstance()->IsKeyTrigger(DIK_0)) {
		EnemySpawn();
	}

	Enemy::EnemyParticleUpdate();

	ImGui::SliderFloat("cameraX", &currentCamera->eye.x, -100.0f, 100.0f);
	ImGui::SliderFloat("cameraY", &currentCamera->eye.y, -100.0f, 100.0f);
	ImGui::SliderFloat("cameraZ", &currentCamera->eye.z, -100.0f, 100.0f);

	//camera->target = camera->eye;
	//camera->target.z += 1.0f;

	//現在のカメラ情報をレールカメラのものに
	currentCamera = railCamera->GetCamera();
	currentCamera->UpdateMatrix();

	//天球の操作
	ImGui::Begin("skydome");
	ImGui::SliderFloat("rotateY", &skydomeObj->rotation.y, 0.0f, 5.0f);
	ImGui::SliderFloat("scaleX", &skydomeObj->scale.x, 0.0f, 5.0f);
	ImGui::SliderFloat("scaleY", &skydomeObj->scale.y, 0.0f, 5.0f);
	ImGui::SliderFloat("scaleZ", &skydomeObj->scale.z, 0.0f, 5.0f);
	ImGui::End();

	skydomeObj->Update();

	//X,Y,Zすべて[-5.0f,+5.0f]でランダムに分布
	{
		const float randPos = 5.0f;
		Vector3 pos{};
		pos.x = Random(-randPos, randPos);
		pos.y = Random(-randPos, randPos);
		pos.z = Random(-randPos, randPos);
		//x,y,zすべて±0.05で分布
		const float randVel = 0.05f;
		Vector3 vel{};
		vel.x = Random(-randVel, randVel);
		vel.y = Random(-randVel, randVel);
		vel.z = Random(-randVel, randVel);
		//重力に見立ててYのみ-0.001f~0でランダムに分布
		Vector3 acc{};
		const float randAcc = 0.001f;
		acc.y = Random(-randAcc, 0.0f);

		//追加
		particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);


	}
	particleMan->Update();

	colTestObj->Update();

	//アニメーション開始ボタン
	if (ImGui::Button("animation start")) {
		object1->PlayAnitimation();
	}

	if (input->IsKeyTrigger(DIK_A)) {
		object1->PlayAnitimation();
	}

	ImGui::Text("current:%d", object1->BaGetCurrentTime());

	//object1->SetRot({ 0,3.14f / 2,0 });
	object1->Update();

	//当たり判定チェック
	collisionManager->CheckAllCollisions();

	//スペースキーでメインゲームへ
	if (input->IsKeyTrigger(DIK_F1))
	{
		//シーンの切り替えを依頼
		sceneManager->ChangeScene("TITLE");
	}



	//----------------------ゲーム内ループはここまで---------------------//


}

void GamePlayScene::Draw()
{
	//-------背景スプライト描画処理-------//
	Sprite::BeginDraw();

	//backGroundSprite->Draw();

	//-------3Dオブジェクト描画処理-------//
	Object3d::BeginDraw(currentCamera);


	//天球
	skydomeObj->Draw();
	
	//プレイヤー
	player->Draw();

	colTestObj->Draw();

	//敵
	for (std::unique_ptr<Enemy>& enemy : enemys) {

		enemy->Draw();
	}


	//FBX
	//object1->Draw();

	//パーティクル描画
	ParticleManager::BeginDraw(currentCamera);
	//パーティクルテスト
	//particleMan->Draw();

	player->DrawParticle();

	Enemy::DrawParticle();

	//-------前景スプライト描画処理-------//
	Sprite::BeginDraw();

	player->DrawUI();

}

void GamePlayScene::EnemySpawn()
{
	float posZ = Random(50.0f, 100.0f);

	Vector3 start{ -60,0,posZ };
	Vector3 p1 = { 0,30,posZ };
	Vector3 p2 = { -30,0,posZ };
	Vector3 p3 = { 0,-30,posZ };
	Vector3 p4 = { 30,0,posZ };
	Vector3 end = { 60,0,posZ };


	std::vector<Vector3> enemyMovePoints = { start,p1,p2,p3,p4,end };

	//敵の生成と初期化
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	newEnemy->Initialize(enemyMovePoints);
	newEnemy->SetModel(defaultModel.get());
	newEnemy->Spawn();
	
	//リストに登録
	enemys.push_back(std::move(newEnemy));


}
