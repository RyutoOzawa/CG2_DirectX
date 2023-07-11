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
	sprite = std::make_unique<Sprite>();
	sprite2 = std::make_unique<Sprite>();
	backGroundSprite->Initialize(backGroundTexture);
	sprite->Initialize(marioGraph);
	sprite2->Initialize(reimuGraph);

	Enemy::EnemyInitialize(particleGraph);


	skydome = std::make_unique<Model>();
	skydome = Model::CreateModel("skydome");

	defaultModel = std::make_unique<Model>();
	defaultModel = Model::CreateModel();
	defaultModel->textureIndex = reimuGraph;

	playerBulletModel = std::make_unique<Model>();
	playerBulletModel = Model::CreateModel("PlayerBullet");

	triangleModel = std::make_unique<Model>();
	triangleModel = Model::CreateModel("triangle_mat");
	//triangleModel->textureIndex = particleGraph;

	//カメラ初期化
	Vector3 eye(0, 20, -20);	//視点座標
	Vector3 target(0, 0, 6);	//注視点座標
	Vector3 up(0, 1, 0);		//上方向ベクトル

	sprite2->SetPos({ 240, 240 });

	currentCamera = new Camera();
	currentCamera->Initialize(eye, target, up);

	skydomeObj = std::make_unique<Object3d>();
	skydomeObj->Initialize();
	skydomeObj->SetModel(skydome.get());
	skydomeObj->scale = { 1000,1000,1000 };

	planeObj = std::make_unique<Object3d>();
	planeObj->Initialize();
	planeObj->SetModel(defaultModel.get());
	planeObj->scale = { 10.0f,0.01f,10.0f };
	planeObj->position.y = -2.0f;

	triangleObj = std::make_unique<Object3d>();
	triangleObj->Initialize();
	triangleObj->SetModel(triangleModel.get());

	particleMan = std::make_unique<ParticleManager>();
	particleMan->Initialize(particleGraph);

	player = std::make_unique<Player>();
	player->Initialize(defaultModel.get(), reticleGraph);
//	player->SetModel(defaultModel.get());
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

	rayObj = std::make_unique<Object3d>();
	rayObj->Initialize();
	rayObj->SetModel(skydome.get());
	//	rayObj->scale = { 0.01f,10.0f,0.01f };

	newAudio = std::make_unique<AudioManager>();
	newAudio->SoundLoadWave("Resources/bgm_title.wav");

	//球の初期値を設定
	sphere.pos = { -1,1,-0 };
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
	player->Update();

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
	ImGui::SliderFloat("posX", &sphere.pos.x, -10.0f, 10.0f);
	ImGui::SliderFloat("posY", &sphere.pos.y, -10.0f, 10.0f);
	ImGui::SliderFloat("posZ", &sphere.pos.z, -10.0f, 10.0f);
	ImGui::SliderFloat("scaleX", &skydomeObj->scale.x, 0.0f, 5.0f);
	ImGui::SliderFloat("scaleY", &skydomeObj->scale.y, 0.0f, 5.0f);
	ImGui::SliderFloat("scaleZ", &skydomeObj->scale.z, 0.0f, 5.0f);
	ImGui::End();

	////レイの操作
	//ImGui::Begin("Ray");
	//ImGui::Text("cube wo scale de ookiku siteiru tame usiro ha hanntei arimasen");
	//ImGui::SliderFloat("posX", &ray.start.x, -10.0f, 10.0f);
	//ImGui::SliderFloat("posZ", &ray.start.z, -10.0f, 10.0f);
	//ImGui::End();

	Vector2 sprite1Pos = { sprite->GetPosition().x,sprite->GetPosition().y };
	Vector2 sprite2Pos = { sprite2->GetPosition().x,sprite2->GetPosition().y };
	float pos[2] = { sprite1Pos.x,sprite1Pos.y };
	ImGui::Begin("Sprite");

	ImGui::SliderFloat2("sprite1", pos, 0.0f, WindowsAPI::winW);
	sprite->SetPos({ pos[0],pos[1] });


	sprite->Update();
	sprite2->Update();

	ImGui::End();

	skydomeObj->position = sphere.pos;
	skydomeObj->Update();
	planeObj->Update();

	rayObj->position = { 1,1,0 };
	rayObj->Update();


	triangleObj->position = { 0,0,0 };

	triangleObj->camera = currentCamera;

	ImGui::Checkbox("is billboard", &triangleObj->isBillboard);
	ImGui::Checkbox("is billboardY", &triangleObj->isBillboardY);


	triangleObj->Update();

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


	//rayObj->Draw();
	//planeObj->Draw();
	//triangleObj->Draw();

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

	//sprite->Draw();
	//sprite2->Draw();
}

void GamePlayScene::EnemySpawn()
{
	Vector3 start{ -100,0,75 };
	Vector3 p1 = { 0,30,75};
	Vector3 p2 = { -30,0,75};
	Vector3 p3 = { 0,-30,75 };
	Vector3 p4 = { 30,0,75 };
	Vector3 end = { 100,0,75 };

	std::vector<Vector3> enemyMovePoints = { start,p1,p2,p3,p4,end };

	//敵の生成と初期化
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	newEnemy->Initialize(enemyMovePoints);
	newEnemy->SetModel(defaultModel.get());
	newEnemy->Spawn();
	
	//リストに登録
	enemys.push_back(std::move(newEnemy));


}
